#!/usr/bin/env python3
"""Turn tap_stats console logs into tapping-term tuning numbers.

Reads the `TS ...` lines streamed by users/teelahti/tap_stats.c (see README.md
next to this file) and reports, per tap-hold key:

  * how presses settled (tap/hold) and how long taps really last,
  * how often a settlement was followed by Backspace (a misfire proxy),
  * what a different tapping term would have changed.

Standard library only. Usage: analyze.py [LOG ...] [--terms 100,120,...]
"""

import argparse
import glob
import os
import re
import sys
from collections import defaultdict
from dataclasses import dataclass
from typing import Optional

PHYS = re.compile(r"TS ([pr]) (\d+) ([LR*]) (\d+) ([0-9A-F]{4})")
RESOLVED = re.compile(r"TS ([th]) (\d+) ([0-9A-F]{4})")

DEFAULT_DIR = os.path.expanduser(os.environ.get("TAP_STATS_DIR", "~/.local/share/tap_stats"))

# Mirror of users/teelahti/config.h and get_tapping_term()/get_permissive_hold()
# in users/teelahti/teelahti.c. Update these when the firmware changes.
TAPPING_TERM = 220
FLOW_TAP_TERM = 120
HOME_F, HOME_J = 0x2209, 0x320D
SHIFTS = {HOME_F, HOME_J}
PERMISSIVE = SHIFTS
NO_FLOW_TAP = SHIFTS  # get_flow_tap_term() returns 0 for these since 2026-10-08
KC_BSPC, KC_ENT, KC_SPC = 0x2A, 0x28, 0x2C


def current_term(kc: int) -> int:
    if kc in SHIFTS:
        return TAPPING_TERM - 100
    if is_layer_tap(kc) and kc & 0xFF in (KC_ENT, KC_SPC):
        return TAPPING_TERM - 80
    return TAPPING_TERM


def stretched_term(kc: int, term: int) -> int:
    """The shift term when a same-hand tap-hold key follows (pre_process_record_user)."""
    return max(term, TAPPING_TERM) if kc in SHIFTS else term


# Keys QMK's default is_flow_tap_key() accepts: a-z, `,` `.` `;`(ö) `/`, space.
FLOW_TAP_BASES = set(range(0x04, 0x1E)) | {0x36, 0x37, 0x33, 0x38, KC_SPC}

HID_NAMES = {0x04 + i: chr(ord("a") + i) for i in range(26)}
HID_NAMES.update({0x33: "ö", 0x34: "ä", KC_SPC: "spc", KC_ENT: "ent", 0x4C: "del", KC_BSPC: "bspc"})
MOD_NAMES = [(0x01, "Ctl"), (0x02, "Sft"), (0x04, "Alt"), (0x08, "Gui")]
LAYER_NAMES = ["QWERTY", "QWERTY_LINUX", "NUM", "SNUM", "NAV", "MEDIA", "CODE", "CODE_LINUX", "CTL"]


def is_mod_tap(kc: int) -> bool:
    return 0x2000 <= kc <= 0x3FFF


def is_layer_tap(kc: int) -> bool:
    return 0x4000 <= kc <= 0x4FFF


def is_tap_hold(kc: int) -> bool:
    return is_mod_tap(kc) or is_layer_tap(kc)


def is_bspc(kc: int) -> bool:
    return kc == KC_BSPC or (is_tap_hold(kc) and kc & 0xFF == KC_BSPC)


def label(kc: int) -> str:
    base = HID_NAMES.get(kc & 0xFF, f"0x{kc & 0xFF:02X}").upper()
    if is_mod_tap(kc):
        mods = (kc >> 8) & 0x1F
        side = "R" if mods & 0x10 else "L"
        return f"{base}/{side}{'+'.join(n for bit, n in MOD_NAMES if mods & bit)}"
    if is_layer_tap(kc):
        layer = (kc >> 8) & 0xF
        return f"{base}/{LAYER_NAMES[layer] if layer < len(LAYER_NAMES) else layer}"
    return base


@dataclass
class Event:
    t: int
    down: bool
    hand: str
    kc: int
    release: Optional[int] = None  # set on presses once the release is seen


def parse(paths):
    """Return physical events in order and {(keycode, press time): 't'|'h'}.

    Firmware times are 16-bit milliseconds; they are unwrapped into one
    increasing timeline. A gap over 65s (or a reboot) only distorts the length
    of that idle gap, which no statistic here depends on.
    """
    events, settled, open_slots = [], {}, {}
    last_raw, now = None, 0
    for path in paths:
        with open(path, encoding="utf-8", errors="replace") as f:
            for line in f:
                m = PHYS.search(line)
                if m:
                    raw = int(m[2])
                    if last_raw is not None:
                        now += (raw - last_raw) & 0xFFFF
                    last_raw = raw
                    ev = Event(now, m[1] == "p", m[3], int(m[5], 16))
                    slot = int(m[4])
                    if ev.down:
                        open_slots[slot] = len(events)
                    elif slot in open_slots:
                        events[open_slots.pop(slot)].release = now
                    events.append(ev)
                    continue
                m = RESOLVED.search(line)
                if m and last_raw is not None:
                    pressed_at = now - ((last_raw - int(m[2])) & 0xFFFF)
                    settled[(int(m[3], 16), pressed_at)] = m[1]
    return events, settled


