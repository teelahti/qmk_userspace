#include QMK_KEYBOARD_H
#include "print.h"
#include "split_util.h"
#include "transactions.h"
#include "usb_device_state.h"
#include "usb_util.h"
#include "hardware/structs/watchdog.h"
#include "hardware/structs/vreg_and_chip_reset.h"
#include "boot_log.h"

// Persistent boot log, one per half, for the Halcyon Kyria's dark slave.
//
// Each boot that gets as far as running the firmware leaves an entry in the
// half's own EEPROM (wear-levelled RP2040 flash): why the chip reset, which
// role it picked and what USB_VBUS_PIN read while it did, and how far the boot
// got. The master's entry also records the slave's boot number once the halves
// talk, and the other way round, which pairs the two logs up.
//
// Plugging in the master powers both halves at once, so every attempt should
// add an entry on both sides. A master entry whose slave never answered, with
// no matching slave entry, means the slave's firmware never ran at all: the
// RP2040 bootloader trap or a power fault. A matching slave entry shows where
// it stopped instead.
//
// RP2040 only (reset cause registers), and needs EECONFIG_USER_DATA_SIZE and
// SPLIT_TRANSACTION_IDS_USER from the keymap's config.h. BOOT_LOG_DUMP prints
// both halves' logs to the QMK console.

#define BOOT_LOG_ENTRIES 16

enum {
    FLAG_MASTER     = 1 << 0, // picked the master role
    FLAG_VBUS       = 1 << 1, // USB_VBUS_PIN read high when picking it
    FLAG_LINK       = 1 << 2, // the other half answered / was heard from
    FLAG_USB        = 1 << 3, // USB configured by a host
    FLAG_POR        = 1 << 4, // power-on or brownout reset
    FLAG_RUN        = 1 << 5, // RUN pin (reset button) reset
    FLAG_WATCHDOG   = 1 << 6, // hardware watchdog reset
    FLAG_SOFT_RESET = 1 << 7, // mcu_reset() from the firmware, see reset_reason
};

enum {
    REASON_NONE,
    REASON_FALSE_MASTER, // reset_if_false_master() in the Halcyon keymap
    REASON_OTHER,        // anything else, in practice QMK's split watchdog
};

typedef struct __attribute__((packed)) {
    uint16_t boot;         // increasing boot number of this half, 0 = empty slot
    uint8_t  flags;        // FLAG_*
    uint8_t  vbus_samples; // USB_VBUS_PIN, bit n = sample n, 1ms apart from the decision
    uint8_t  reset_reason; // REASON_* when FLAG_SOFT_RESET
    uint8_t  reserved;
    uint16_t peer_boot;    // the other half's boot number, once linked
    uint16_t link_ms;      // uptime when the halves first talked
    uint16_t usb_ms;       // uptime when USB got configured (master)
    uint32_t chip_reset;   // raw VREG_AND_CHIP_RESET.CHIP_RESET
} boot_entry_t;

_Static_assert(sizeof(boot_entry_t) * BOOT_LOG_ENTRIES <= EECONFIG_USER_DATA_SIZE, "EECONFIG_USER_DATA_SIZE too small for the boot log");
_Static_assert(sizeof(boot_entry_t) <= RPC_S2M_BUFFER_SIZE, "boot log entry does not fit an RPC");

// Watchdog scratch registers survive mcu_reset() but not a chip reset, so they
// carry the reason across a reset the firmware asked for. Scratch 4-7 belong
// to the bootrom.
#define SOFT_RESET_MAGIC 0xB0071060u
static uint8_t pending_reason = REASON_OTHER;

// RAM copy of the whole log, so the slave's RPC handler (which runs outside
// the main loop) never touches flash. `entry` is this boot's slot in it.
static boot_entry_t  boot_log[BOOT_LOG_ENTRIES];
static boot_entry_t *entry;
static uint8_t      slot;
static uint8_t      vbus_samples;
static bool         vbus_first;
static volatile bool ready = false;

