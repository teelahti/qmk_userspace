/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Dummy
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {{{ KC_NO }}};

void suspend_power_down_user(void) {
    // Switch off sensor + LED making trackball unable to wake host
    adns5050_power_down();
}

void suspend_wakeup_init_user(void) {
    adns5050_init();
}

// Copied from:
// https://github.com/t4corun/qmk_userspace/blob/main/keyboards/ploopyco/trackball_nano/keymaps/t4corun/keymap.c
//
// Flash with:
//   qmk flash -kb ploopyco/trackball_nano -km teelahti
static bool num_lock_state = false;
static bool scroll_lock_state = false;

void keyboard_post_init_user(void) {
    num_lock_state  = host_keyboard_led_state().num_lock;
    scroll_lock_state  = host_keyboard_led_state().scroll_lock;
}

// React based on incoming led states. The methods we call to enable drag etc. are defined in
// https://github.com/qmk/qmk_firmware/blob/master/keyboards/ploopyco/ploopyco.c
//
// Ability to set leds on MacOS is defined in:
// https://www.reddit.com/r/ploopy/comments/ojvhru/led_trick_on_macos_making_nano_versatile_as/
//
// - scroll: setleds -name "Trackball*" ^scroll
// - num:    setleds -name "Trackball*" ^num
// - caps:   setleds -name "Trackball*" ^caps
//
// Setting all three puts the keyboard in boot loader state:
//   setleds -name "Trackball*" -v +num +scroll +caps
//
// Setleds is blocked by MacOS sandbox, the program that calls it needs to be allowed at
// Security & Privacy | Input control. Use MacOS console to see the TCC error messages if
// doesn't work.
// 
bool led_update_user(led_t led_state) {
    // when scroll lock is pressed, toggle drag scroll state
    if ( scroll_lock_state != led_state.scroll_lock ) {
        toggle_drag_scroll();
        scroll_lock_state = led_state.scroll_lock;
    }

    // when num lock is pressed, cycle dpi
    if ( num_lock_state != led_state.num_lock ) {
        cycle_dpi();
        num_lock_state = led_state.num_lock;
    }

    // when all three are enabled, go to bootloader
    if ( led_state.num_lock && led_state.caps_lock && led_state.scroll_lock ) {
        reset_keyboard();
    }

    return true;
}
