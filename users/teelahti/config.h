#pragma once
#define EE_HANDS

// default 200
#define TAPPING_TERM 220
#define TAPPING_TERM_PER_KEY

// This used to be TAPPING_FORCE_HOLD, but it changed to more granular
// setting. 0 = never repeat, which is the same than old behavior
#define QUICK_TAP_TERM 0

// Chordal hold prevents same-hand misfires. Permissive hold makes opposite-hand
// chords resolve faster (on key release instead of waiting for tapping term).
// These two are designed to be used together per QMK docs.
#define CHORDAL_HOLD
#define PERMISSIVE_HOLD

// Disable hold behavior during fast typing flow. Reduces input lag for home row
// mods when typing quickly. 150ms is the recommended starting point.
#define FLOW_TAP_TERM 150

#define LEADER_PER_KEY_TIMING
#define LEADER_TIMEOUT 350

// Recommended for heavy chording.
#define QMK_KEYS_PER_SCAN 4
