/* Vim: use ;mktgc to build and run unit tests with compiler=gcc */
/* ---Unit Test Framework--- */
#include <unity.h>
#include <Mock.h>
/* ---Fake Hardware Dependencies (pins/registers)--- */
#include "fake/BiColorLed-Hardware.h"
#include "fake/UartSpi-Hardware.h"
#include "fake/Pwm-Hardware.h"
#include "fake/Lis-Hardware.h"
/* ---Make Tests Visible To `test_runner.c`--- */
#include "test_SensorVis.h" // 
#include "test_Spi.h" // setup and teardown functions

/* ---Test Framework requires runner to define setup/teardown/mock pointers--- */
void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock; // record calls/args to mocked-out libs
/* ---Define an empty setup/teardown for pointing to--- */
void NothingToSetUp(void){}
void NothingToTearDown(void){}

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;

void ExampleTestSuite(bool run_test)
{if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
    }
}
void use_Queue_lib_for_SPI_rx_FIFO_buffer(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(test_lib_SensorVis_can_use_lib_Queue_and_sees_SpiFifo);
        RUN_TEST(test_Queue_lib_examples_of_push_and_pop);
        RUN_TEST(test_Queue_lib_example_of_push_wrap_around);
    }
}

void unittest_GetSensorLED(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_GetSensorLED; tearDown = TearDown_GetSensorLED;
        RUN_TEST(GetSensorLED_receives_led_number);
        RUN_TEST(GetSensorLED_replies_msg_status_error_if_led_does_not_exist);
        RUN_TEST(GetSensorLED_sends_no_additional_bytes_if_msg_status_is_error);
        RUN_TEST(GetSensorLED_replies_msg_status_ok_if_led_number_is_recognized);
        RUN_TEST(GetSensorLED_sends_led_status_byte_after_sending_msg_status_ok);
        RUN_TEST(GetSensorLED_replies_led_off_if_led_is_off);
        RUN_TEST(GetSensorLED_replies_led_green_if_led_is_green);
        RUN_TEST(GetSensorLED_replies_led_red_if_led_is_red);
    }
}
void unittest_SetSensorLED(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_SetSensorLED; tearDown = TearDown_SetSensorLED;
        RUN_TEST(SetSensorLED_receives_led_number_and_led_state_from_Bridge);
        RUN_TEST(SetSensorLED_replies_with_one_byte);
        RUN_TEST(SetSensorLED_replies_msg_status_error_if_led_number_is_not_valid);
        RUN_TEST(SetSensorLED_replies_msg_status_error_if_led_state_is_not_valid);
        RUN_TEST(SetSensorLED_replies_msg_status_ok_if_led_number_and_led_state_are_valid);
        RUN_TEST(SetSensorLED_turns_off_led_if_payload_is_led_off);
        RUN_TEST(SetSensorLED_turns_led_on_and_green_if_payload_is_led_green);
        RUN_TEST(SetSensorLED_turns_led_on_and_red_if_payload_is_led_red);
    }
}
void unittest_GetSensorConfig(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_GetSensorConfig; tearDown = TearDown_GetSensorConfig;
        RUN_TEST(GetSensorConfig_sends_three_bytes_of_data_to_Bridge_after_sending_ok);
    }
}
void unittest_SetSensorConfig(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_SetSensorConfig; tearDown = TearDown_SetSensorConfig;
        RUN_TEST(SetSensorConfig_receives_three_bytes_of_config_from_Bridge);
        RUN_TEST(SetSensorConfig_replies_msg_status_error_if_binning_is_invalid);
        RUN_TEST(SetSensorConfig_replies_msg_status_error_if_gain_is_invalid);
        RUN_TEST(SetSensorConfig_replies_msg_status_error_if_active_rows_is_invalid);
        RUN_TEST(SetSensorConfig_replies_msg_status_ok_if_all_config_bytes_are_valid);
        RUN_TEST(SetSensorConfig_converts_three_data_bytes_to_a_28_bit_config);
    }
}
void unittest_GetExposure(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_GetExposure; tearDown = TearDown_GetExposure;
        RUN_TEST(GetExposure_sends_status_byte_ok);
        RUN_TEST(GetExposure_sends_two_bytes_of_exposure_time_most_significant_byte_first);
    }
}

int main(void)
{
    UNITY_BEGIN();
    use_Queue_lib_for_SPI_rx_FIFO_buffer(Nope); // good
    unittest_GetSensorLED(Nope);
    unittest_SetSensorLED(Nope);
    /* setUp = NothingToSetUp; tearDown = NothingToTearDown; */
    // Put single tests here (move single tests to test suite later).
    unittest_GetSensorConfig(Nope);
    unittest_SetSensorConfig(Nope);
    unittest_GetExposure(Nope);
    return UNITY_END();
}
