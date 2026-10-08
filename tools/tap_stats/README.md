# Tap-hold statistics

Collects real typing timings from the keyboard so tapping terms can be tuned
on data instead of feel.

## Pieces

- `users/teelahti/tap_stats.c` streams events over the QMK console. Opt in per
  keymap with `TAP_STATS_ENABLE = yes` in its `rules.mk`; only the Halcyon
  Kyria has it, since it needs the console and the AVR boards lack the flash.
- `record.sh` appends that stream to `~/.local/share/tap_stats/YYYY-MM-DD.log`
  (override with `TAP_STATS_DIR`).
- `analyze.py` turns the logs into tables. Standard-library Python only.

## Use

    tools/tap_stats/record.sh          # leave running while you work
    tools/tap_stats/analyze.py         # any time, reads all collected logs

A few days of normal typing gives usable numbers. `analyze.py example.log`
runs it on a small log produced by the real firmware code in QMK's test
harness, with comments describing each scenario.

## Privacy

Only tap-hold keys (the home row mods and thumb layer keys) and Backspace are
identified. Every other key is logged as its hand alone, plus a slot number
that pairs presses with releases without saying which key it was. Timing is
kept; the text is not. The home row letters themselves (a s d f j k l ö) do
appear in typing order, so a log is not completely anonymous: stop recording
before typing anything sensitive, and keep the logs out of the repo.

Nothing is stored on the keyboard. With no `record.sh` running, the firmware
drops the output; after the first unanswered write (one 100ms hiccup at
most) further writes are discarded instantly.

## Reading the report

- **Settlement per key**: hold share, how long real taps last (p50/p90/p99),
  and how often a tap or a hold was followed by Backspace within about a
  second. A tapping term under a key's tap p99 eats letters; a high
  `bspc|hold` points at accidental holds.
- **Overlap patterns**: what decided each press. `solo` = released before
  the next key; `nested` = next key pressed and released inside it; `rolled`
  = next key still down when it was released. `same`/`opp` hand, and whether
  the next key was itself a tap-hold key (`th`).
- **Model check**: the what-if table replays presses through a model of
  Chordal Hold, per-key Permissive Hold and the same-hand shift stretch. This
  line says how often that model matches what the firmware actually did.
  Presses within `FLOW_TAP_TERM` of the previous key are left out, because
  Flow Tap depends on the previous key's identity, which is not logged.
- **What if**: for each candidate term, how many presses would settle the
  other way, and how many of those were followed by Backspace. A flip that
  was usually corrected is a fix; one that was not is a new misfire.

The analyzer mirrors the firmware's terms in constants at its top. Update
them together with `users/teelahti/config.h` and `teelahti.c`.

The Backspace signal is a proxy: it also catches ordinary typos. Compare
rates between settings rather than reading any one number as "misfires".
