#include QMK_KEYBOARD_H
#include "print.h"
#include "keymap_finnish.h"
#include "sendstring_finnish.h"
#include "eeconfig.h"
#include "teelahti.h" 


// Per key tapping term settings
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // Reduce the amount of accidental Enter and Space key hits when using those keys
        // as layer change
        case LT_ENT:
        case LT_SPC:
            return TAPPING_TERM - 40;

        // Using shift is usually fast, reduce tap term for them to avoid getting
        // individual keys instead of shift + key. This is needed, since when
        // IGNORE_MOD_TAP_INTERRUPT is on, both keys, modifier and another key, need to be held
        // at least for TAPPING_TERM for the modified symbol to appear.
        case HOME_J:
        case HOME_F:
            return TAPPING_TERM - 70;

        default:
            return TAPPING_TERM;
    }
}

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
    switch (keycode) {
        case NOTEQUAL:
            if (record->event.pressed) {
                // when keycode NOTEQUAL is pressed
                SEND_STRING("!=");
            } else {
                // when keycode NOTEQUAL is released
            }
            break;
    }
        // If console is enabled, it will print the matrix position and status of each key pressed
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %u, row: %u, pressed: %b, time: %u, interrupt: %b, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif
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


