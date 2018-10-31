#include "test_Auto.h"
#include "Auto.h"               // lib under test
#include <unity.h>              // unit testing framework

void test_NticsExposureToHitTarget(void)
{
    /* =====[ Setup ]===== */
    uint16_t target = 50000;
    /* =====[ Operation ]===== */
    uint16_t ntics = NticsExposureToHitTarget(target);
    // Test
    TEST_ASSERT_EQUAL(ntics, target);
}
