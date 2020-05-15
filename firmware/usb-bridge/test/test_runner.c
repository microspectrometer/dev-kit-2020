// Vim: use ;mktgc to build and run unit tests with compiler=gcc
// ---Unit Test Framework---
#include <unity.h>
#include <Mock.h>
// Python-to-Firmware communication status codes
#include "StatusCodes.h"

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
#include "test_UsbCmd.h"

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

/* =====[ ReadLedState ]===== */
void ReadLedState_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(ReadLedState_returns_OFF_if_LED_is_off);
        RUN_TEST(ReadLedState_returns_GREEN_if_LED_is_on_and_green);
        RUN_TEST(ReadLedState_returns_RED_if_LED_is_on_and_red);
    }
}

/* =====[ GetBridgeLED ]===== */
void GetBridgeLED_tests(bool run_test)
{
    if (run_test)
    {
        ReadLedState_tests(Nope);
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(GetBridgeLED_waits_for_byte_led_num);
        RUN_TEST(GetBridgeLED_reads_byte_led_num);
        RUN_TEST(GetBridgeLED_writes_two_bytes_to_USB_host);
        RUN_TEST(GetBridgeLED_writes_OK_and_LED_STATE_if_led_num_is_valid);
        RUN_TEST(GetBridgeLED_writes_ERROR_and_pads_second_byte_if_led_num_is_invalid);
    }
}

/* =====[ SetBridgeLED ]===== */
void SetBridgeLED_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SetBridgeLED_waits_for_byte_led_num);
        RUN_TEST(SetBridgeLED_reads_byte_led_num);
        RUN_TEST(SetBridgeLED_waits_for_byte_led_setting);
        RUN_TEST(SetBridgeLED_reads_byte_led_setting);
        RUN_TEST(SetBridgeLED_applies_led_setting_and_writes_OK_if_parameters_are_valid);
        RUN_TEST(SetBridgeLED_writes_ERROR_if_led_num_is_invalid);
        RUN_TEST(SetBridgeLED_writes_ERROR_if_led_setting_is_invalid);
    }
}

/* =====[ GetSensorLED_tests ]===== */
void GetSensorLED_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(GetSensorLED_waits_for_byte_led_num);
        RUN_TEST(GetSensorLED_reads_byte_led_num);
        RUN_TEST(GetSensorLED_sends_command_to_sensor);
        RUN_TEST(GetSensorLED_sends_led_num_to_sensor);
        RUN_TEST(GetSensorLED_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
        RUN_TEST(GetSensorLED_waits_for_sensor_to_signal_STATUS_data_ready);
        RUN_TEST(GetSensorLED_reads_status_from_sensor);
        RUN_TEST(GetSensorLED_waits_for_sensor_to_signal_LED_SETTING_data_ready);
        RUN_TEST(GetSensorLED_reads_led_setting_from_sensor);
        RUN_TEST(GetSensorLED_writes_sensor_status);
        RUN_TEST(GetSensorLED_writes_sensor_led_setting);
    }
}

/* =====[ SetSensorLED_tests ]===== */
void SetSensorLED_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SetSensorLED_waits_for_byte_led_num);
        RUN_TEST(SetSensorLED_reads_byte_led_num);
        RUN_TEST(SetSensorLED_waits_for_byte_led_setting);
        RUN_TEST(SetSensorLED_reads_byte_led_setting);
        RUN_TEST(SetSensorLED_sends_command_to_sensor);
        RUN_TEST(SetSensorLED_sends_led_num_to_sensor);
        RUN_TEST(SetSensorLED_sends_led_setting_to_sensor);
        RUN_TEST(SetSensorLED_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
        RUN_TEST(SetSensorLED_waits_for_sensor_to_signal_STATUS_data_ready);
        RUN_TEST(SetSensorLED_reads_status_from_sensor);
        RUN_TEST(SetSensorLED_writes_sensor_status);
    }
}

