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
// See comments below how to set the Ploopy nano in reset mode with led controls.
static bool num_lock_state = false;
static bool scroll_lock_state = false;

// Scroll mode state. We deliberately do NOT use the core drag-scroll
// (toggle_drag_scroll / is_drag_scroll in keyboards/ploopyco/ploopyco.c): it emits
// the whole integer part of the accumulated delta, so rolling the ball fast sends
// multi-detent reports that the host accelerates further, making the view jump many
// rows at once. Instead we convert here in pointing_device_task_user() and emit
// high resolution scroll units strictly proportional to ball travel - no rate limit
// and no acceleration, just a fixed gain, like a trackpad.
//
// Note on ordering: pointing_device_task_kb() calls pointing_device_task_user()
// first and only then applies its own drag-scroll conversion. Since we never turn
// is_drag_scroll on, the core leaves the h/v values we produce untouched.
static bool    scroll_mode = false;

// Sub-unit remainders, in count*GAIN_NUM. Carrying these forward is what makes a
// fractional gain exact instead of quantising away slow ball movement.
static int16_t scroll_rem_x = 0;
static int16_t scroll_rem_y = 0;

#ifdef TEE_SCROLL_AXIS_SNAP
typedef enum {
    SCROLL_AXIS_UNSETTLED, // still choosing; dominant axis re-evaluated per report
    SCROLL_AXIS_H,
    SCROLL_AXIS_V,
} scroll_axis_t;

static scroll_axis_t scroll_axis      = SCROLL_AXIS_UNSETTLED;
static int16_t       axis_travel_x    = 0; // |travel| since the axis was last reset
static int16_t       axis_travel_y    = 0;
static uint16_t      last_scroll_move = 0;
#endif

void keyboard_post_init_user(void) {
    num_lock_state  = host_keyboard_led_state().num_lock;
    scroll_lock_state  = host_keyboard_led_state().scroll_lock;
}

static void reset_scroll_state(void) {
    scroll_rem_x = 0;
    scroll_rem_y = 0;
#ifdef TEE_SCROLL_AXIS_SNAP
    scroll_axis   = SCROLL_AXIS_UNSETTLED;
    axis_travel_x = 0;
    axis_travel_y = 0;
#endif
}

// Convert one axis of ball travel into hi-res scroll units, keeping the fractional
// part for the next report.
static int16_t scroll_units(int16_t delta, int16_t *remainder) {
    *remainder += delta * TEE_SCROLL_GAIN_NUM;

    // Truncation toward zero keeps this symmetric for both directions.
    int16_t units = *remainder / TEE_SCROLL_GAIN_DEN;
    *remainder -= units * TEE_SCROLL_GAIN_DEN;
    return units;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (!scroll_mode) {
        return mouse_report;
    }

    // Ball movement drives the wheel instead of the cursor.
    int16_t dx     = mouse_report.x;
    int16_t dy     = mouse_report.y;
    mouse_report.x = 0;
    mouse_report.y = 0;

#ifdef TEE_SCROLL_AXIS_SNAP
    if (timer_elapsed(last_scroll_move) > TEE_SCROLL_AXIS_UNLOCK_MS) {
        scroll_axis   = SCROLL_AXIS_UNSETTLED;
        axis_travel_x = 0;
        axis_travel_y = 0;
    }
    if (dx != 0 || dy != 0) {
        last_scroll_move = timer_read();
    }

    if (scroll_axis == SCROLL_AXIS_UNSETTLED) {
        axis_travel_x += abs(dx);
        axis_travel_y += abs(dy);
    }

    // Below the lock threshold we still suppress the weaker axis, so scrolling is
    // snapped from the very first unit, but the choice can still flip. Past the
    // threshold the axis is committed until the ball goes idle.
    bool vertical = (axis_travel_y >= axis_travel_x);
    if (scroll_axis == SCROLL_AXIS_V) {
        vertical = true;
    } else if (scroll_axis == SCROLL_AXIS_H) {
        vertical = false;
    } else if (axis_travel_x >= TEE_SCROLL_AXIS_LOCK_COUNTS || axis_travel_y >= TEE_SCROLL_AXIS_LOCK_COUNTS) {
        scroll_axis = vertical ? SCROLL_AXIS_V : SCROLL_AXIS_H;
    }

    if (vertical) {
        dx = 0;
    } else {
        dy = 0;
    }
#endif

    int16_t h = scroll_units(dx, &scroll_rem_x);
    int16_t v = scroll_units(dy, &scroll_rem_y);

    mouse_report.h = h;
#ifdef TEE_SCROLL_INVERT_V
    mouse_report.v = -v;
#else
    mouse_report.v = v;
#endif

    return mouse_report;
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
//
// Setleds is blocked by MacOS sandbox, the program that calls it needs to be allowed at
// Security & Privacy | Input control. Use MacOS console to see the TCC error messages if
// doesn't work.
//
// NOTE: Since the day to day terminal (Wezterm) is installed by NixOS and changed often,
// it might be easier to just open system terminal and run the above setleds there. System
// terminal stays the same so the privacy allowance should hold better. 
// 
bool led_update_user(led_t led_state) {
    // when scroll lock is pressed, toggle our own scroll mode
    if ( scroll_lock_state != led_state.scroll_lock ) {
        scroll_mode = led_state.scroll_lock;

        // Start from a clean slate so leftover ball movement can't leak into the
        // first scroll units right after toggling.
        reset_scroll_state();

        // Decouple scroll sensitivity from cursor DPI: run the sensor at a low,
        // dedicated CPI while scrolling, and restore the selected cursor DPI when
        // leaving scroll mode. Normal (non-scroll) pointer behaviour is unchanged.
        if ( scroll_mode ) {
            pointing_device_set_cpi(TEE_SCROLL_DPI);
        } else {
            pointing_device_set_cpi(dpi_array[keyboard_config.dpi_config]);
        }

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
