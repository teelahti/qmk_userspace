#pragma once

// Included after every config.h, including the ChibiOS board's, which turns
// the RP2040 double-tap reset back on unconditionally (GENERIC_PROMICRO_RP2040
// configs/config.h). A keymap opts out by defining NO_DOUBLE_TAP_RESET.
#ifdef NO_DOUBLE_TAP_RESET
#    undef RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#endif
