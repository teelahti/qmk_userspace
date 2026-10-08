#pragma once

#ifdef OLED_ENABLE
#    define OLED_DISPLAY_128X64
#endif

#ifdef CONVERT_TO_LIATRIS
    // On Liatris microcontroller use the power led as caps lock indicator instead of
    // having it shining bright all the time.
    // See https://docs.splitkb.com/hc/en-us/articles/5799711553820-Power-LED
    // Warning: these will break Elite-C based Kyria
#    define LED_CAPS_LOCK_PIN 24
#    define LED_PIN_ON_STATE 0
#endif

// rev3's ATmega32U4 runs at ~99% of flash and LTO is already on at keyboard
// level. No one-shot keys are used anywhere, so drop that machinery.
#define NO_ACTION_ONESHOT
