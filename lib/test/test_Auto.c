#include "test_Auto.h"
#include "Auto.h"               // lib under test
#include <unity.h>              // unit testing framework

/* =====[ Context ]===== */
/* - macro `SetExposureTime()` in `mBrd.c` writes to `Lis_nticks_exposure` */
/* - macro `LisExpose()` in `mBrd.c` exposes the Lis */
/* - function `LisFrameReadout()` stores the pixel readout in SRAM */
/* =====[ Takeaway: why this was abandoned ]===== */
/* - Globals make a mess. */
/* - mBrd function to get a frame gets integration time from a global */
/* - integration time is only changed by the host */
/* - this algorithm needs access to change the integration time */
/* - to really use TDD I need to seriously restructure how data is stored */
/* - so table this lib and implement directly in mBrd.c */
/* =====[ List of Tests ]===== */
/* [-] NticsExposureToHitTarget_takes_fnptr_for_get_frame */
/* - this is a function pointer to LisFrameReadout */
/* - no, do not do this. LisFrameReadout stores the frame in a global, it does not return the frame. */
/* - instead, just take a function pointer to ReadPeakCounts() */
/* - let ReadPeakCounts() handle getting the frame */
/* - no I still need to pass the function so I can pass the integration time */
/* [ ] NticsExposureToHitTarget_takes_fnptr_for_read_peak_counts */
/* - this function does not exist yet */
static uint16_t PeakCounts_Stub(void)
{
    // stub getting a frame and calculating peak counts
    return 20000;
}
void test_NticsExposureToHitTarget_returns_target_minus_peak_counts(void)
{ // dummy test: takes target input and fp to peak counts?
    /* =====[ Setup ]===== */
    uint16_t target = 50000;
    uint16_t (*PeakCounts)(void) = PeakCounts_Stub;
    uint16_t dummy_result = target - PeakCounts();
    /* =====[ Operation ]===== */
    uint16_t ntics = NticsExposureToHitTarget(target, PeakCounts);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL(dummy_result, ntics);
}
