# Teelahti's Kyria firmware

When flashing remember to check which board version is in use and flash
accordingly. Default is usually the newest and it will flash correctly but not
work. E.g. to flash `rev1` do `qmk flash -kb splitkb/kyria/rev1 -km teelahti` or
`make splitkb/kyria/rev1:teelahti`.

For RP2040 based boards the option `BOOTLOADER = liatris` does not seem to work,
but one needs to use the QMK option `-e CONVERT_TO=liatris` instead.

Currently working flash commands are as follows:

Home:

Enable extra led pins from config.h. Then

```
qmk flash -kb splitkb/kyria/rev3 -km teelahti -e CONVERT_TO=liatris
```

Work:

Disable extra led pins from config.h. Then

```
qmk flash -kb splitkb/kyria/rev1 -km teelahti
```
