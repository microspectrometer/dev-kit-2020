// Vim: use ;mktgc to build and run unit tests with compiler=gcc
// ---Unit Test Framework---
#include <unity.h>
#include <Mock.h>
// Python-to-Firmware communication status codes
#include "StatusCodes.h"
// LIS-770i configuration
#include "LisConfigs.h"

/* ---Global Queue--- */
#include "Queue.h"          // VisCmd accesses the Spi Rx Buffer
// SpiFifo must be global in main application for linking against
// extern declarations in other translation units
volatile Queue_s * SpiFifo;

// ---Test Framework requires runner to define setup/teardown/mock pointers---
void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock; // record calls/args to mocked-out libs
// ---Define an empty setup/teardown for pointing to---
void NothingToSetUp(void){}
void NothingToTearDown(void){}
/* ---Define a setup/teardown for writing call history--- */
void SetUp_Mock(void) { mock = Mock_new(); }
void TearDown_Mock(void) { Mock_destroy(mock); mock = NULL; }

// ---Lists of tests---
#include "test_Example.h"
#include "test_AutoExpose.h"
#include "test_VisCmd.h"

// ---Fake all hardware---
#include "HardwareFake.h"

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;
//
void Example_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(test_Can_build_test_suite);
        RUN_TEST(test_Can_call_function_defined_in_app_lib);
        RUN_TEST(test_Can_call_inline_function_defined_in_app_lib);
    }
}

/* =====[ AutoExpose_tests ]===== */

void Run_GetPeak_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(GetPeak_is_visible);
    }
}
void Run_AutoExpose_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(AutoExpose_is_visible);
    }
}
void AutoExpose_tests(bool run_test)
{
    if (run_test)
    {
        Run_GetPeak_tests(Yep);
        Run_AutoExpose_tests(Yep);
    }
}
/* =====[ VisCmd_tests ]===== */
void Run_ReplyCommandInvalid_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(ReplyCommandInvalid_transmits_one_byte_over_SPI);
        RUN_TEST(ReplyCommandInvalid_sends_byte_INVALID_CMD);
    }
}
void Run_LedNumIsValid_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(LedNumIsValid_returns_TRUE_if_led_num_is_0);
        RUN_TEST(LedNumIsValid_returns_TRUE_if_led_num_is_1);
        RUN_TEST(LedNumIsValid_returns_FALSE_if_led_num_is_not_0_or_1);
    }
}
void Run_LisReadout_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(LisReadout_waits_for_Lis_Sync_to_go_HIGH_then_go_LOW);
        RUN_TEST(LisReadout_reads_one_pixel_on_each_rising_edge_of_Lis_Clk);
        RUN_TEST(LisReadout_LOOP_wait_for_the_rising_edge_of_Lis_Clk);
        RUN_TEST(LisReadout_LOOP_start_the_ADC_conversion);
        RUN_TEST(LisReadout_LOOP_wait_for_45_cycles_of_10MHz_clock);
        RUN_TEST(LisReadout_LOOP_start_ADC_readout);
        RUN_TEST(LisReadout_LOOP_wait_for_most_significant_byte_ADC_readout);
        RUN_TEST(LisReadout_LOOP_save_MSB_to_frame_buffer);
        RUN_TEST(LisReadout_LOOP_wait_for_least_significant_byte_ADC_readout);
        RUN_TEST(LisReadout_LOOP_save_LSB_to_frame_buffer);
    }
}

void Run_GetSensorLED_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(GetSensorLED_waits_for_byte_led_num);
        RUN_TEST(GetSensorLED_reads_byte_led_num);
        RUN_TEST(GetSensorLED_sends_OK_and_LED_SETTING_if_led_num_is_valid);
        RUN_TEST(GetSensorLED_sends_ERROR_and_pads_second_byte_if_led_num_is_invalid);
    }
}
void Run_SetSensorLED_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(SetSensorLED_waits_for_byte_led_num);
        RUN_TEST(SetSensorLED_reads_byte_led_num);
        RUN_TEST(SetSensorLED_waits_for_byte_led_setting);
        RUN_TEST(SetSensorLED_reads_byte_led_setting);
        RUN_TEST(SetSensorLED_sends_ERROR_if_led_num_is_invalid);
        RUN_TEST(SetSensorLED_sends_ERROR_if_led_setting_is_invalid);
        RUN_TEST(SetSensorLED_applies_LED_setting_if_valid);
        RUN_TEST(SetSensorLED_sends_OK_if_num_and_setting_are_valid);
    }
}
void Run_GetSensorConfig_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(GetSensorConfig_sends_OK);
        RUN_TEST(GetSensorConfig_sends_binning);
        RUN_TEST(GetSensorConfig_sends_gain);
        RUN_TEST(GetSensorConfig_sends_row_bitmap);
    }
}
void Run_SetSensorConfig_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(SetSensorConfig_receives_three_bytes_of_config_from_Bridge);
        RUN_TEST(SetSensorConfig_does_not_update_config_globals_if_config_is_invalid);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_binning_is_invalid);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_gain_is_invalid);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_active_rows_is_invalid);
        RUN_TEST(SetSensorConfig_does_not_program_LIS_770i_if_config_is_invalid);
        RUN_TEST(SetSensorConfig_writes_valid_config_to_LIS_770i_programmable_setup_register);
        RUN_TEST(SetSensorConfig_replies_OK_if_all_config_values_are_valid);
        RUN_TEST(SetSensorConfig_the_OK_is_sent_after_LIS_is_programmed_with_new_config);
    }
}
void Run_GetExposure_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(GetExposure_sends_OK);
        RUN_TEST(GetExposure_sends_MSB_of_exposure_ticks);
        RUN_TEST(GetExposure_sends_LSB_of_exposure_ticks);
    }
}
void Run_SetExposure_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SetExposure_waits_for_byte_exposure_MSB);
        RUN_TEST(SetExposure_reads_byte_exposure_MSB);
        RUN_TEST(SetExposure_waits_for_byte_exposure_LSB);
        RUN_TEST(SetExposure_reads_byte_exposure_LSB);
        RUN_TEST(SetExposure_updates_global_exposure_ticks);
        RUN_TEST(SetExposure_sends_OK);
    }
}
void Run_CaptureFrame_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(CaptureFrame_sends_OK);
        RUN_TEST(CaptureFrame_checks_binning_to_determine_number_of_pixels_in_frame);
        RUN_TEST(CaptureFrame_sends_num_pixels_MSB);
        RUN_TEST(CaptureFrame_sends_num_pixels_LSB);
        RUN_TEST(CaptureFrame_exposes_the_pixels);
        RUN_TEST(CaptureFrame_does_readout_of_num_pixels_into_the_frame_buffer);
        RUN_TEST(CaptureFrame_sends_the_pixel_readings_stored_in_the_frame_buffer);
    }
}

void VisCmd_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        Run_ReplyCommandInvalid_tests(Nope);
        Run_LedNumIsValid_tests(Nope);
        Run_LisReadout_tests(Nope);
        Run_GetSensorLED_tests(Nope);
        Run_SetSensorLED_tests(Nope);
        Run_GetSensorConfig_tests(Nope);
        Run_SetSensorConfig_tests(Nope);
        Run_GetExposure_tests(Nope);
        Run_SetExposure_tests(Nope);
        Run_CaptureFrame_tests(Yep);
    }
}

int main(void)
{
    UNITY_BEGIN();
    Example_tests(Nope);
    AutoExpose_tests(Yep);
    VisCmd_tests(Nope);
    return UNITY_END();
}
