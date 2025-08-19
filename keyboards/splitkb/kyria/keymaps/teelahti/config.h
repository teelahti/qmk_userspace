#pragma once

#ifdef OLED_ENABLE
#    define OLED_DISPLAY_128X64
#endif

// On Liatris microcontroller use the power led as caps lock indicator instead of
// having it shining bright all the time.
// See https://docs.splitkb.com/hc/en-us/articles/5799711553820-Power-LED
// Warning: these will break Elite-C based Kyria
#define LED_CAPS_LOCK_PIN 24
#define LED_PIN_ON_STATE 0
