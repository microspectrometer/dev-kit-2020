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

/* =====[ LisWriteCfg ]===== */
/* This is an API call for the client to load a configuration into the LIS. */
void LisWriteCfg_example_usage(void)
{
    /** `LisWriteCfg` handles entering and exiting the LIS programming mode.
     *  For each byte in the `cfg` byte stream,
     *  `LisWriteCfg` calls `OutputCfgByte`.
     * */
    /** LisWriteCfg cannot be tested because I do not mock its helpers:
     *  EnterLisProgrammingMode()
     *  OutputCfgByte()
     *  ExitLisProgrammingMode()
     *  I do not mock its helpers because I do not want function pointer seams.
     *  Function pointer seams prevent inlining. Calls compile with `jump` and
     *  `return`.
     * . */
    /* Therefore, this *test* is just a documentation example. */

    /* Fake receiving a valid `cfg`. */
    uint8_t const valid_cfg[] = {0x0F, 0xFF, 0xFF, 0xF9};
    /* =====[ Operate ]===== */
    LisWriteCfg(valid_cfg);
}
void OutputCfgByte_does_nothing_if_arg_nbits_is_0(void)
{
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00; // fake Lis_Rst pin is low
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* Fake a cfg byte that makes Rst high. */
    uint8_t const cfg_byte = 0xFF;
    /* =====[ Operate ]===== */
    uint8_t const bad_nbits = 0;
    OutputCfgByte(cfg_byte, bad_nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1] *if nbits is in bound*. */
    /* But nbits is out of bounds, so Rst is still low from set up. */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void OutputCfgByte_does_nothing_if_arg_nbits_is_more_than_8(void)
{
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00; // fake Lis_Rst pin is low
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* Fake a cfg byte that makes Rst high. */
    uint8_t const cfg_byte = 0xFF;
    /* =====[ Operate ]===== */
    uint8_t const bad_nbits = 9;
    OutputCfgByte(cfg_byte, bad_nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1] *if nbits is in bound*. */
    /* But nbits is out of bounds, so Rst is still low from set up. */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void OutputCfgByte_outputs_cfg_byte_on_Lis_Rst(void)
{
    uint8_t nbits, cfg_byte;
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00;
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* =====[ Operate ]===== */
    /* Fake a cfg byte that makes Rst high on bit[nbits-1]. */
    nbits = 1; cfg_byte = 0x01; // 0b 0000 0001
    OutputCfgByte(cfg_byte, nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1]. */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_port1);
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00;
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* =====[ Operate ]===== */
    /* Fake a cfg byte that makes Rst high on bit[nbits-1]. */
    nbits = 2; cfg_byte = 0x02; // 0b 0000 0010
    OutputCfgByte(cfg_byte, nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1]. */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_port1);
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00;
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* =====[ Operate ]===== */
    /* Fake a cfg byte that makes Rst high on bit[nbits-1]. */
    nbits = 8; cfg_byte = 0x80; // 0b 1000 0000
    OutputCfgByte(cfg_byte, nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[nbits-1]. */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_port1);
    //
    /* Prove the test really is looking at bit index nbits-1. */
    /* =====[ Setup ]===== */
    /* Fake the Lis_Rst pin is starting out low. */
    *Lis_port1 = 0x00;
    /* Check the test is set up correctly */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    /* =====[ Operate ]===== */
    /* Fake a cfg byte that makes Rst high on bit[nbits-1]. */
    nbits = 8; cfg_byte = 0x08; // 0b 0000 1000
    OutputCfgByte(cfg_byte, nbits);
    /* =====[ Test ]===== */
    /* Rst has the value of cfg_byte bit[7]. */
    /* But this test uses a cfg_byte with bit[3] set and bit[7] clear. */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void EnterLisProgrammingMode_outputs_high_on_Lis_PixSelect_pin(void)
{
    /* =====[ Setup ]===== */
    *Lis_port2 = 0x00; // fake port with Lis_PixSelect pin
    /* =====[ Operate ]===== */
    EnterLisProgrammingMode();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_PixSelect, *Lis_port2);
}
void ExitLisProgrammingMode_outputs_low_on_pins_Lis_PixSelect_and_Rst(void)
{
    /* =====[ Setup ]===== */
    *Lis_port1 = 0xFF; // fake port with Lis_Rst pin
    *Lis_port2 = 0xFF; // fake port with Lis_PixSelect pin
    /* =====[ Operate ]===== */
    ExitLisProgrammingMode();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
    TEST_ASSERT_BIT_LOW(Lis_PixSelect, *Lis_port2);
}
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
    // Abandoned. Dev done directly on embedded target.
    // Removed from test runner.
    //
    // Function call/return overhead is too slow to use function calls.
    // If I had more time, I'd develop with function calls then insert macro
    // substitutions, but I do not. The major time sink there is all the
    // stubbing to fake the hardware behavior in flag checking and clearing.
    // Stubbing requires creating functions for those behaviors. And that
    // requires cleverly bypassing all those functions with the macros. I need
    // to move quick, so I am short-circuiting all of that by working directly
    // in macros and manually confirming behavior by measuring on an
    // oscilloscope.
    /* =====[ Setup ]===== */
    Lis_exposure_is_done = true;  // becomes false during exposure
    /* =====[ Operate and Test ]===== */
    uint16_t nticks = 10;     // fake global exposure value set by USB host
    /* bool LisClkTick = false;  // fake global tick flag set by PWM interrupt */
    uint16_t actual_nticks = 0;
    /* LisExpose(nticks); */
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