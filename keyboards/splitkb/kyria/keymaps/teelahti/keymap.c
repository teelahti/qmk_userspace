#include QMK_KEYBOARD_H
#include "print.h"
#include "features/achordion.h"
#include "keymap_finnish.h"
#include "sendstring_finnish.h"
#include "eeconfig.h"
#include "keymap.h"

enum custom_keycodes {
    NOTEQUAL = SAFE_RANGE,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer 1: QWERTY
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  '   |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * | TAB  |   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  |   Ö  |  Ä   |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * | LSFT |   Z  |   X  |   C  |   V  |   B  |  Nav | Del  |  |Leader| Bspc |   N  |   M  |   ,  |   .  |   -  | Lead |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |  Del | Space| Enter|  | Enter| Space| Bspc |      |      |
 *                      |      |      |  Num | Snum | FNum |  |  Ctl | Code | Nav  |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      KC_ESC,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                      KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
      KC_TAB,  HOME_A, HOME_S, HOME_D, HOME_F, KC_G,                                      KC_H,   HOME_J,  HOME_K,  HOME_L,  HOME_OE, FI_ADIA,
      KC_LSFT, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   LT_NAV, KC_DEL, QK_LEAD, KC_BSPC,  KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH, QK_LEAD,
                               _______,_______,LT_NUM, LT_SPC2,LT_ENT, LT_ENT2, LT_SPC,   LT_NAV, _______, _______
    ),