static volatile bool     slave_heard   = false;
static volatile uint16_t slave_peer    = 0;
static volatile uint16_t slave_heard_at;

static void save(void) {
    eeconfig_update_user_datablock(entry, slot * sizeof(boot_entry_t), sizeof(boot_entry_t));
}

static uint16_t uptime(void) {
    uint32_t t = timer_read32();
    return t > UINT16_MAX ? UINT16_MAX : t;
}

// Same decision as QMK's default (USB_VBUS_PIN, no SPLIT_USB_DETECT), plus a
// few extra samples to show whether the pin was still settling.
bool is_keyboard_master_impl(void) {
    bool is_master = usb_vbus_state();
    vbus_first     = is_master;
    vbus_samples   = is_master;
    for (uint8_t i = 1; i < 8; i++) {
        wait_ms(1);
        vbus_samples |= usb_vbus_state() << i;
    }
    if (!is_master) {
        usb_disconnect();
    }
    return is_master;
}

void mcu_reset(void) {
    watchdog_hw->scratch[0] = SOFT_RESET_MAGIC;
    watchdog_hw->scratch[1] = pending_reason;
    NVIC_SystemReset();
}

void boot_log_false_master_reset(void) {
    pending_reason = REASON_FALSE_MASTER;
}

// Slave side: the master announces itself with its boot number and gets ours,
// or 0 while this half is still booting.
static void hello_handler(uint8_t in_len, const void *in, uint8_t out_len, void *out) {
    uint16_t reply = 0;
    if (ready && in_len >= sizeof(uint16_t)) {
        if (!slave_heard) {
            memcpy((void *)&slave_peer, in, sizeof(uint16_t));
            slave_heard_at = uptime();
            slave_heard    = true;
        }
        reply = entry->boot;
    }
    if (out_len >= sizeof(reply)) {
        memcpy(out, &reply, sizeof(reply));
    }
}

// Slave side: hand out one entry of our log.
static void fetch_handler(uint8_t in_len, const void *in, uint8_t out_len, void *out) {
    if (ready && in_len >= 1 && out_len >= sizeof(boot_entry_t)) {
        memcpy(out, &boot_log[*(const uint8_t *)in % BOOT_LOG_ENTRIES], sizeof(boot_entry_t));
    }
}

void keyboard_post_init_user(void) {
    eeconfig_read_user_datablock(boot_log, 0, sizeof(boot_log));
    uint16_t last = 0;
    for (uint8_t i = 0; i < BOOT_LOG_ENTRIES; i++) {
        if (boot_log[i].boot > last) {
            last = boot_log[i].boot;
        }
    }

    uint16_t boot = last == UINT16_MAX ? 1 : last + 1;
    slot          = boot % BOOT_LOG_ENTRIES;
    entry         = &boot_log[slot];
    memset(entry, 0, sizeof(*entry));
    entry->boot         = boot;
    entry->vbus_samples = vbus_samples;
    entry->chip_reset   = vreg_and_chip_reset_hw->chip_reset;

    if (is_keyboard_master()) entry->flags |= FLAG_MASTER;
    if (vbus_first) entry->flags |= FLAG_VBUS;
    if (entry->chip_reset & VREG_AND_CHIP_RESET_CHIP_RESET_HAD_POR_BITS) entry->flags |= FLAG_POR;
    if (entry->chip_reset & VREG_AND_CHIP_RESET_CHIP_RESET_HAD_RUN_BITS) entry->flags |= FLAG_RUN;
    if (watchdog_hw->reason) entry->flags |= FLAG_WATCHDOG;
    if (watchdog_hw->scratch[0] == SOFT_RESET_MAGIC) {
        entry->flags |= FLAG_SOFT_RESET;
        entry->reset_reason = watchdog_hw->scratch[1];
    }
    watchdog_hw->scratch[0] = 0;

    save();
    transaction_register_rpc(BOOT_LOG_HELLO, hello_handler);
    transaction_register_rpc(BOOT_LOG_FETCH, fetch_handler);
    ready = true;
}

