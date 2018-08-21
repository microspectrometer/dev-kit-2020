#include "mock_Lis.h"
#include "Pwm.h"
#include <Mock.h>
#include "test_Lis_MockUps.h"
#include "test_Lis.h"
#include "Lis.h"                // lib under test
#include "fake/Lis-Hardware.h"  // fake hardware dependencies in Lis.h
#include <unity.h>              // unit testing framework
#include "mock_Pwm.h"           // mocked version of DOF lib
/* =====[ List of tests ]===== */
// [x] LisInit
    // [x] LisInit_sets_PixSelect_as_an_output
    // [x] LisInit_idles_PixSelect_low
    // [x] LisInit_sets_Clk_as_an_output
    // [x] LisInit_sets_Rst_as_an_output
    // [x] LisInit_idles_Rst_low
        // This will change later when I implement power down mode.
        // In power down, Rst idles high.
    // [x] LisInit_configures_pin_Clk_to_generate_the_clock_signal
    // [x] LisInit_turns_on_the_clock_signal
        // This will change later when I implement power down mode.
        // In power down, Clk idles low.
    // [x] LisInit_sets_Sync_as_an_input
// [x] LisClk
    // [x] LisRunClkAt50kHz_sets_clock_to_50kHz
    // [x] LisRunClkAt50kHz_sets_clock_to_50percent_duty_cycle
// [x] LisClkOn and LisClkOff
    // [x] LisClkOn_outputs_the_clock_signal_on_pin_Clk
    // [x] LisClkOff_idles_Clk_low
// [ ] LisExpose
    // [ ] LisExpose_exposes_pixels_for_nticks_of_LIS_clock

//
/* =====[ LisInit ]===== */
//
void LisInit_sets_PixSelect_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr2 = 0x00;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_PixSelect, *Lis_ddr2);
}
void LisInit_idles_PixSelect_low(void)
{
    /* =====[ Setup ]===== */
    *Lis_port2 = 0xFF;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_PixSelect, *Lis_port2);
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
void LisInit_idles_Rst_low(void)
{
    /* =====[ Setup ]===== */
    *Lis_port1 = 0xFF;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void LisInit_sets_Sync_as_an_input(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr1 = 0xFF;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_Sync, *Lis_ddr1);
}
void SetUp_LisInit(void)
{
    SetUpMock_LisInit();    // create the mock object to record calls
    // other setup code
}
void TearDown_LisInit(void)
{
    TearDownMock_LisInit();    // destroy the mock object
    // other teardown code
}
void LisInit_configures_pin_Clk_to_generate_the_clock_signal(void)
{
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Set expected calls ]===== */
    Expect_LisRunClkAt50kHz();  // testing this one
    Expect_LisClkOn();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void LisInit_turns_on_the_clock_signal(void)
{
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Set expected calls ]===== */
    Expect_LisRunClkAt50kHz();
    Expect_LisClkOn();  // testing this one
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

//
/* =====[ Lis_Clk ]===== */
//
void SetUp_LisRunClkAt50kHz(void)
{
    SetUpMock_LisRunClkAt50kHz();    // create the mock object to record calls
    // other setup code
}
void TearDown_LisRunClkAt50kHz(void)
{
    TearDownMock_LisRunClkAt50kHz();    // destroy the mock object
    // other teardown code
}
void LisRunClkAt50kHz_sets_clock_to_50kHz(void)
{
    /* =====[ Setup ]===== */
    *Lis_clktop = 0;
    int fcpu = 10e6;
    int fclk = 50e3;
    /* =====[ Operate ]===== */
    LisRunClkAt50kHz();
    /* =====[ Test ]===== */
    uint8_t expect_regval = fcpu/fclk;
    TEST_ASSERT_EQUAL_UINT8(expect_regval, *Lis_clktop);
    /* =====[ Set expected calls ]===== */
    Expect_PwmResetCounterAtTop();
    Expect_PwmTopIsOcr0a();
    Expect_PwmClkIsCpuClk();
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void LisRunClkAt50kHz_sets_clock_to_50percent_duty_cycle(void)
{
    /* =====[ Setup ]===== */
    *Lis_clkth = 0;
    int fcpu = 10e6;
    int fclk = 50e3;
    int pwm_ticks = fcpu/fclk;
    /* =====[ Operate ]===== */
    LisRunClkAt50kHz();
    /* =====[ Test ]===== */
    uint8_t expect_regval = pwm_ticks*(50.0/100.0);
    TEST_ASSERT_EQUAL_UINT8(expect_regval, *Lis_clkth);
}

//
/* =====[ LisClkOn and LisClkOff ]===== */
//
void SetUp_LisClkOn(void)
{
    SetUpMock_LisClkOn();    // create the mock object to record calls
    // other setup code
}
void TearDown_LisClkOn(void)
{
    TearDownMock_LisClkOn();    // destroy the mock object
    // other teardown code
}
void LisClkOn_outputs_the_clock_signal_on_pin_Clk(void)
{
    /* =====[ Operate ]===== */
    LisClkOn();
    /* =====[ Set expected calls ]===== */
    Expect_PwmEnableOutputSetUntilMatch(); _MOCK_PWM_H;
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void SetUp_LisClkOff(void)
{
    SetUpMock_LisClkOff();    // create the mock object to record calls
    // other setup code
}
void TearDown_LisClkOff(void)
{
    TearDownMock_LisClkOff();    // destroy the mock object
    // other teardown code
}
void LisClkOff_idles_Clk_low(void)
{
    /* =====[ Setup ]===== */
    *Lis_port1 = 0xFF;
    /* =====[ Operate ]===== */
    LisClkOff();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_Clk, *Lis_port1);
    Expect_PwmDisableOutput();
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

//
/* =====[ LisExpose ]===== */
//
void LisExpose_exposes_pixels_for_nticks_of_LIS_clock(void)
{
    /* =====[ Setup ]===== */
    Lis_exposure_is_done = true;  // becomes false during exposure
    /* =====[ Operate and Test ]===== */
    uint16_t nticks = 10;     // fake global exposure value set by USB host
    /* bool LisClkTick = false;  // fake global tick flag set by PWM interrupt */
    uint16_t actual_nticks = 0;
    LisExpose(nticks);
    // LisExpose calls ExposureStart(): pull LisRst high
    // LisExpose counts nticks of LisClk
    // LisExpose calls ExposureStop(): pulls LisRst low
    while (!Lis_exposure_is_done)
    {
        /* while(!LisClkTick); */
        /* LisClkTick = false; */
        /* actual_nticks++; */
    }
    TEST_ASSERT_EQUAL_UINT16(nticks, actual_nticks);
}
