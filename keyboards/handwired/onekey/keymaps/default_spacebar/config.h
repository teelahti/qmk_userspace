#pragma once

// Override matrix pins for Seeed XIAO RP2040.
// GP26 = XIAO pin D0.
// GP27 = XIAO pin D1.
#undef MATRIX_COL_PINS
#undef MATRIX_ROW_PINS
#define DIRECT_PINS \
    {               \
        {           \
            GP26    \
        }           \
    }
