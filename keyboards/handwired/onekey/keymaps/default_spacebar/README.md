# Spacebar — single-key hand-wired keyboard

A one-key keyboard that sends `KC_SPC` (spacebar), built for starting and
stopping a clock in sports tracking software.

## Hardware

- **Controller**: Seeed Studio XIAO RP2040
- **Switch**: Any MX-compatible keyswitch (spacebar)
- **Diode**: Not needed (direct pin mode)

## Wiring

Connect the switch between **D0** (GP26) and **GND** on the XIAO RP2040.

The firmware uses direct pin mode with an internal pull-up resistor on GP26.
No diode is required.

## Build

```sh
qmk compile -kb handwired/onekey/rp2040 -km default_spacebar
```

This produces `handwired_onekey_rp2040_default_spacebar.uf2` in both the
qmk_firmware and userspace directories.

## Flash

1. Hold the **B** button on the XIAO RP2040 while plugging in the USB cable.
   A drive named `RPI-RP2` will appear.
2. Either drag the `.uf2` file onto the drive, or run:

```sh
qmk flash -kb handwired/onekey/rp2040 -km default_spacebar
```

The board resets automatically after flashing.
