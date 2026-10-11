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

# Persistent per-half boot log for diagnosing a split half that fails to start.
# RP2040 split boards only; the keymap's config.h must set
# EECONFIG_USER_DATA_SIZE and SPLIT_TRANSACTION_IDS_USER, see boot_log.c.
ifeq ($(strip $(BOOT_LOG_ENABLE)), yes)
    SRC += boot_log.c
    OPT_DEFS += -DBOOT_LOG_ENABLE
    CONSOLE_ENABLE = yes
endif
