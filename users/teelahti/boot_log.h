#pragma once

// Persistent per-half boot log for diagnosing the Halcyon Kyria's dark slave.
// See boot_log.c.

void boot_log_task(void);
void boot_log_dump(void);

// Call right before mcu_reset() to say why this half is resetting itself.
void boot_log_false_master_reset(void);
