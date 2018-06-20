#include <stdio.h>          // printf
#include <stdbool.h>        // bool, true, false
#include <unity.h>          // used by all test client code
#include <Mock.h>           // used by all test client code
#include <RecordedCall.h>   // used by *DOF* `mock_lib.c`
#include <RecordedArg.h>    // used by *DOF* `mock_lib.c`
#include "test_ReadWriteBits.h"
#include "test_DebugLed.h"
#include "test_Ft1248.h"

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
}}
int main()
{
    UNITY_BEGIN();
    DevelopingReadWriteBits (Nope);
    DevelopingDebugLed      (Nope);
    //setUp = SetUp_NothingForFt1248; tearDown = TearDown_NothingForFt1248;

    setUp = SetUp_FtPorts; tearDown = TearDown_FtPorts;
    RUN_TEST(FtActivateInterface_pulls_SS_low);
    RUN_TEST(FtPushData_pulls_SCK_high);
    RUN_TEST(FtLetMasterDriveBus_configures_MIOSIO_port_for_MCU_output);
    RUN_TEST(FtOutputByte_outputs_a_byte_on_port_MIOSIO);
    RUN_TEST(FtPullData_pulls_SCK_low);

    setUp = SetUp_FtSendCommand; tearDown = TearDown_FtSendCommand;
    RUN_TEST(FtSendCommand_Read_does_entire_command_phase_for_ReadCmd);

    setUp = SetUp_FtBusTurnaround; tearDown = TearDown_FtBusTurnaround;
    RUN_TEST(FtBusTurnaround_handles_the_entire_bus_turnaround);

    setUp = SetUp_FtPorts; tearDown = TearDown_FtPorts;
    RUN_TEST(FtLetSlaveDriveBus_configures_MIOSIO_port_for_MCU_input);
    RUN_TEST(FtIsBusOk_returns_true_if_MISO_is_low);
    return UNITY_END();
}
