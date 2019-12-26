#include "unity.h"
#include "test_Lis.h"
#include "Lis.h"
void LisInit_sets_PixSelect_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr2 = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_PixSelect,
        *Lis_ddr2,
        "PixSelect must be an input pin when the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_FAIL_MESSAGE("Implement test.");
}
