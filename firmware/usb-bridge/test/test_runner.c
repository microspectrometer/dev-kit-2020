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


void API_GetBridgeLED(bool run_test) // [x] unit test GetBridgeLED
{if (run_test)
    {
    setUp = SetUp_GetBridgeLED; tearDown = TearDown_GetBridgeLED;
    RUN_TEST(GetBridgeLED_reads_one_byte_of_payload);
    RUN_TEST(GetBridgeLED_replies_msg_status_error_if_host_queries_nonexistent_led);
    RUN_TEST(GetBridgeLED_replies_with_one_byte_if_led_number_is_not_recognized);
    RUN_TEST(GetBridgeLED_replies_msg_status_ok_if_host_queries_status_led);
    RUN_TEST(GetBridgeLED_replies_with_two_bytes_if_led_number_is_recognized);
    RUN_TEST(GetBridgeLED_replies_with_msg_status_byte_and_led_status_byte);
    RUN_TEST(GetBridgeLED_replies_led_off_if_status_led_is_off);
    RUN_TEST(GetBridgeLED_replies_led_green_if_status_led_is_green);
    RUN_TEST(GetBridgeLED_replies_led_red_if_status_led_is_red);
    }
}
void API_SetBridgeLED(bool run_test) // [x] unit test SetBridgeLED
{if (run_test)
    {
    setUp = SetUp_SetBridgeLED; tearDown = TearDown_SetBridgeLED;
    RUN_TEST(SetBridgeLED_reads_two_bytes_of_payload);
    RUN_TEST(SetBridgeLED_replies_with_one_byte);
    RUN_TEST(SetBridgeLED_replies_msg_status_ok_if_led_number_is_status_led);
    RUN_TEST(SetBridgeLED_replies_msg_status_error_if_led_number_is_not_recognized);
    RUN_TEST(SetBridgeLED_turns_off_led_if_payload_is_led_off);
    RUN_TEST(SetBridgeLED_turns_led_on_and_green_if_payload_is_led_green);
    RUN_TEST(SetBridgeLED_turns_led_on_and_red_if_payload_is_led_red);
    }
}
void API_GetSensorLED(bool run_test) // [ ] unit test GetSensorLED
{if (run_test)
    {
    }
}
void ApiSupport(bool run_test)
{if (run_test)
    {
    setUp = SetUp_SerialWriteByte_writes_one_byte_over_USB;
    tearDown = TearDown_SerialWriteByte_writes_one_byte_over_USB;
    RUN_TEST(SerialWriteByte_writes_one_byte_over_USB);

    setUp = SetUp_Stub_UsbReadN_with_value_in_read_buffer;
    tearDown = TearDown_Stub_UsbReadN_with_value_in_read_buffer;
    RUN_TEST(Stub_UsbReadN_with_value_in_read_buffer);
    }
}
void BridgeJumpTable(bool run_test)
{if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(LookupBridgeCmd_takes_key_and_returns_fn_ptr);
    RUN_TEST(LookupBridgeCmd_returns_NULL_if_key_is_not_found);
    }
}