/*
 * Numpad layer, activated with LT_NUM
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * |      |      |      |      |      |      |                              |   *  | 7 /  | 8 (  | 9 )  |  +   |  å   |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctl  | Shift|      |                              |   /  | 4 €  | 5 %  | 6 &  |  -   |      |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |  |      |      |   =  | 1 !  | 2 "  | 3 #  |  ,   |  .   |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |      |      |      |  |      |      |   0  |      |      |
 *                      |      | Prev |  Num | Next | Play |  |      |      |      |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_NUM] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     FI_ASTR, KC_7,    KC_8,    KC_9, FI_PLUS, FI_ARNG,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     FI_SLSH, KC_4,    KC_5,    KC_6, FI_MINS, _______,
      _______, _______, _______, KC_VOLD, KC_MUTE, _______, _______, _______, _______, _______, FI_EQL,  KC_1,    KC_2,    KC_3, FI_COMM,  FI_DOT,
                                 _______, KC_MRWD, _______, KC_MFFD, KC_MPLY, _______, _______, KC_0,   _______,  _______
    ),
/*
 * Shifted numbers, activated with LT_SPC2
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * |      |      |      |      |      |      |                              |      |  /   |  (   |  )   |  ?   |  ¨   |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctl  | Shift|      |                              |      |  €   |  %   |  &   |  _   |      |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |  |      |      |  !=  |  !   |  "   |  #   |  ;   |  :   |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |      |      |      |  |      |      |      |      |      |
 *                      |      |      |      | SNum |      |  |      |      |   =  |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_SNUM] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, FI_SLSH, FI_LPRN, FI_RPRN, FI_QUES, FI_DIAE,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     _______, FI_CURR, FI_PERC, FI_AMPR, FI_UNDS, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, NOTEQUAL,FI_EXLM, FI_DQUO, FI_HASH,FI_SCLN ,FI_COLN,
                                 _______, _______, _______, _______, _______, _______, _______, FI_EQL , _______, _______
    ),

/*
 * F-keys layer, entered with FNum
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * |      |      |      |      |      |      |                              |      | F7   | F8   | F9   | F10  |      |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctl  | Shift|      |                              |      | F4   | F5   | F6   | F11  |      |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |  |      |      |      | F1   | F2   | F3   | F12  |      |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |      |      |      |  |      |      |      |      |      |
 *                      |      |      |      |      | FNum |  |      |      |      |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_FNUM] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     _______, KC_F4,   KC_F5,   KC_F6,   KC_F11,  _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F12,  _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Coding layer, entered with LT_SPC
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * |   `  |   [  |  ]   |  |   |  \   |  ´   |                              |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |   ~  |   {  |  }   |  (   |  )   |  "   |                              |      | Shift| Ctl  |  Alt | GUI  |      |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * |   @  |   <  |  >   |  !   |  =   |  ^   |      |      |  |      |      |      |      |      |      |      |      |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |      |      |      |  |      |      |      |      |      |
 *                      |      |      |      |      |      |  |      | Code |      |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_CODE] = LAYOUT(
      FI_GRV,  FI_LBRC, FI_RBRC, PIPE,    BSLS,    FI_ACUT,                                     _______, _______, _______, _______, _______, _______,
      FI_TILD, LCBR,    RCBR,    FI_LPRN, FI_RPRN, FI_DQUO,                                     _______, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, _______,
      AT,      LABK,    RABK,    FI_EXLM, FI_EQL,  FI_CIRC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Navigation Layer, entered with Nav
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * | Sleep|      |      |      |      |      |                              |      | PgUp |  Up  | PgDn |      |ScrlLk|
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctl  | Shift|      |                              | Home | Left | Down | Right| End  |CapsLk|
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * |      |      |      |      |      |      | Nav  |      |  |      | Mute |RDelW | Bspc |  Del | DelW | Ctl- |      |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |      |      |      |  |      |      |      |      |      |
 *                      |      |      |      |      | Mute |  | VolDn| VolUp| Nav  |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      KC_SLEP, _______, _______, _______, _______, _______,                                     _______, KC_PGUP,  KC_UP,   KC_PGDN, _______, KC_SCRL,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_HOME, KC_LEFT,  KC_DOWN, KC_RGHT, KC_END,  KC_CAPS,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MUTE, RDELWRD ,KC_BSPC , KC_DEL,  DELWRD,  CMIN,    _______,
                                 _______, _______, _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______,  _______, _______
    ),
/*
 * Adjust Layer, entered with LT_ENT2
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * |      |      |      |      |      |      |                              |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                              |      |      |      |      |      |      |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * |      |      |      |      |      | RES  |      |      |  |      |      |      |      |      |      |      |      |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |      |      |      |  |      |      |      |      |      |
 *                      |      |      |      |      |      |  | CTL  |      |      |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_CTL] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
// /*
//  * Layer template
//  *
//  * ,-----------------------------------------.                              ,-----------------------------------------.
//  * |      |      |      |      |      |      |                              |      |      |      |      |      |      |
//  * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
//  * |      |      |      |      |      |      |                              |      |      |      |      |      |      |
//  * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
//  * |      |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |      |
//  * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
//  *                      |      |      |      |      |      |  |      |      |      |      |      |
//  *                      |      |      |      |      |      |  |      |      |      |      |      |
//  *                      `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

// clang-format on

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
    // To further customize Achordion library that allows mods only on opposite hands see
    // achordion_chord below.
    if (!process_achordion(keycode, record)) {
        return false;
    }

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

// Customize how the achordion add-on blocks keypresses on same side.
//     https://getreuer.info/posts/keyboards/achordion/index.html
// The default functionality is:
// bool achordion_chord(uint16_t tap_hold_keycode,
//                  keyrecord_t* tap_hold_record,
//                  uint16_t other_keycode,
//                  keyrecord_t* other_record) {
//   return achordion_opposite_hands(tap_hold_record, other_record);
// }
bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    // Exceptionally consider the following chords as holds, even though they
    // are on the same hand. This is to allow my nav layer to work properly.
    switch (tap_hold_keycode) {
        case LT_NAV:
            // On split keyboard the 0..MATRIX_ROWS/2 are on left side, and the rest on right.
            // Allow anything on right hand side with our nav press.
            if (other_record->event.key.row >= MATRIX_ROWS / 2) {
                return true;
            }
            break;
    }

    // Otherwise, follow the opposite hands rule.
    return achordion_opposite_hands(tap_hold_record, other_record);
}

void matrix_scan_user(void) {
    achordion_task();
}

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

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    // debug_enable=true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

static void render_signature(void) {
    // clang-format off
  static const char PROGMEM img[] = {
    // Generate with https://javl.github.io/image2cpp/, 128x64px, plain bytes, vertical, 1 bit per pixel
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x60, 0xf0, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xe0,
    0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xf0,
    0xf0, 0xf0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x00,
    0x00, 0x00, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x00,
    0x00, 0x00, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f,
    0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x1f, 0x0f, 0x0f,
    0x0f, 0x07, 0x0f, 0x0f, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0xe0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x8f, 0x8f, 0xcf, 0xcf, 0xc7, 0xc7, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc7, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0x07, 0x0f, 0x1f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x07, 0x00,
    0x00, 0x00, 0x03, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x0f, 0x07, 0x07, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80,
    0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x80, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x80, 0xe0, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9f, 0x0f, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x87, 0x87, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8,
    0xf0, 0xf8, 0xfe, 0xff, 0xff, 0xff, 0x7f, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x0f, 0x07, 0x00, 0x00, 0x00,
    0x00, 0x0c, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x0f, 0x0f, 0x03, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x07, 0x0f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
    0x3f, 0x3f, 0x1f, 0x1f, 0x0f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1e, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x1f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x78, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
    // clang-format on

    oled_write_raw_P(img, sizeof(img));
}

static void render_status(void) {
    // Host Keyboard Layer Status
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("QWERTY\n\n"), false);
            oled_write_P(PSTR("                  \n"), false);
            oled_write_P(PSTR("                  \n"), false);
            oled_write_P(PSTR("                  \n"), false);
            oled_write_P(PSTR("                  \n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("Num:\n\n"), false);
            oled_write_P(PSTR("* 7 8 9 + o\n"), false);
            oled_write_P(PSTR("/ 4 5 6 - a\n"), false);
            oled_write_P(PSTR("= 1 2 3 , .\n"), false);
            oled_write_P(PSTR("0\n"), false);
            break;
        case _SNUM:
            oled_write_P(PSTR("Shifted num:\n\n"), false);
            oled_write_P(PSTR("   / ( ) ? ''\n"), false);
            oled_write_P(PSTR("   e % & _\n"), false);
            oled_write_P(PSTR("!= ! \" # ; :\n"), false);
            oled_write_P(PSTR("=\n"), false);
            break;
        case _FNUM:
            oled_write_P(PSTR("F-keys:\n\n"), false);
            oled_write_P(PSTR("  F7  F8  F9  F10\n"), false);
            oled_write_P(PSTR("  F4  F5  F6  F11\n"), false);
            oled_write_P(PSTR("  F1  F2  F3  F12\n"), false);
            oled_write_P(PSTR("\n"), false);
            break;
        case _CODE:
            oled_write_P(PSTR("Code MAC:\n\n"), false);
            oled_write_P(PSTR("` [ ] | \\ ´\n"), false);
            oled_write_P(PSTR("~ { } ( ) \"\n"), false);
            oled_write_P(PSTR("@ < > ! = ^\n"), false);
            oled_write_P(PSTR("\n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("Navigation:\n\n"), false);
            oled_write_P(PSTR("    PU U PD\n"), false);
            oled_write_P(PSTR("Hom L  D R  E\n"), false);
            oled_write_P(PSTR("RDW BS D DW C-\n"), false);
            oled_write_P(PSTR("\n"), false);
            break;
        case _CTL:
            oled_write_P(PSTR("Adjust:\n\n"), false);
            oled_write_P(PSTR("\n"), false);
            oled_write_P(PSTR("\n"), false);
            oled_write_P(PSTR("                 RES\n"), false);
            oled_write_P(PSTR("\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_signature();
    }
    return false;
}
#endif