@dataclass
class Press:
    kc: int
    t: int
    dur: int
    outcome: Optional[str]  # 't', 'h', or None if the settle line was lost
    gap: Optional[int]  # ms since the previous key press
    flow_gap: Optional[int]  # ms since the last event Flow Tap counts
    pattern: str  # 'solo', 'nested' or 'rolled'
    same_hand: bool = False
    next_tap_hold: bool = False
    next_at: int = 0  # ms from this press to the next key press
    next_up: Optional[int] = None  # ms from this press to the next key release
    corrected: bool = False  # Backspace followed shortly after


def flow_tap_times(events, settled):
    """For each event index, the time of the last earlier event Flow Tap counts.

    Despite the docs saying "previous key press", QMK's
    flow_tap_update_last_event() also counts releases, except those of keys
    that were held as a modifier or layer.
    """
    last, out, held = None, [], set()
    for e in events:
        out.append(last)
        if e.down:
            if is_tap_hold(e.kc) and settled.get((e.kc, e.t)) == "h":
                held.add(e.kc)
            last = e.t
        elif e.kc in held:
            held.discard(e.kc)
        else:
            last = e.t
    return out


def build_presses(events, settled, correction_ms=1000, correction_keys=3):
    downs = [i for i, e in enumerate(events) if e.down]
    flow_last = flow_tap_times(events, settled)
    presses = []
    for n, i in enumerate(downs):
        e = events[i]
        if not is_tap_hold(e.kc) or e.release is None:
            continue
        p = Press(e.kc, e.t, e.release - e.t, settled.get((e.kc, e.t)), None, None, "solo")
        if n > 0:
            p.gap = e.t - events[downs[n - 1]].t
        if flow_last[i] is not None:
            p.flow_gap = e.t - flow_last[i]
        if n + 1 < len(downs):
            nxt = events[downs[n + 1]]
            if nxt.t < e.release:
                p.pattern = "nested" if nxt.release is not None and nxt.release <= e.release else "rolled"
                p.same_hand = e.hand != "*" and nxt.hand == e.hand
                p.next_tap_hold = is_tap_hold(nxt.kc)
                p.next_at = nxt.t - e.t
                p.next_up = None if nxt.release is None else nxt.release - e.t
        # Misfire proxy: Backspace within a few keys and correction_ms of release.
        for later in downs[n + 1 : n + 1 + correction_keys + 1]:
            ev = events[later]
            if ev.t - e.release > correction_ms:
                break
            if is_bspc(ev.kc):
                p.corrected = True
                break
        presses.append(p)
    return presses


def in_flow_zone(p: Press) -> bool:
    """Flow Tap may have forced a tap; it depends on the previous key, which the
    log keeps anonymous, so such presses are left out of the term model."""
    if p.kc in NO_FLOW_TAP:
        return False
    return p.flow_gap is not None and p.flow_gap < FLOW_TAP_TERM and p.kc & 0xFF in FLOW_TAP_BASES


def predict(p: Press, term: int) -> Optional[str]:
    """How this press settles with `term`, per Chordal Hold + per-key Permissive Hold."""
    if in_flow_zone(p):
        return None
    if p.pattern == "solo" or p.next_at >= term:
        return "h" if p.dur >= term else "t"
    if p.same_hand:
        if not p.next_tap_hold:
            return "t"  # Chordal Hold settles a same-hand plain key as a tap at once.
        # Two same-hand tap-hold keys: undecided until either is released.
        term = stretched_term(p.kc, term)
        first_up = p.dur if p.next_up is None else min(p.dur, p.next_up)
        return "t" if first_up < term else "h"
    if p.kc in PERMISSIVE and p.pattern == "nested":
        return "h"
    return "h" if p.dur >= term else "t"


def pct(values, q):
    if not values:
        return "-"
    values = sorted(values)
    return str(values[min(len(values) - 1, int(q * len(values)))])


def table(rows, header):
    widths = [max(len(str(r[i])) for r in [header] + rows) for i in range(len(header))]
    line = lambda r: "  ".join(str(c).rjust(w) for c, w in zip(r, widths))
    print(line(header))
    print("  ".join("-" * w for w in widths))
    for r in rows:
        print(line(r))
    print()


