#pragma once

#define PLOOPY_DRAGSCROLL_INVERT
#define PLOOPY_DRAGSCROLL_DIVISOR_H 40.0
#define PLOOPY_DRAGSCROLL_DIVISOR_V 40.0

// Custom (keymap-only) macro: dedicated sensor CPI while drag-scroll is active.
// Core firmware ignores this; keymap.c applies it on scroll toggle. Running a low
// CPI during scroll keeps the speed comfortable while the small divisor above
// removes the initial pickup dead zone (instant first tick).
#define PLOOPY_DRAGSCROLL_DPI 600

#define PLOOPY_DPI_OPTIONS { 1200, 1400, 1800 }
#define PLOOPY_DPI_DEFAULT 1
