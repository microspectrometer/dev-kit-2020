#include "Auto.h"

uint16_t NticsExposureToHitTarget(uint16_t target_peak_counts, uint16_t (*PeakCounts)(void))
{
    uint16_t peak_counts = PeakCounts();
    return target_peak_counts - peak_counts;
}
