#include QMK_KEYBOARD_H
#include "print.h"
#include "keymap_finnish.h"
#include "sendstring_finnish.h"
#include "eeconfig.h"
#include "teelahti.h"
#ifdef TAP_STATS_ENABLE
#    include "tap_stats.h"
#endif

// Chordal hold only enforces the opposite hands rule within the tapping term.
// When the second key is itself a same-hand tap-hold key (F then A), QMK does
// not settle F on that press but waits for A's release, so with the short shift
// term below an ordinary "fa" roll often outlasts it and comes out as "A".
// Achordion used to block this for up to a second. Restore the full tapping term
// for exactly that case; opposite-hand shifting stays fast.
static keypos_t shift_pos;
static bool     shift_pending         = false;
static bool     same_hand_after_shift = false;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef TAP_STATS_ENABLE
    tap_stats_pre_process(keycode, record);
#endif
    if (keycode == HOME_F || keycode == HOME_J) {
        shift_pending         = record->event.pressed;
        shift_pos             = record->event.key;
        same_hand_after_shift = false;
    } else if (shift_pending && record->event.pressed && (IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode)) && chordal_hold_handedness(record->event.key) == chordal_hold_handedness(shift_pos)) {
        same_hand_after_shift = true;
    }
    return true;
}

// Per key tapping term settings
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Reduce the amount of accidental Enter and Space key hits when using those keys
        // as layer change
        case LT_ENT:
        case LT_ENT2:
        case LT_SPC:
        case LT_SPCL:
        case LT_SPC2:
            return TAPPING_TERM - 80;

        // Shift is used frequently and quickly. A shorter tapping term makes shift
        // activate more eagerly, reducing missed shifts during fast typing.
        case HOME_J:
        case HOME_F:
            return same_hand_after_shift ? TAPPING_TERM : TAPPING_TERM - 100;

        default:
            return TAPPING_TERM;
    }
}

// Used alongside chordal hold: permissive hold is enabled per key below,
// not globally, so only the shift keys get the faster resolution.
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Enable permissive hold for shift, but keep it disabled for all other keys
        case HOME_F:
        case HOME_J:
            return true;
        default:
            return false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef TAP_STATS_ENABLE
    tap_stats_process(keycode, record);
#endif
    switch (keycode) {
        case NOTEQUAL:
            if (record->event.pressed) {
                SEND_STRING("!=");
            }
            break;
        case MY_EURO:
            if (record->event.pressed) {
                // Clear modifiers to prevent any held home row mods
                // from contaminating the key combo, then restore.
                uint8_t mods = get_mods();
                clear_mods();
                clear_weak_mods();
                if (get_highest_layer(default_layer_state) == _QWERTY) {
                    tap_code16(FI_CURR);   // macOS Finnish: Shift+4 = €
                } else {
                    tap_code16(ALGR(KC_E));   // Linux Finnish (kotoistus): AltGr+E = €
                }
                set_mods(mods);
            }
            return false;
    }
    return true;
};

void leader_end_user(void) {
    if (leader_sequence_one_key(KC_C)) {
        // Inline code
        SEND_STRING("``" SS_TAP(X_LEFT));
    } else if (leader_sequence_two_keys(KC_C, KC_C)) {
        // Slack code block. Must have the code writing setting on Slack on;
        // If not, use SS_LSFT("\n\n") instead, like:
        // SEND_STRING("```" SS_LSFT("\n\n") "``` " SS_TAP(X_UP));
        SEND_STRING("```\n\n``` " SS_TAP(X_UP));
    } else if (leader_sequence_one_key(KC_S)) {
        // MacOS screenshot
        SEND_STRING(SS_LGUI(SS_LSFT("5")));
    } else if (leader_sequence_one_key(KC_E)) {
        // Email personal
        SEND_STRING("tero@teelahti.fi");
    } else if (leader_sequence_two_keys(KC_E, KC_E)) {
        // Email work
        SEND_STRING("tteelahti@netflix.com");
    } else if (leader_sequence_one_key(KC_T)) {
        SEND_STRING("+358405622455");
    }
}

// Fix caps word for finnish keyboard
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case FI_ODIA:
        case FI_ADIA:
        case FI_ARNG:
        case KC_MINS:
        case FI_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        case FI_UNDS:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}
