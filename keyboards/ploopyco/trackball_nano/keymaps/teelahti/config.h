#pragma once

// Scroll mode is implemented in keymap.c (pointing_device_task_user), not by the
// core drag-scroll code in keyboards/ploopyco/ploopyco.c. The core divides the raw
// delta and emits the whole integer part, so a fast ball roll sends multi-tick
// reports (v=3, v=5, ...) which the host then accelerates on top -> sudden jumps.
//
// Instead we emit scroll strictly proportional to ball travel, with the fractional
// part carried between reports - no rate limit and no acceleration, just a gain.
//
// Deliberately NOT using POINTING_DEVICE_HIRES_SCROLL_ENABLE. It advertises a
// resolution multiplier so the host can treat each unit as 1/120 of a detent, but
// macOS never writes the required feature report - IOHIDFamily does not implement
// it for generic HID mice (it is a Microsoft-originated mechanism; Windows and
// Linux honour it). Verified on this machine: the device gets the generic
// HIDScrollResolution of 9.0 while the built-in trackpad gets 400.0. With hi-res
// enabled, macOS therefore reads every unit as a full detent and scrolls 120x too
// fast, so the same firmware would behave completely differently per host.
// One whole detent is the finest increment macOS will take from a wheel device.

// Sensor CPI while scrolling. Decoupled from the cursor DPI options so scroll feel
// does not change when cycling DPI. Applied on scroll toggle in keymap.c.
//
// Must be a multiple of 125 and no higher than 1375 - see the PLOOPY_DPI_OPTIONS
// note at the bottom for why. Anything else is silently rounded down (600 actually
// ran at 500). Do not tune speed with this: it is the same knob as GAIN_DEN below
// (speed is CPI/DEN) but quantised to 125-count steps. Keep it fixed, tune DEN.
#define TEE_SCROLL_DPI 625

// THE SPEED KNOB. Detents emitted per sensor count, as NUM/DEN. Fractions are exact
// - the remainder carries to the next report. LOWER DEN = FASTER.
//
// DEN is the ball travel in sensor counts per detent, so speed is:
//     TEE_SCROLL_DPI / DEN  detents per inch of ball travel
// and macOS turns each detent into ~3 lines. At 625 CPI:
//     DEN 200 -> 3.1 detents/in  (~9 lines/in)   slower
//     DEN 125 -> 5.0 detents/in  (~15 lines/in)
//     DEN  80 -> 7.8 detents/in  (~23 lines/in)  faster
#define TEE_SCROLL_GAIN_NUM 1
#define TEE_SCROLL_GAIN_DEN 125

// HOST-SIDE SMOOTHING (macOS): Mac Mouse Fix.
//
// One detent is the finest increment macOS will take from a wheel device, so on a
// bare system this scrolls in ~3 line steps however fine the gain above is. Mac
// Mouse Fix (macmousefix.com) closes that gap - it intercepts the HID events and
// re-posts them as pixel-unit scroll events, the same units a trackpad produces,
// which is the only way to get sub-line scrolling on macOS. That is what turns this
// from stepped into smooth, and it is doing the work no firmware setting can.
//
// Use its "Regular Smoothness" mode. "High Smoothness" adds momentum and bounce,
// which reintroduces the speed-dependent scroll distance that the fixed gain above
// exists to eliminate.
//
// Nothing here depends on it. The firmware still decides how far one detent is
// worth, so DEN stays the speed knob and Mac Mouse Fix only smooths what we emit;
// without it, or on another machine, scrolling degrades to whole-detent steps at
// the same speed rather than breaking.

// Snap scrolling to one axis at a time. Programs that smooth their own scrolling
// tend to go slow or jittery when fed vertical and horizontal wheel input at once,
// which is easy to do with a ball. Comment out to allow free diagonal scrolling.
#define TEE_SCROLL_AXIS_SNAP

// Ball travel (sensor counts) after which the chosen axis is locked in. Below this
// the dominant axis is re-evaluated every report, so there is no dead zone at the
// start of a scroll - only a brief window where the direction can still settle.
#define TEE_SCROLL_AXIS_LOCK_COUNTS 20

// Idle time that releases the axis lock, so the next scroll can pick a new axis.
#define TEE_SCROLL_AXIS_UNLOCK_MS 150

// Natural (inverted) vertical scrolling, matching the old PLOOPY_DRAGSCROLL_INVERT.
#define TEE_SCROLL_INVERT_V

// Cursor DPI options. Every value MUST be a multiple of 125 and no higher than
// 1375, because adns5050_set_cpi() does constrain(cpi / 125, 1, 13):
//
//   - the division is integer, so anything else silently rounds DOWN
//   - the sensor's own resolution table in drivers/sensors/adns5050.h stops at
//     CPI1375 = 0x1b, so the constrain() ceiling of 13 happily writes 1500 and 1625,
//     which are past the end of that table and leave the sensor in an undefined
//     resolution - it does not track anywhere near the requested speed
//
// The previous { 1200, 1400, 1800 } therefore really meant { 1125, 1375, undefined },
// which is why the ball felt wrong after a flash whenever the stored index was 2.
// 1375 is the sensor's hardware maximum; there is no faster setting to be had.
#define PLOOPY_DPI_OPTIONS { 1000, 1250, 1375 }

// Index 2 (1375) keeps the boot speed identical to what { 1200, 1400, 1800 } gave
// at its default index 1, which also resolved to 1375.
#define PLOOPY_DPI_DEFAULT 2
