#include QMK_KEYBOARD_H
#include "keymap.h"
#include "teelahti.h"


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * Has some duplication since this keyboard has more thumb keys than some others I use.
 * Originally had a layer under backspace, but that caused too many mistakes so opted out of it.
 *
 * F-keys are needed needed for trackball control.
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  '   |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * | TAB  |   A  |   S  |   D  |   F  |   G  |                              |   H  |   J  |   K  |   L  |   Ö  |  Ä   |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * | Med  |   Z  |   X  |   C  |   V  |   B  |      |      |  |      |      |   N  |   M  |   ,  |   .  |   -  | m_l  |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      | F19  | F18  |  Del | Space| Enter|  | Enter| Space| Bspc | Lead | m_r  |
 *                      |      |      |  Num | SNum | Nav  |  |  Ctl | Code |      |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      KC_ESC,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                      KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
      KC_TAB,  HOME_A, HOME_S, HOME_D, HOME_F, KC_G,                                      KC_H,   HOME_J,  HOME_K,  HOME_L,  HOME_OE, FI_ADIA,
      LT_MED,  KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   LT_NAV, KC_DEL, QK_LEAD, KC_BSPC,  KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH, QK_MOUSE_BUTTON_1,
                               KC_F19, KC_F18, LT_NUM, LT_SPC2,LT_ENT, LT_ENT2, LT_SPC,   KC_BSPC, QK_LEAD,QK_MOUSE_BUTTON_2
    ),
/*
 * Alternative base Layer for Linux 1: QWERTY
 * Layers are changed with magic keys based on index: it is important that qwertys are 0 and 1.
 * Changes:
 *  - Space opens Linux version of code layer
 */
    [_QWERTY_LINUX] = LAYOUT(
      KC_ESC,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                      KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
      KC_TAB,  HOME_A, HOME_S, HOME_D, HOME_F, KC_G,                                      KC_H,   HOME_J,  HOME_K,  HOME_L,  HOME_OE, FI_ADIA,
      LT_MED,  KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   LT_NAV, KC_DEL, QK_LEAD, KC_BSPC,  KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH, QK_MOUSE_BUTTON_1,
                               KC_F19, KC_F18, LT_NUM, LT_SPC2,LT_ENT, LT_ENT2, LT_SPCL,  KC_BSPC, QK_LEAD,QK_MOUSE_BUTTON_2
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
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, FI_EQL,  KC_1,    KC_2,    KC_3, FI_COMM,  FI_DOT,
                                 _______, _______, _______, _______, _______, _______, _______, KC_0,   _______,  _______
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
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, NOTEQUAL,FI_EXLM, FI_DQUO, FI_HASH, FI_SCLN ,FI_COLN,
                                 _______, _______, _______, _______, _______, _______, _______, FI_EQL , _______, _______
    ),

/*
 * Coding layer, entered with LT_SPC
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * |   `  |   [  |  ]   |  |   |  \   |  ´   |                              |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |   ~  |   {  |  }   |  (   |  )   |  "   |                              |      | Shift| Ctl  | Alt  | GUI  |      |
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
 * Alternative coding layer for linux, entered with LT_SPCL
 * Changes:
 *  - Use default finnish keycodes from keymap_finnish.h
 *
 */
    [_CODE_LINUX] = LAYOUT(
      FI_GRV,  FI_LBRC, FI_RBRC, FI_PIPE, FI_BSLS, FI_ACUT,                                     _______, _______, _______, _______, _______, _______,
      FI_TILD, FI_LCBR, FI_RCBR, FI_LPRN, FI_RPRN, FI_DQUO,                                     _______, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, _______,
      FI_AT,   FI_LABK, FI_RABK, FI_EXLM, FI_EQL,  FI_CIRC, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Navigation Layer, entered with Nav
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * |      |      |      |      |      |      |                              | Home | PgDn | PgUp | End  |      |      |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |      | GUI  | Alt  | Ctl  | Shift|      |                              | Left | Down |  Up  | Right|      |CapsLk|
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * |      |      |      |      |      |      |      |      |  |      |      | RDelW| Ctl- | Ctl+ | DelW |      |      |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |      |      |      |  |      |      |      |      |      |
 *                      |      |      |      |      | Nav  |  |      | Del  |      |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_HOME, KC_PGDN,  KC_PGUP, KC_END,  _______, _______,
      _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, _______,                                     KC_LEFT, KC_DOWN,  KC_UP,   KC_RGHT, _______, KC_CAPS,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RDELWRD, CMIN,     CPLUS,   DELWRD,  _______, _______,
                                 _______, _______, _______, _______, _______, _______, KC_DEL,  _______,  _______, _______
    ),
/*
 * Media Layer, entered with Med
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * |      |      |      |      |      |      |                              |      | Mute | Play |      |      |      |
 * |------+------+------+------+------+------|                              |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                              | Prev |VolDwn|VolUp | Next |      |      |
 * |------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+------|
 * | Med  |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |      |
 * `--------------------+------+------+------+------+------|  |------+------+------+------+------+--------------------'
 *                      |      |      |      |      |      |  |      |      |      |      |      |
 *                      |      |      |      |      |      |  |      |      |      |      |      |
 *                      `----------------------------------'  `----------------------------------'
 */
    [_MEDIA] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, KC_MUTE, KC_MPLY, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     KC_MRWD, KC_VOLD, KC_VOLU, KC_MFFD, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Adjust Layer, entered with LT_ENT2
 *
 * ,-----------------------------------------.                              ,-----------------------------------------.
 * | Sleep|      |      |      |  MAC | Linux|                              |      |      |      |      |      |      |
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
      KC_SLEP, _______, _______, _______,  PDF(0),  PDF(1),                                     _______, _______, _______, _______, _______, _______,
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
    // If we didn't do any changes to default layer, i.e. no PDF() above, we
    // could just use 'get_hightest_layer(layer_state)'. Based on this article:
    //   https://www.reddit.com/r/olkb/comments/o5924u/get_heighest_layer_and_default_layer_change/
    // we can use 'layer_state|default_layer_state' to catch the swapped layer.
    switch (get_highest_layer(layer_state|default_layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("QWERTY MAC\n\n"), false);
            oled_write_P(PSTR("                  \n"), false);
            oled_write_P(PSTR("                  \n"), false);
            oled_write_P(PSTR("                  \n"), false);
            oled_write_P(PSTR("                  \n"), false);
            break;
        case _QWERTY_LINUX:
            oled_write_P(PSTR("QWERTY LINUX\n\n"), false);
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
        case _CODE_LINUX:
            oled_write_P(PSTR("Code:\n\n"), false);
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
            oled_write_P(PSTR("          MAC LIN\n"), false);
            oled_write_P(PSTR("                 \n"), false);
            oled_write_P(PSTR("              RES\n"), false);
            oled_write_P(PSTR("                 \n"), false);
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
