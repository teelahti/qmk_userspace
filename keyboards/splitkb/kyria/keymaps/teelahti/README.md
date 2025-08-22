# Teelahti's Kyria firmware

When flashing remember to check which board version is in use and flash
accordingly. Default is usually the newest and it will flash correctly but not
work. E.g. to flash `rev1` do `qmk flash -kb splitkb/kyria/rev1 -km teelahti` or
`make splitkb/kyria/rev1:teelahti`.

For RP2040 based boards the option we need to convert with QMK option `-e CONVERT_TO=liatris`. This could e defined in RULES.mk, too.

Currently working flash commands are as follows:

Home:

```
qmk flash -kb splitkb/kyria/rev3 -km teelahti -e CONVERT_TO=liatris
```

Work (note the revision!):

```
qmk flash -kb splitkb/kyria/rev1 -km teelahti
```
