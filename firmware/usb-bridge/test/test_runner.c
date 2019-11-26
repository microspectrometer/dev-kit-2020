/* Vim: use ;mktgc to build and run unit tests with compiler=gcc */
/* ---Unit Test Framework--- */
#include <unity.h>
#include <Mock.h>
/* ---Fake Hardware Dependencies (pins/registers)--- */
#include "fake/BiColorLed-Hardware.h"
#include "fake/Ft1248-Hardware.h"
/* #include "fake/Spi-Hardware.h" */
#include "fake/Pwm-Hardware.h"
#include "fake/Lis-Hardware.h"
/* ---Make Tests Visible To `test_runner.c`--- */
#include "test_Bridge.h" // 
#include "test_Spi.h" // setup and teardown functions

/* ---Test Framework requires runner to define setup/teardown/mock pointers--- */
void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock; // record calls/args to mocked-out libs
/* ---Define an empty setup/teardown for pointing to--- */
void NothingToSetUp(void){}
void NothingToTearDown(void){}


/* ---Turn tests on and off--- */
bool Yep=true, Nope=false;


void unittest_GetBridgeLED(bool run_test) // [x] unit test GetBridgeLED
{if (run_test)
    {
    setUp = SetUp_GetBridgeLED; tearDown = TearDown_GetBridgeLED;
    RUN_TEST(GetBridgeLED_reads_and_ignores_Sensor_reply_to_GetBridgeLED);
    RUN_TEST(GetBridgeLED_receives_led_number);
    RUN_TEST(GetBridgeLED_replies_msg_status_error_if_led_does_not_exist);
    RUN_TEST(GetBridgeLED_sends_no_additional_bytes_if_msg_status_is_error);
    RUN_TEST(GetBridgeLED_replies_msg_status_ok_if_led_number_is_recognized);
    RUN_TEST(GetBridgeLED_sends_led_status_byte_after_sending_msg_status_ok);
    RUN_TEST(GetBridgeLED_replies_led_off_if_led_is_off);
    RUN_TEST(GetBridgeLED_replies_led_green_if_led_is_green);
    RUN_TEST(GetBridgeLED_replies_led_red_if_led_is_red);
    }
}
void unittest_SetBridgeLED(bool run_test) // [x] unit test SetBridgeLED
{if (run_test)
    {
    setUp = SetUp_SetBridgeLED; tearDown = TearDown_SetBridgeLED;
    RUN_TEST(SetBridgeLED_reads_and_ignores_Sensor_reply_to_SetBridgeLED);
    RUN_TEST(SetBridgeLED_reads_two_bytes_of_payload_from_usb_host);
    RUN_TEST(SetBridgeLED_replies_with_one_byte);
    RUN_TEST(SetBridgeLED_replies_msg_status_error_if_led_number_is_not_recognized);
    RUN_TEST(SetBridgeLED_replies_msg_status_ok_if_led_number_is_status_led);
    RUN_TEST(SetBridgeLED_turns_off_led_if_payload_is_led_off);
    RUN_TEST(SetBridgeLED_turns_led_on_and_green_if_payload_is_led_green);
    RUN_TEST(SetBridgeLED_turns_led_on_and_red_if_payload_is_led_red);
    }
}
void unittest_BridgeGetSensorLED(bool run_test) // [x] unit test BridgeGetSensorLED
{
    if (run_test)
    {
        setUp = SetUp_BridgeGetSensorLED; tearDown = TearDown_BridgeGetSensorLED;
        RUN_TEST(BridgeGetSensorLED_reads_one_byte_of_host_payload);
        RUN_TEST(BridgeGetSensorLED_checks_for_invalid_command_error_from_Sensor);
        RUN_TEST(BridgeGetSensorLED_does_not_send_payload_if_Sensor_says_invalid_cmd);
        RUN_TEST(BridgeGetSensorLED_passes_invalid_cmd_reply_back_to_host);
        RUN_TEST(BridgeGetSensorLED_responds_ok_if_Sensor_does_not_say_invalid_cmd);
        RUN_TEST(BridgeGetSensorLED_writes_led_number_to_Sensor);
        RUN_TEST(BridgeGetSensorLED_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host);
        RUN_TEST(BridgeGetSensorLED_reads_and_sends_led_status_byte_if_Sensor_status_is_ok);
        RUN_TEST(BridgeGetSensorLED_reads_no_more_bytes_if_Sensor_status_is_error);
    }
}
void unittest_BridgeSetSensorLED(bool run_test) // [ ] unit test BridgeSetSensorLED
{
    if (run_test)
    {
        setUp = SetUp_BridgeSetSensorLED; tearDown = TearDown_BridgeSetSensorLED;
        RUN_TEST(BridgeSetSensorLED_reads_two_bytes_of_host_payload);
        RUN_TEST(BridgeSetSensorLED_checks_for_invalid_command_error_from_Sensor);
        RUN_TEST(BridgeSetSensorLED_does_not_send_payload_if_Sensor_says_invalid_cmd);
        RUN_TEST(BridgeSetSensorLED_passes_invalid_cmd_reply_back_to_host);
        RUN_TEST(BridgeSetSensorLED_responds_ok_if_Sensor_does_not_say_invalid_cmd);
        RUN_TEST(BridgeSetSensorLED_passes_two_bytes_of_payload_to_Sensor);
        RUN_TEST(BridgeSetSensorLED_reads_and_sends_one_byte_Sensor_reply_to_host);
    }
}
void unittest_BridgeGetSensorConfig(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_BridgeGetSensorConfig; tearDown = TearDown_BridgeGetSensorConfig;
        RUN_TEST(BridgeGetSensorConfig_replies_ok_to_USB_host);
        RUN_TEST(BridgeGetSensorConfig_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host);
        RUN_TEST(BridgeGetSensorConfig_reads_no_more_bytes_if_Sensor_status_is_error);
        RUN_TEST(BridgeGetSensorConfig_if_status_is_ok_then_read_config_data_from_Sensor_and_send_to_USB_host);
    }
}
void unittest_BridgeSetSensorConfig(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_BridgeSetSensorConfig; tearDown = TearDown_BridgeSetSensorConfig;
        RUN_TEST(BridgeSetSensorConfig_reads_three_bytes_of_host_payload);
        RUN_TEST(BridgeSetSensorConfig_sends_msg_status_ok_after_reading_payload);
        RUN_TEST(BridgeSetSensorConfig_checks_for_invalid_command_error_from_Sensor);
        RUN_TEST(BridgeSetSensorConfig_does_not_send_payload_if_Sensor_says_invalid_cmd);
        RUN_TEST(BridgeSetSensorConfig_passes_invalid_cmd_reply_back_to_host);
        RUN_TEST(BridgeSetSensorConfig_passes_three_bytes_of_payload_to_Sensor_if_it_does_not_say_invalid_cmd);
        RUN_TEST(BridgeSetSensorConfig_reads_and_sends_one_byte_Sensor_reply_to_host);
    }
}
void unittest_BridgeGetExposure(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_BridgeGetExposure; tearDown = TearDown_BridgeGetExposure;
        RUN_TEST(BridgeGetExposure_replies_ok_to_USB_host);
        RUN_TEST(BridgeGetExposure_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host);
        RUN_TEST(BridgeGetExposure_reads_no_more_bytes_if_Sensor_status_is_error);
        RUN_TEST(BridgeGetExposure_reads_two_bytes_of_exposure_time_from_Sensor_MSB_first_and_sends_to_USB_host);
    }
}
void unittest_BridgeSetExposure(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_BridgeSetExposure; tearDown = TearDown_BridgeSetExposure;
        RUN_TEST(BridgeSetExposure_reads_two_bytes_of_payload_from_usb_host);
        RUN_TEST(BridgeSetExposure_sends_msg_status_ok_to_usb_host);
        RUN_TEST(BridgeSetExposure_checks_for_invalid_command_error_from_Sensor);
        RUN_TEST(BridgeSetExposure_does_not_send_payload_if_Sensor_says_invalid_cmd);
        RUN_TEST(BridgeSetExposure_passes_invalid_cmd_reply_back_to_host);
        RUN_TEST(BridgeSetExposure_passes_two_bytes_of_payload_to_Sensor_if_it_does_not_say_invalid_cmd);
        RUN_TEST(BridgeSetExposure_reads_and_sends_one_byte_Sensor_reply_to_host);
    }
}
void unittest_BridgeCaptureFrame(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_BridgeCaptureFrame; tearDown = TearDown_BridgeCaptureFrame;
        RUN_TEST(BridgeCaptureFrame_sends_msg_status_ok_to_usb_host);
        RUN_TEST(BridgeCaptureFrame_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host);
        RUN_TEST(BridgeCaptureFrame_reads_no_more_bytes_if_Sensor_status_is_error);
        RUN_TEST(BridgeCaptureFrame_reads_npixels_in_frame_and_sends_to_USB_host);
        RUN_TEST(BridgeCaptureFrame_reads_another_status_byte_from_Sensor_and_sends_to_USB_host);
        RUN_TEST(BridgeCaptureFrame_does_not_read_frame_if_Sensor_status_is_error);
        RUN_TEST(BridgeCaptureFrame_reads_and_sends_frame_if_Sensor_status_is_ok);
    }
}
void ApiSupport(bool run_test)
{if (run_test)
    {
    setUp = SetUp_SerialWriteByte_writes_one_byte_over_USB;
    tearDown = TearDown_SerialWriteByte_writes_one_byte_over_USB;
    RUN_TEST(SerialWriteByte_writes_one_byte_over_USB);

    }
}
void BridgeJumpTable(bool run_test)
{if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(LookupBridgeCmd_takes_key_and_returns_fn_ptr);
    RUN_TEST(LookupBridgeCmd_returns_NULL_if_key_is_not_found);
    }
}

int main(void)
{
    UNITY_BEGIN();
    /* ---Plumbing Works For Testing Lib `Bridge`--- */
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    //
    ApiSupport                      (Nope);
    BridgeJumpTable                 (Nope);
    unittest_GetBridgeLED           (Yep);
    unittest_SetBridgeLED           (Nope);
    unittest_BridgeGetSensorLED     (Nope);
    unittest_BridgeSetSensorLED     (Nope);
    unittest_BridgeGetSensorConfig  (Nope);
    unittest_BridgeSetSensorConfig  (Nope);
    unittest_BridgeGetExposure      (Nope);
    unittest_BridgeSetExposure      (Nope);
    unittest_BridgeCaptureFrame     (Nope);
    // Put single tests here (move single tests to test suite later).
    return UNITY_END();
}
