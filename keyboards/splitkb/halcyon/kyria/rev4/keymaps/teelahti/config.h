#pragma once

// ---------------------------------------------------------------------------
// Slave half not starting after a KVM switch
// ---------------------------------------------------------------------------
// The slave's LEDs are driven entirely over the split link (RGB_MATRIX_SPLIT,
// split_count [31,31]), and the master re-sends that state at least every
// FORCED_SYNC_THROTTLE_MS (100ms). So a permanently dark slave is never an LED
// bug -- it means the slave is not running its main loop at all.
//
// Two ways that can happen on this board, both provoked by the flaky VBUS a
// monitor hub / KVM produces:

// 1. rev4/config.h sets RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT to 1000U.
//    That leaves a full second at every boot where the double-tap magic word is
//    live in SRAM; any reset landing in that window -- including a brownout on
//    the slave, whose 5V arrives through the interconnect -- makes the RP2040
//    jump into the UF2 bootloader instead of the firmware. The half then sits
//    there dark until it is power cycled. 200U is the QMK default and shrinks
//    the vulnerable window 5x while still allowing a deliberate double tap.
#undef RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U

// 2. If the slave ever misreads USB_VBUS_PIN as high it declares itself master,
//    never calls transport_slave_init(), and then wedges forever in the
//    USB_SUSPENDED loop in protocol_pre_task(). The software watchdog reboots
//    the slave if the master has not talked to it within the timeout, which
//    recovers that case (and any other wedge) without a replug.
#define SPLIT_WATCHDOG_ENABLE
#define SPLIT_WATCHDOG_TIMEOUT 3000

// KVMs and monitor hubs bounce the USB device state wildly on wakeup. Give
// things time to settle before trusting the matrix again.
#define USB_SUSPEND_WAKEUP_DELAY 200
