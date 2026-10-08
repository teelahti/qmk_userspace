LEADER_ENABLE = yes
CAPS_WORD_ENABLE = yes

SRC += teelahti.c

# Tap-hold timing stream for tools/tap_stats. Opt in per keymap with
# TAP_STATS_ENABLE = yes; it needs the console, which the AVR boards lack
# flash for.
ifeq ($(strip $(TAP_STATS_ENABLE)), yes)
    SRC += tap_stats.c
    OPT_DEFS += -DTAP_STATS_ENABLE
    CONSOLE_ENABLE = yes
endif
