#pragma once

#ifdef OLED_ENABLE
  #define OLED_DISPLAY_128X64
#endif

// On Liatris microcontroller use the power led as caps lock indicator instead of 
// having it shining bright all the time. Not tested whether this breaks something
// when using with other microcontrollers.
// See https://docs.splitkb.com/hc/en-us/articles/5799711553820-Power-LED
#define LED_CAPS_LOCK_PIN 24
#define LED_PIN_ON_STATE 0
