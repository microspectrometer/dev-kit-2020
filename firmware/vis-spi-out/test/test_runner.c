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

/* TODO: pull useful stuff from these old tests */
void DevelopingSpiSlave(bool run_test) {if (run_test) {
    setUp = SetUp_SpiSlaveSendBytes; tearDown = TearDown_SpiSlaveSendBytes;
    // These tests require mocking out the calls made by SpiSlaveSendBytes,
    // otherwise they hang waiting for a reply from an imaginary SpiMaster!
    RUN_TEST(LookupSensorCmd_example_calling_the_returned_command);
    RUN_TEST(SpiSlaveWrite_StatusOk_sends_0x00_0x02_0x00_valid_cmd);
    RUN_TEST(SpiSlaveWrite_StatusInvalid_sends_0x00_0x02_0xFF_invalid_cmd_name);
    //
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(LookupSensorCmd_returns_Nth_fn_for_Nth_key);
    RUN_TEST(LookupSensorCmd_returns_NULL_if_key_is_not_in_jump_table);
}}

int main(void)
{
    UNITY_BEGIN();
    DevelopingSpiSlave(Nope);
    /* setUp = NothingToSetUp; tearDown = NothingToTearDown; */
    // Put single tests here (move single tests to test suite later).
    /* setUp = SetUp_GetSensorLED; tearDown = TearDown_GetSensorLED; */
    /* RUN_TEST(GetSensorLED_sends_OK_to_Bridge_to_signal_ready_for_payload); */
    /* RUN_TEST(GetSensorLED_reads_one_byte_of_payload); */
    /* RUN_TEST(GetSensorLED_replies_with_three_bytes_if_led_is_non_existent); */
    /* RUN_TEST(GetSensorLED_replies_msg_status_error_if_led_is_non_existent); */
    /* RUN_TEST(GetSensorLED_replies_with_four_bytes_if_led_number_is_recognized); */
    /* RUN_TEST(GetSensorLED_replies_msg_status_ok_if_led_number_is_recognized); */
    /* RUN_TEST(GetSensorLED_replies_led_off_if_led_is_off); */
    /* RUN_TEST(GetSensorLED_replies_led_green_if_led_is_green); */
    /* RUN_TEST(GetSensorLED_replies_led_red_if_led_is_red); */
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(test_lib_SensorVis_can_use_lib_Queue);
    return UNITY_END();
}
