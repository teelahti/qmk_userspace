# Halcyon Kyria (rev4), Teelahti keymap

Compile with the following, then copy the uf2 to disk:

    qmk compile -kb splitkb/halcyon/kyria/rev4 -km teelahti

## Slave half not lighting up after a KVM switch

See `config.h` for the reasoning. Short version: the slave's LEDs are driven
purely over the split link and the master re-sends that state every 100ms, so a
*permanently* dark slave means the slave is not running its main loop at all --
it is not an LED bug.

Diagnostics, in order, while the slave is dark:

1. **Do the slave's keys work?** If yes, the split link is fine and this is a
   different (RGB) problem entirely.
2. **Plug the dark slave's own host USB-C port into the Mac.**
   - An `RPI-RP2` volume mounts -> the half is stuck in the RP2040 UF2
     bootloader, i.e. the double-tap-reset window was hit by a brownout.
   - It enumerates as a keyboard -> firmware is alive; master/slave detection or
     the interconnect is at fault.
   - Nothing at all -> the half is not getting power. Hardware.
3. **Bypass the KVM** and plug into the Mac directly for a while. If it never
   fails there, the trigger is the monitor hub's VBUS behaviour.

If diagnosis lands on "slave declared itself master", add `NO_USB_STARTUP_CHECK
= yes` to `rules.mk`. That removes the blocking USB_SUSPENDED loop the slave
wedges in, at the cost of waking the host by pressing a key.
