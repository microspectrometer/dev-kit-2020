#include "test_Lis.h"
#include "Lis.h"                // lib under test
#include "fake/Lis-Hardware.h"  // fake hardware dependencies in Lis.h
#include <unity.h>              // unit testing framework
/* =====[ List of tests ]===== */
    // [x] LisInit_sets_PixSelect_as_an_output
    // [x] LisInit_sets_Clk_as_an_output
    // [x] LisInit_sets_Rst_as_an_output
    // [ ] LisInit_sets_Sync_as_an_input
    //
    // Find notes on how to talk to the Lis770i
    // Clk uses PWM

void LisInit_sets_PixSelect_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr2 = 0x00;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_PixSelect, *Lis_ddr2);
}
void LisInit_sets_Clk_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr1 = 0x00;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_Clk, *Lis_ddr1);
}
void LisInit_sets_Rst_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr1 = 0x00;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_ddr1);
}
