#pragma once

// ---------------------------------------------------------------------------
// Slave half not starting after a KVM switch
// ---------------------------------------------------------------------------
// The slave's LEDs are driven entirely over the split link (RGB_MATRIX_SPLIT,
// split_count [31,31]), and the master re-sends that state at least every
// FORCED_SYNC_THROTTLE_MS (100ms). So a permanently dark slave is never an LED
// bug -- it means the slave is not running its main loop at all.
//
// Two ways that can happen on this board, both provoked by the flaky VBUS a
// monitor hub / KVM produces:

// 1. rev4/config.h sets RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT to 1000U.
//    That leaves a full second at every boot where the double-tap magic word is
//    live in SRAM; any reset landing in that window -- including a brownout on
//    the slave, whose 5V arrives through the interconnect -- makes the RP2040
//    jump into the UF2 bootloader instead of the firmware. The half then sits
//    there dark until it is power cycled. 200U is the QMK default and shrinks
//    the vulnerable window 5x while still allowing a deliberate double tap.
#undef RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U

// 2. If the slave ever misreads USB_VBUS_PIN as high it declares itself master,
//    never calls transport_slave_init(), and then wedges forever in the
//    USB_SUSPENDED loop in protocol_pre_task(). QMK's split watchdog does NOT
//    recover this: split_watchdog_task() only resets a half that knows it is
//    the slave. reset_if_false_master() in keymap.c handles it instead.
//
// The split watchdog still covers a correctly detected slave that wedges for
// any other reason: it reboots if the master has not talked to it within the
// timeout.
#define SPLIT_WATCHDOG_ENABLE
#define SPLIT_WATCHDOG_TIMEOUT 3000

// Persistent boot log (users/teelahti/boot_log.c): 16 entries of 16 bytes in
// EEPROM, and two split RPCs to pair the halves' logs and read the slave's.
#define EECONFIG_USER_DATA_SIZE 256
#define SPLIT_TRANSACTION_IDS_USER BOOT_LOG_HELLO, BOOT_LOG_FETCH

// KVMs and monitor hubs bounce the USB device state wildly on wakeup. Give
// things time to settle before trusting the matrix again.
#define USB_SUSPEND_WAKEUP_DELAY 200