void UsbCmdParser_JumpTableSandbox(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(LookupBridgeCmd_example_calling_the_command);
    RUN_TEST(LookupBridgeCmd_example_storing_the_returned_pointer);
    RUN_TEST(UsbWriteStatusOk_tells_UsbHost_command_was_success);
    RUN_TEST(UsbWriteStatusInvalid_sends_error_byte_and_echoes_invalid_command);
    RUN_TEST(UsbWriteStatusBadArgs_sends_error_byte_and_echoes_invalid_command);
    RUN_TEST(UsbWriteStatusMissingArgs_sends_error_byte_and_echoes_invalid_command);
    RUN_TEST(UsbWriteStatusSpiBusError_sends_error_byte_and_slave_cmd);

    RUN_TEST(LookupBridgeCmd_sad_example_using_UsbWriteStatus_API);
    RUN_TEST(LookupBridgeCmd_happy_example_using_UsbWriteStatus_API);
    RUN_TEST(CmdCfgLis_returns_StatusOk_and_echoes_back_the_4_cfg_bytes);
    RUN_TEST(CfgTurnsOffAllPixels_returns_true_if_cfg_turns_off_all_pixels);
    RUN_TEST(CfgTurnsOffAllPixels_ignores_the_3LSB_and_4MSB_of_cfg);
    RUN_TEST(CfgTurnsOffAllPixels_returns_false_if_any_pixels_are_on);
    RUN_TEST(XOR_returns_true_if_a_is_true_and_b_is_false);
    RUN_TEST(CfgTurnsRowPartiallyOn_returns_false_if_cfg_turns_on_all_of_row1);
    RUN_TEST(CfgTurnsRowPartiallyOn_returns_true_if_cfg_turns_on_part_of_row1);
    RUN_TEST(CfgTurnsRowPartiallyOn_returns_true_if_cfg_turns_on_part_of_row5);
    RUN_TEST(CfgTurnsRowPartiallyOn_returns_true_if_row_number_is_out_bounds);
    RUN_TEST(CfgBytesAreValid_checks_against_all_255_valid_configs); 
    printf("\n# WIP:\n");
    RUN_TEST(BytesComing_returns_16bit_word_from_struct_spi_NBytesToExpect);
    RUN_TEST(CmdCfgLis_returns_StatusBadArgs_if_cfg_bytes_are_invalid);
    RUN_TEST(CmdCfgLis_1pushes_cfg_to_SpiSlave_2pulls_updated_cfg_3reports_StatusOk_updated_cfg);
}}

void DevelopingInlineSpiMaster(bool run_test) {if (run_test) {
    setUp = SetUp_SpiMasterWrite; tearDown = TearDown_SpiMasterWrite;
    RUN_TEST(SpiMasterWriteN_NoInlineHelpers_sends_N_bytes_to_SpiSlave);
    RUN_TEST(SpiMasterWriteByte_sends_one_byte_to_SpiSlave);
}}

int main(void)
{
    UNITY_BEGIN();
    /* ---Plumbing Works For Testing Lib `Bridge`--- */
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    // Put single tests here (move single tests to test suite later).
    //
    /* ---Test Suites--- */
    /* ---DEPRECATED--- */
    UsbCmdParser_JumpTableSandbox (Nope); // [ ] more functionality to implement
    DevelopingInlineSpiMaster (Nope); // [x] pass
    /* LookupSensorCmd_example_calling_the_returned_command() */
    /* Make this pass by adding code to turn usb-bridge status_led green if
     * status is OK. */
    /* SpiSlaveWrite_StatusOk_sends_0x00_0x02_0x00_valid_cmd:IGNORE: Move test
     * to lib `Sensor`. */
    /* ---ACTIVE--- */
    API_GetBridgeLED (Nope);
    API_SetBridgeLED (Nope);
    API_GetSensorLED (Yep);
    /* API (Yep); */
    ApiSupport (Nope);
    BridgeJumpTable (Nope);
    setUp = SetUp_BridgeGetSensorLED; tearDown = TearDown_BridgeGetSensorLED;
    RUN_TEST(BridgeGetSensorLED_reads_one_byte_of_payload);
    RUN_TEST(BridgeGetSensorLED_responds_ok_after_reading_host_payload);
    RUN_TEST(BridgeGetSensorLED_passes_cmd_to_Sensor_and_waits_for_response);
    RUN_TEST(BridgeGetSensorLED_passes_Sensor_command_response_back_to_host);
    RUN_TEST(BridgeGetSensorLED_exits_if_Sensor_responds_error_to_command);
    RUN_TEST(BridgeGetSensorLED_sends_Sensor_led_number_if_Sensor_responds_ok_to_command);
    RUN_TEST(BridgeGetSensorLED_reads_two_bytes_of_reply_size_and_nbytes_of_reply_from_Sensor);
    RUN_TEST(BridgeGetSensorLED_passes_reply_size_and_reply_to_host);
    return UNITY_END();
}