def share(n, total):
    return f"{100 * n / total:.0f}%" if total else "-"


def report(presses, terms):
    by_key = defaultdict(list)
    for p in presses:
        by_key[p.kc].append(p)
    keys = sorted(by_key, key=lambda kc: -len(by_key[kc]))

    print("== Settlement per tap-hold key ==")
    print("tap ms = press-to-release of presses that settled as taps.")
    print("bspc% = share followed by Backspace within ~1s (a misfire proxy).\n")
    rows = []
    for kc in keys:
        ps = by_key[kc]
        taps = [p for p in ps if p.outcome == "t"]
        holds = [p for p in ps if p.outcome == "h"]
        tap_ms = [p.dur for p in taps]
        rows.append([
            label(kc), len(ps), share(len(holds), len(ps)), current_term(kc),
            pct(tap_ms, 0.5), pct(tap_ms, 0.9), pct(tap_ms, 0.99),
            share(sum(p.corrected for p in taps), len(taps)),
            share(sum(p.corrected for p in holds), len(holds)),
        ])
    table(rows, ["key", "n", "hold", "term", "tap p50", "p90", "p99", "bspc|tap", "bspc|hold"])

    print("== Overlap patterns (what decided each press) ==")
    rows = []
    for kc in keys:
        groups = defaultdict(list)
        for p in by_key[kc]:
            if p.pattern == "solo":
                groups["solo"].append(p)
            else:
                hand = "same" if p.same_hand else "opp"
                kind = "th" if p.next_tap_hold else "key"
                groups[f"{p.pattern} {hand}-{kind}"].append(p)
        for name in sorted(groups):
            ps = groups[name]
            holds = [p for p in ps if p.outcome == "h"]
            rows.append([
                label(kc), name, len(ps), share(len(holds), len(ps)),
                pct([p.dur for p in ps], 0.5),
                share(sum(p.corrected for p in holds), len(holds)),
                share(sum(p.corrected for p in ps if p.outcome == "t"), len(ps) - len(holds)),
            ])
    table(rows, ["key", "pattern", "n", "hold", "dur p50", "bspc|hold", "bspc|tap"])

    modelled = [p for p in presses if p.outcome and predict(p, current_term(p.kc))]
    agree = sum(predict(p, current_term(p.kc)) == p.outcome for p in modelled)
    flow = sum(in_flow_zone(p) for p in presses)
    print("== Model check ==")
    print(f"{agree}/{len(modelled)} ({share(agree, len(modelled))}) of modelled presses settle as the model predicts")
    print(f"at today's terms; {flow} presses within FLOW_TAP_TERM of the previous key event are left out.")
    print("Below ~95% agreement, treat the what-if numbers with suspicion.\n")

    print("== What if: presses that would settle differently ==")
    print("t>h = would become hold, h>t = would become tap; (bspc) = of those,")
    print("how many were followed by Backspace. Mostly-bspc flips are fixes;")
    print("flips without bspc are likely new misfires.\n")
    rows = []
    for kc in keys:
        ps = [p for p in by_key[kc] if p.outcome and predict(p, current_term(kc))]
        if not ps:
            continue
        row = [label(kc), current_term(kc)]
        for term in terms:
            to_hold = [p for p in ps if p.outcome == "t" and predict(p, term) == "h"]
            to_tap = [p for p in ps if p.outcome == "h" and predict(p, term) == "t"]
            cell = []
            if to_hold:
                cell.append(f"+{len(to_hold)}h({sum(p.corrected for p in to_hold)})")
            if to_tap:
                cell.append(f"+{len(to_tap)}t({sum(p.corrected for p in to_tap)})")
            row.append(" ".join(cell) or ".")
        rows.append(row)
    table(rows, ["key", "now"] + [str(t) for t in terms])


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("logs", nargs="*", help=f"log files (default: {DEFAULT_DIR}/*.log)")
    parser.add_argument("--terms", default="100,120,140,160,180,200,220,250",
                        help="comma-separated tapping terms for the what-if table")
    args = parser.parse_args()

    paths = args.logs or sorted(glob.glob(os.path.join(DEFAULT_DIR, "*.log")))
    if not paths:
        sys.exit(f"No logs given and none found in {DEFAULT_DIR}. Run record.sh first.")
    events, settled = parse(paths)
    presses = build_presses(events, settled)
    if not presses:
        sys.exit("No tap-hold presses found in the logs.")

    span = sum(min(b.t - a.t, 2000) for a, b in zip(events, events[1:]))
    print(f"{sum(e.down for e in events)} key presses, {len(presses)} on tap-hold keys, "
          f"~{span / 60000:.0f} min of active typing, from {len(paths)} log(s).\n")
    report(presses, [int(t) for t in args.terms.split(",")])


if __name__ == "__main__":
    main()
