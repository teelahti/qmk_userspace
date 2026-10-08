#!/bin/sh
# Append the keyboard's tap-hold timing stream to a per-day log until Ctrl-C.
# `qmk console` waits for the keyboard and reconnects after KVM switches.
set -eu
dir="${TAP_STATS_DIR:-$HOME/.local/share/tap_stats}"
mkdir -p "$dir"
echo "Recording to $dir (Ctrl-C to stop)"
# qmk is Python: unbuffered, or piped lines sit in its buffer and die with it.
PYTHONUNBUFFERED=1 qmk console | grep --line-buffered 'TS [prth] ' >> "$dir/$(date +%Y-%m-%d).log"
