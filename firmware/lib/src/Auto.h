#ifndef _AUTO_H
#define _AUTO_H

#include <stdint.h>

uint16_t NticsExposureToHitTarget(uint16_t target_peak_counts, uint16_t (*PeakCounts)(void));

#endif // _AUTO_H
