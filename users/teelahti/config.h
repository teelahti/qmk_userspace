#pragma once
#define EE_HANDS

// default 200
#define TAPPING_TERM 220
#define TAPPING_TERM_PER_KEY

// This used to be TAPPING_FORCE_HOLD, but it changed to more granular
// setting. 0 = never repeat, which is the same than old behavior
#define QUICK_TAP_TERM 0

// This prevents short hold periods to be interpreted as individual taps when typing quickly.
// #define PERMISSIVE_HOLD
// #undef PERMISSIVE_HOLD
// Use per key option to be able to disable permissive hold from home row modifiers
#define PERMISSIVE_HOLD_PER_KEY

// New replacement for Achordion library
#define CHORDAL_HOLD

#define LEADER_PER_KEY_TIMING
#define LEADER_TIMEOUT 350

// Recommended for heavy chording.
#define QMK_KEYS_PER_SCAN 4

