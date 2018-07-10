#include <stdio.h>          // printf
#include <stdbool.h>        // bool, true, false
#include <unity.h>          // used by all test client code
#include <Mock.h>           // used by all test client code
#include <RecordedCall.h>   // used by *DOF* `mock_lib.c`
#include <RecordedArg.h>    // used by *DOF* `mock_lib.c`
#include "test_ReadWriteBits.h"
#include "test_DebugLed.h"
#include "test_Ft1248.h"
#include "test_Usb.h"

void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock;

bool Yep=true, Nope=false;

void DevelopingReadWriteBits(bool run_test) { if (run_test) {
    setUp       = SetUp_libReadWriteBits;
    tearDown    = TearDown_libReadWriteBits;
    RUN_TEST(SetBit_sets_a_bit);
    RUN_TEST(SetBit_does_not_clear_any_bits);
    RUN_TEST(ClearBit_clears_a_bit);
    RUN_TEST(ClearBit_does_not_set_any_bits);
    RUN_TEST(ToggleBit_toggles_a_bit_low_to_high);
    RUN_TEST(ToggleBit_toggles_a_bit_high_to_low);
    RUN_TEST(BitIsSet_is_true_if_bit_is_set);
    RUN_TEST(BitIsSet_is_false_if_bit_is_clear);
    RUN_TEST(BitIsClear_is_true_if_bit_is_clear);
    RUN_TEST(BitIsClear_is_false_if_bit_is_set);
}}
void DevelopingDebugLed(bool run_test) { if (run_test) {
    setUp = SetUp_DebugLedInit; tearDown = TearDown_DebugLedInit;
    RUN_TEST(DebugLedInit_turns_debug_led_on_and_green);
    RUN_TEST(DebugLedTurnRedToShowError_turns_debug_led_red);
}}
void DevelopingFt1248_lowlevel(bool run_test) { if (run_test) {
    //setUp = SetUp_NothingForFt1248; tearDown = TearDown_NothingForFt1248;
    setUp = SetUp_FtPorts; tearDown = TearDown_FtPorts;
    //
    RUN_TEST(FtHasDataToRead_returns_true_if_MISO_is_low);
    RUN_TEST(FtHasDataToRead_returns_false_if_MISO_is_high);
    RUN_TEST(FtHasRoomToWrite_returns_true_if_MIOSIO_bit_0_is_low);
    RUN_TEST(FtHasRoomToWrite_returns_false_if_MIOSIO_bit_0_is_high);
    //
    RUN_TEST(FtActivateInterface_pulls_SS_low);
    RUN_TEST(FtDeactivateInterface_pulls_SS_high);
    RUN_TEST(FtPushData_pulls_SCK_high);
    RUN_TEST(FtPullData_pulls_SCK_low);
    RUN_TEST(FtLetMasterDriveBus_configures_MIOSIO_port_for_MCU_output);
    RUN_TEST(FtLetSlaveDriveBus_configures_MIOSIO_port_for_MCU_input);
    RUN_TEST(FtOutputByte_outputs_a_byte_on_port_MIOSIO);
    RUN_TEST(FtReadData_returns_the_value_on_MIOSIO_pins);
    RUN_TEST(FtIsBusOk_returns_true_if_MISO_is_low);
    RUN_TEST(FtIsBusOk_returns_false_if_MISO_is_high);
}}
void DevelopingFt1248_highlevel(bool run_test) { if (run_test) {
    setUp = SetUp_FtSendCommand; tearDown = TearDown_FtSendCommand;
    RUN_TEST(FtSendCommand_Read_does_entire_command_phase_for_ReadCmd);
    //
    setUp = SetUp_FtBusTurnaround; tearDown = TearDown_FtBusTurnaround;
    RUN_TEST(FtBusTurnaround_returns_true_if_ok_to_proceed_with_command);
    RUN_TEST(FtBusTurnaround_returns_false_if_not_ok_to_proceed);
    //
    setUp = SetUp_FtRead; tearDown = TearDown_FtRead;
    RUN_TEST(FtRead_does_not_write_to_mem_and_returns_false_if_NAK);
    RUN_TEST(FtRead_should_write_to_mem_and_return_true_if_ACK);
}}
void DevelopingUsb(bool run_test) {if (run_test) {
    setUp = SetUp_NothingForUsb; tearDown = TearDown_NothingForUsb;
    RUN_TEST(UsbRead_should_read_until_buffer_is_empty);
}}
int main()
{
    UNITY_BEGIN();
    DevelopingReadWriteBits   (Nope);
    DevelopingDebugLed        (Nope);
    DevelopingFt1248_lowlevel (Nope);
    DevelopingFt1248_highlevel(Nope);
    DevelopingUsb             (Yep);
    return UNITY_END();
}