void boot_log_task(void) {
    if (!ready) {
        return;
    }
    if (is_keyboard_master()) {
        if (!(entry->flags & FLAG_USB) && usb_device_state_get_configure_state() == USB_DEVICE_STATE_CONFIGURED) {
            entry->flags |= FLAG_USB;
            entry->usb_ms = uptime();
            save();
        }
        // Say hello until the slave answers, but only while QMK thinks the link
        // is up, so a missing slave does not cost a timeout every few ms.
        static uint32_t last_try = 0;
        if (!(entry->flags & FLAG_LINK) && is_transport_connected() && timer_elapsed32(last_try) > 100) {
            last_try = timer_read32();
            uint16_t peer = 0;
            if (transaction_rpc_exec(BOOT_LOG_HELLO, sizeof(entry->boot), &entry->boot, sizeof(peer), &peer) && peer != 0) {
                entry->flags |= FLAG_LINK;
                entry->link_ms   = uptime();
                entry->peer_boot = peer;
                save();
            }
        }
    } else if (slave_heard && !(entry->flags & FLAG_LINK)) {
        entry->flags |= FLAG_LINK;
        entry->link_ms   = slave_heard_at;
        entry->peer_boot = slave_peer;
        save();
    }
}

static void print_entry(const boot_entry_t *e) {
    if (e->boot == 0) {
        return;
    }
    uprintf("  #%5u %-6s vbus=%u samples=%02X reset:%s%s%s%s", e->boot, (e->flags & FLAG_MASTER) ? "master" : "slave", (e->flags & FLAG_VBUS) ? 1 : 0, e->vbus_samples, (e->flags & FLAG_POR) ? " power" : "", (e->flags & FLAG_RUN) ? " button" : "", (e->flags & FLAG_WATCHDOG) ? " hw-watchdog" : "", (e->flags & FLAG_SOFT_RESET) ? (e->reset_reason == REASON_FALSE_MASTER ? " false-master" : " soft") : "");
    if (e->flags & FLAG_LINK) {
        uprintf(" link=%ums peer=#%u", e->link_ms, e->peer_boot);
    } else {
        uprintf(" link=NEVER");
    }
    if (e->flags & FLAG_USB) {
        uprintf(" usb=%ums", e->usb_ms);
    }
    uprintf(" chip_reset=%08lX\n", (unsigned long)e->chip_reset);
}

// Oldest first, starting from the slot after the newest.
static void print_log(bool (*read)(uint8_t i, boot_entry_t *e)) {
    boot_entry_t copy[BOOT_LOG_ENTRIES];
    uint8_t      newest = 0;
    for (uint8_t i = 0; i < BOOT_LOG_ENTRIES; i++) {
        if (!read(i, &copy[i])) {
            uprintf("  (unreachable)\n");
            return;
        }
        if (copy[i].boot > copy[newest].boot) {
            newest = i;
        }
    }
    for (uint8_t n = 1; n <= BOOT_LOG_ENTRIES; n++) {
        print_entry(&copy[(newest + n) % BOOT_LOG_ENTRIES]);
    }
}

static bool read_local(uint8_t i, boot_entry_t *e) {
    *e = boot_log[i];
    return true;
}

static bool read_remote(uint8_t i, boot_entry_t *e) {
    memset(e, 0, sizeof(*e));
    return transaction_rpc_exec(BOOT_LOG_FETCH, 1, &i, sizeof(*e), e);
}

void boot_log_dump(void) {
    if (!is_keyboard_master()) {
        return;
    }
    uprintf("BOOTLOG uptime=%lums\n", (unsigned long)timer_read32());
    uprintf("BOOTLOG %s half (master, this boot #%u):\n", is_keyboard_left() ? "left" : "right", entry->boot);
    print_log(read_local);
    uprintf("BOOTLOG %s half (slave):\n", is_keyboard_left() ? "right" : "left");
    print_log(read_remote);
    uprintf("BOOTLOG end\n");
}