/* =====[ GetSensorConfig_tests ]===== */
void GetSensorConfig_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(GetSensorConfig_sends_command_to_sensor);
        RUN_TEST(GetSensorConfig_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
        RUN_TEST(GetSensorConfig_waits_for_sensor_to_signal_STATUS_data_ready);
        RUN_TEST(GetSensorConfig_reads_status_from_sensor);
        RUN_TEST(GetSensorConfig_waits_for_sensor_to_signal_BINNING_data_ready);
        RUN_TEST(GetSensorConfig_reads_binning_from_sensor);
        RUN_TEST(GetSensorConfig_waits_for_sensor_to_signal_GAIN_data_ready);
        RUN_TEST(GetSensorConfig_reads_gain_from_sensor);
        RUN_TEST(GetSensorConfig_waits_for_sensor_to_signal_ROW_BITMAP_data_ready);
        RUN_TEST(GetSensorConfig_reads_row_bitmap_from_sensor);
        RUN_TEST(GetSensorConfig_writes_sensor_status);
        RUN_TEST(GetSensorConfig_writes_sensor_binning);
        RUN_TEST(GetSensorConfig_writes_sensor_gain);
        RUN_TEST(GetSensorConfig_writes_sensor_row_bitmap);
    }
}

/* =====[ SetSensorConfig_tests ]===== */
void SetSensorConfig_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SetSensorConfig_waits_for_byte_binning);
        RUN_TEST(SetSensorConfig_reads_byte_binning);
        RUN_TEST(SetSensorConfig_waits_for_byte_gain);
        RUN_TEST(SetSensorConfig_reads_byte_gain);
        RUN_TEST(SetSensorConfig_waits_for_byte_row_bitmap);
        RUN_TEST(SetSensorConfig_reads_byte_row_bitmap);
        RUN_TEST(SetSensorConfig_sends_command_to_sensor);
        RUN_TEST(SetSensorConfig_sends_binning_to_sensor);
        RUN_TEST(SetSensorConfig_sends_gain_to_sensor);
        RUN_TEST(SetSensorConfig_sends_row_bitmap_to_sensor);
        RUN_TEST(SetSensorConfig_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
        RUN_TEST(SetSensorConfig_waits_for_sensor_to_signal_STATUS_data_ready);
        RUN_TEST(SetSensorConfig_reads_status_from_sensor);
        RUN_TEST(SetSensorConfig_writes_sensor_status);
    }
}

/* =====[ GetExposure_tests ]===== */
void GetExposure_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(GetExposure_sends_command_to_sensor);
        RUN_TEST(GetExposure_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
        RUN_TEST(GetExposure_waits_for_sensor_to_signal_STATUS_data_ready);
        RUN_TEST(GetExposure_reads_status_from_sensor);
        RUN_TEST(GetExposure_waits_for_sensor_to_signal_EXPOSURE_MSB_data_ready);
        RUN_TEST(GetExposure_reads_exposure_MSB_from_sensor);
        RUN_TEST(GetExposure_waits_for_sensor_to_signal_EXPOSURE_LSB_data_ready);
        RUN_TEST(GetExposure_reads_exposure_LSB_from_sensor);
        RUN_TEST(GetExposure_writes_sensor_status);
        RUN_TEST(GetExposure_writes_exposure_MSB);
        RUN_TEST(GetExposure_writes_exposure_LSB);
    }
}

/* =====[ SetExposure_tests ]===== */
void SetExposure_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SetExposure_waits_for_byte_exposure_MSB);
        RUN_TEST(SetExposure_reads_byte_exposure_MSB);
        RUN_TEST(SetExposure_waits_for_byte_exposure_LSB);
        RUN_TEST(SetExposure_reads_byte_exposure_LSB);
        RUN_TEST(SetExposure_sends_command_to_sensor);
        RUN_TEST(SetExposure_sends_exposure_MSB_to_sensor);
        RUN_TEST(SetExposure_sends_exposure_LSB_to_sensor);
        RUN_TEST(SetExposure_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
        RUN_TEST(SetExposure_waits_for_sensor_to_signal_STATUS_data_ready);
        RUN_TEST(SetExposure_reads_status_from_sensor);
        RUN_TEST(SetExposure_writes_sensor_status);
    }
}

void UsbCmd_tests(bool run_test)
{
    if (run_test)
    {
        GetBridgeLED_tests(Nope);
        SetBridgeLED_tests(Nope);
        GetSensorLED_tests(Nope);
        SetSensorLED_tests(Nope);
        GetSensorConfig_tests(Nope);
        SetSensorConfig_tests(Nope);
        GetExposure_tests(Nope);
        SetExposure_tests(Nope);
    }
}

int main(void)
{
    UNITY_BEGIN();
    Example_tests(Nope);
    UsbCmd_tests(Yep);
    return UNITY_END();
}
