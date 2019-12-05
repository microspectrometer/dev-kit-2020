#include "unity.h"
#include "test_BiColorLed.h"
#include "BiColorLed.h"

void BiColorLedOn_sets_bit_in_ddr(void)
{
    /* =====[ Setup ]===== */
    *BiColorLed_ddr = 0xFE;
    bicolorled_num led = 0;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        led,
        *BiColorLed_ddr,
        "Cannot run test: must start with ddr led bit clear!"
        );
    /* =====[ Operate ]===== */
    BiColorLedOn(led);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(led, *BiColorLed_ddr);
}
