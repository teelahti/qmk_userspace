#pragma once

#include QMK_KEYBOARD_H
#include "keymap_finnish.h"

enum layers {
    _QWERTY,
    _NUM,
    _SNUM,
    _FNUM,
    _NAV,
    _CODE,
    _CTL
};

enum custom_keycodes {
    NOTEQUAL = SAFE_RANGE,
};

// Left hand home row
#define HOME_A LGUI_T(KC_A)
#define HOME_S LALT_T(KC_S)
#define HOME_D LCTL_T(KC_D)
#define HOME_F LSFT_T(KC_F)

// right hand home row
#define HOME_J RSFT_T(KC_J)
#define HOME_K RCTL_T(KC_K)
#define HOME_L RALT_T(KC_L)
#define HOME_OE RGUI_T(FI_ODIA)

// Function shortcuts
#define DELWRD RALT(KC_DEL)
#define RDELWRD RALT(KC_BSPC)
#define SF12 LSFT(KC_F12)    // SHIFT + F12
#define CMIN LCTL(FI_MINS)   // CTRL + -

// Layer change keys
// Change the base layer between Mac and Win
#define LT_ENT LT(_FNUM,KC_ENT)
#define LT_ENT2 LT(_CTL,KC_ENT)
#define LT_SPC LT(_CODE,KC_SPC)
#define LT_SPC2 LT(_SNUM,KC_SPC)
#define LT_DEL LT(_FNUM,KC_DEL)
#define LT_NUM LT(_NUM,KC_DEL)
#define LT_NAV LT(_NAV,KC_BSPC)

// Some of the default FI_* symbols do not work with MacOS
#define PIPE LALT(KC_7)    // FI_PIPE, |
#define BSLS LALT(KC_AMPR) // FI_BSLS
#define LCBR LALT(KC_ASTR) // FI_LCBR, {
#define RCBR LALT(KC_LPRN) // FI_RCBR, }
#define LABK KC_GRV        // FI_GRV, <
#define RABK LSFT(KC_GRV)  // FI_RABK, >
#define AT LALT(KC_2)

