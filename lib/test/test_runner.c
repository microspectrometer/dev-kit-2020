#include <stdio.h>          // printf
#include <stdbool.h>        // bool, true, false
#include <unity.h>          // used by all test client code
#include <Mock.h>           // used by all test client code
#include <RecordedCall.h>   // used by *DOF* `mock_lib.c`
#include <RecordedArg.h>    // used by *DOF* `mock_lib.c`
#include "test_ReadWriteBits.h"  // low-level bit manipulations
#include "test_DebugLed.h"  // debug LED  on simBrd
#include "test_DebugLeds.h" // debug LEDs on mBrd
#include "test_Ft1248.h"    // lib-level API for FT1248 master on simBrd
#include "test_Usb.h"       // app-level API for FT1248 master on simBrd
#include "test_SpiMaster.h" // SPI on simBrd
#include "test_SpiSlave.h"  // SPI on mBrd

void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock;
void NothingToSetUp(void){}
void NothingToTearDown(void){}

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
void DevelopingDebugLeds(bool run_test) { if (run_test) {
    setUp = SetUp_DebugLeds; tearDown = TearDown_DebugLeds;
    RUN_TEST(DebugLedsTurnOn_led_N_turns_on_led_N);
    RUN_TEST(DebugLedsTurnRed_led_N_turns_led_N_red);
    RUN_TEST(DebugLedsTurnGreen_led_N_turns_led_N_green);
    RUN_TEST(DebugLedsTurnAllRed_turns_all_4_leds_red);
    RUN_TEST(DebugLedsTurnAllGreen_turns_all_4_leds_green);
    RUN_TEST(DebugLedsTurnAllOn_turns_on_all_4_leds);
}}
void DevelopingFt1248_lowlevel(bool run_test) { if (run_test) {
    setUp = SetUp_FtPorts; tearDown = TearDown_FtPorts;
    RUN_TEST(FtSetMisoAsInput_configures_MISO_as_an_input_pin);
    RUN_TEST(FtEnablePullupOnMiso_enables_pullup_on_MISO);
    RUN_TEST(FtSetMiosioAsInputs_configures_MIOSIO_as_an_input_port);
    RUN_TEST(FtEnablePullupsOnMiosio_configures_pullups_on_MIOSIO);
    RUN_TEST(FtSckLow_drives_SCK_low);
    RUN_TEST(FtSsHigh_drives_SS_high);
    //
    setUp = SetUp_FtPorts; tearDown = TearDown_FtPorts;
    RUN_TEST(FtHasDataToRead_returns_true_if_MISO_is_low);
    RUN_TEST(FtHasDataToRead_returns_false_if_MISO_is_high);
    RUN_TEST(FtHasRoomToWrite_returns_true_if_MIOSIO_bit_0_is_low);
    RUN_TEST(FtHasRoomToWrite_returns_false_if_MIOSIO_bit_0_is_high);
    //
    setUp = SetUp_FtPorts; tearDown = TearDown_FtPorts;
    RUN_TEST(FtActivateInterface_pulls_SS_low);
    RUN_TEST(FtDeactivateInterface_pulls_SS_high);
    RUN_TEST(FtPushData_pulls_SCK_high);
    RUN_TEST(FtPullData_pulls_SCK_low);
    RUN_TEST(FtLetMasterDriveBus_configures_MIOSIO_port_for_MCU_output);
    RUN_TEST(FtLetSlaveDriveBus_configures_MIOSIO_port_for_MCU_input);
    RUN_TEST(FtOutputByte_outputs_a_byte_on_port_MIOSIO);
    RUN_TEST(FtReadData_returns_the_value_on_MIOSIO_pins);
    RUN_TEST(FtWriteData_byte_outputs_byte_on_MIOSIO_pins);
    RUN_TEST(FtIsBusOk_returns_true_if_MISO_is_low);
    RUN_TEST(FtIsBusOk_returns_false_if_MISO_is_high);
}}
void DevelopingFt1248_highlevel(bool run_test) { if (run_test) {
    setUp = SetUp_FtSendCommand; tearDown = TearDown_FtSendCommand;
    RUN_TEST(FtSendCommand_Read_does_entire_command_phase_for_ReadCmd);
    RUN_TEST(FtSendCommand_Write_does_entire_command_phase_for_WriteCmd);
    //
    setUp = SetUp_FtBusTurnaround; tearDown = TearDown_FtBusTurnaround;
    RUN_TEST(FtBusTurnaround_returns_true_if_ok_to_proceed_with_command);
    RUN_TEST(FtBusTurnaround_returns_false_if_not_ok_to_proceed);
    //
    setUp = SetUp_FtRead; tearDown = TearDown_FtRead;
    RUN_TEST(FtRead_should_return_false_if_MISO_is_NAK);
    RUN_TEST(FtRead_should_not_write_to_input_buffer_if_MISO_is_NAK);
    RUN_TEST(FtRead_should_return_true_if_MISO_is_ACK);
    RUN_TEST(FtRead_should_write_to_input_buffer_if_MISO_is_ACK);
    //
    setUp = SetUp_DetailsOf_FtRead; tearDown = TearDown_DetailsOf_FtRead;
    RUN_TEST(FtRead_sad_path_is_implemented_like_this);
    RUN_TEST(FtRead_happy_path_is_implemented_like_this);
    //
    setUp = SetUp_FtWrite; tearDown = TearDown_FtWrite;
    RUN_TEST(FtWrite_should_return_false_if_slave_sends_NAK);
    RUN_TEST(FtWrite_should_return_true_if_slave_sends_ACK);
    //
    setUp = SetUp_DetailsOf_FtWrite; tearDown = TearDown_DetailsOf_FtWrite;
    RUN_TEST(FtWrite_implements_the_happy_path_like_this);
}}
void DevelopingUsb(bool run_test) {if (run_test) {
    setUp = SetUp_UsbHasDataToRead; tearDown = TearDown_UsbHasDataToRead;
    RUN_TEST(UsbHasDataToRead_returns_true_if_the_rx_buffer_has_data);
    RUN_TEST(UsbHasDataToRead_returns_false_if_the_rx_buffer_is_empty);
    //
    setUp = SetUp_UsbHasRoomToWrite; tearDown = TearDown_UsbHasRoomToWrite;
    RUN_TEST(UsbHasRoomToWrite_returns_true_if_the_tx_buffer_is_not_full);
    RUN_TEST(UsbHasRoomToWrite_returns_false_if_the_tx_buffer_is_full);
    //
    // UsbRead behavioral tests
    setUp = SetUp_UsbRead; tearDown = TearDown_UsbRead;
    RUN_TEST(UsbRead_returns_0_if_there_was_no_data_to_read);
    RUN_TEST(UsbRead_returns_N_if_there_were_N_bytes_to_read);
    RUN_TEST(UsbRead_turns_LED_red_if_there_was_no_data_to_read);
    RUN_TEST(UsbRead_copies_bytes_to_the_input_read_buffer);
    //
    // UsbRead implementation descriptions
    setUp = SetUp_DetailsOf_UsbRead; tearDown = TearDown_DetailsOf_UsbRead;
    RUN_TEST(UsbRead_sad_path_is_implemented_like_this);
    RUN_TEST(UsbRead_happy_path_is_implemented_like_this);
    //
    // UsbWrite behavioral tests
    setUp = SetUp_UsbWrite; tearDown = TearDown_UsbWrite;
    RUN_TEST(UsbWrite_returns_the_number_of_bytes_sent);
    RUN_TEST(UsbWrite_calls_FtWrite_for_each_byte_to_send);
    RUN_TEST(UsbWrite_stops_sending_bytes_if_the_tx_buffer_is_full);
    RUN_TEST(UsbWrite_turns_LED_red_if_the_tx_buffer_was_already_full);
    RUN_TEST(UsbWrite_returns_0_if_the_tx_buffer_was_already_full);
    //
    // UsbWrite implementation descriptions
    setUp = SetUp_DetailsOf_UsbWrite; tearDown = TearDown_DetailsOf_UsbWrite;
    RUN_TEST(UsbWrite_happy_path_is_implemented_like_this);
    RUN_TEST(UsbWrite_sad_path_is_implemented_like_this);
}}
void DevelopingSpiMaster(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(SpiMasterOpenSpi_selects_the_SPI_slave);
    RUN_TEST(SpiMasterCloseSpi_unselects_the_SPI_slave);
    RUN_TEST(SpiMasterInit_pulls_Ss_high);
    RUN_TEST(SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs);
    RUN_TEST(SpiMasterInit_makes_this_mcu_the_SPI_master);
    RUN_TEST(SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8);
    RUN_TEST(SpiMasterInit_enables_the_SPI_hardware_module);
    RUN_TEST(SpiMasterWrite_byte_loads_SPI_tx_buffer_with_byte);
    //
    setUp = Setup_SpiMasterWrite; tearDown = TearDown_SpiMasterWrite;
    RUN_TEST(SpiMasterWrite_byte_waits_for_transmission_to_complete);
}}
void DevelopingSpiSlave(bool run_test) {if (run_test) {
}}
int main()
{
    UNITY_BEGIN();
    DevelopingReadWriteBits   (Nope);
    DevelopingDebugLed        (Nope);
    DevelopingDebugLeds       (Nope);
    DevelopingFt1248_lowlevel (Nope);
    DevelopingFt1248_highlevel(Nope);
    DevelopingUsb             (Nope);
    DevelopingSpiMaster       (Nope);
    DevelopingSpiSlave        (Yep);
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(SpiSlaveInit_configures_pin_Miso_as_an_output);
    return UNITY_END();
}
