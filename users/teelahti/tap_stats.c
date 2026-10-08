#include QMK_KEYBOARD_H
#include "print.h"
#include "tap_stats.h"

// Streams tap-hold timing data over the QMK console for tools/tap_stats.
//
// Only tap-hold keys and Backspace are identified. Every other key is logged
// as its hand alone, so the stream carries the timing needed to tune tapping
// terms without carrying the text being typed.
//
//   TS p|r <time> <hand> <slot> <keycode>   physical press/release
//   TS t|h <time> <keycode>                 how a tap-hold key settled; <time>
//                                           is its press time, joining it to
//                                           the press
//
// <slot> is the lowest index free among the keys currently held. It pairs each
// release with its press, even for anonymous keys, without saying which key.

static uint8_t  slot_of[MATRIX_ROWS][MATRIX_COLS];
static uint16_t used_slots = 0;

static bool is_tap_hold(uint16_t keycode) {
    return IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode);
}

static uint8_t take_slot(keypos_t key) {
    uint8_t slot = 0;
    while (slot < 15 && (used_slots & (1u << slot))) {
        slot++;
    }
    used_slots |= 1u << slot;
    slot_of[key.row][key.col] = slot;
    return slot;
}

static uint8_t free_slot(keypos_t key) {
    uint8_t slot = slot_of[key.row][key.col];
    used_slots &= ~(1u << slot);
    return slot;
}

// Called from pre_process_record_user, i.e. before the tap-hold state machine
// has buffered or reordered anything.
void tap_stats_pre_process(uint16_t keycode, keyrecord_t *record) {
    if (!IS_KEYEVENT(record->event)) {
        return;
    }
    keypos_t key    = record->event.key;
    uint8_t  slot   = record->event.pressed ? take_slot(key) : free_slot(key);
    uint16_t logged = (is_tap_hold(keycode) || keycode == KC_BSPC) ? keycode : 0;
    uprintf("TS %c %u %c %u %04X\n", record->event.pressed ? 'p' : 'r', record->event.time, chordal_hold_handedness(key), slot, logged);
}

// Called from process_record_user, which sees a tap-hold press only once it
// has settled.
void tap_stats_process(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed && is_tap_hold(keycode)) {
        uprintf("TS %c %u %04X\n", record->tap.count ? 't' : 'h', record->event.time, keycode);
    }
}
