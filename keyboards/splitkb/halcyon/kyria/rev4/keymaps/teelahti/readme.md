# Halcyon Kyria (rev4), Teelahti keymap

Compile with the following, then copy the uf2 to disk:

    qmk compile -kb splitkb/halcyon/kyria/rev4 -km teelahti

## Slave half not lighting up after a KVM switch

See `config.h` for the reasoning. Short version: the slave's LEDs are driven
purely over the split link and the master re-sends that state every 100ms, so a
*permanently* dark slave means the slave is not running its main loop at all --
it is not an LED bug.

Diagnostics:

1. **Do the slave's keys work?** If yes, the split link is fine and this is a
   different (RGB) problem entirely.
2. **Read the boot log** once the halves work again (however many replugs it
   took). Run `qmk console` and press BLog on the Ctl layer (hold the right
   Enter, press D). Each half prints its last 16 boots; see
   `users/teelahti/boot_log.c`. Plugging in the master powers both halves, so
   every attempt should leave an entry on both sides:
   - Master entries with `link=NEVER` and no matching slave entries -> the
     slave's firmware never ran. That was the double-tap bootloader trap until
     it was turned off (see `config.h`); now it means an early boot hang or a
     power fault.
   - Matching slave entries -> the firmware ran; their role, `vbus` samples,
     reset cause and `link` show where it stopped.
3. **Bypass the KVM** and plug into the Mac directly for a while. If it never
   fails there, the trigger is the monitor hub's VBUS behaviour.

Unplugging the master to test the dark half on its own does not work: it cuts
the slave's power, and whatever state it was stuck in is gone.

A slave that declared itself master now restarts itself within about 100ms
(`reset_if_false_master()` in `keymap.c`), so a half that stays dark is more
likely the bootloader trap or a power fault. Such restarts show up in the
slave's log as `false-master`. If the log instead shows the slave picking the
master role with no restart after it, the self-reset did not fire; the next
lever is `NO_USB_STARTUP_CHECK = yes` in `rules.mk`, which removes the blocking
USB_SUSPENDED loop at the cost of waking the host by pressing a key.
