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
#include "test_Spi.h"       // SPI master on simBrd and SPI slave on mBrd
#include "test_UartSpi.h"   // mBrd USART in MSPIM mode for ADC readout
#include "test_Lis.h"       // mBrd I/O to LIS-770i
#include "test_Pwm.h"       // lib `Lis` uses PWM for the clock signal
#include "test_Auto.h"      // lib `Auto` for running auto-exposure on the mBrd

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
    //
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(MacroSetBit_sets_a_bit);
    RUN_TEST(MacroSetBit_does_not_clear_any_bits);
    RUN_TEST(MacroClearBit_clears_a_bit);
    RUN_TEST(MacroClearBit_does_not_set_any_bits);
    RUN_TEST(MacroBitIsSet_is_true_if_bit_is_set);
    RUN_TEST(MacroBitIsSet_is_false_if_bit_is_clear);
    RUN_TEST(MacroBitIsClear_is_true_if_bit_is_clear);
    RUN_TEST(MacroBitIsClear_is_false_if_bit_is_set);
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
    RUN_TEST(DebugLedsToggleAll_toggles_all_the_leds);
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
    RUN_TEST(UsbReadN_reads_N_bytes_and_returns_num_bytes_read);
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
void DevelopingSpiPlumbing(bool run_test) {if (run_test) {
    setUp = SetUp_ClearPendingSpiInterrupt;
    tearDown = TearDown_ClearPendingSpiInterrupt;
    RUN_TEST(ClearPendingSpiInterrupt_reads_SPSR_and_SPDR);
}}
void DevelopingSpiMaster(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(SpiMasterOpenSpi_selects_the_SPI_slave);
    RUN_TEST(SpiMasterCloseSpi_unselects_the_SPI_slave);
    RUN_TEST(SpiTransferIsDone_returns_true_when_the_transfer_is_done);
    RUN_TEST(SpiTransferIsDone_returns_false_when_the_transfer_is_not_done);
    RUN_TEST(SpiResponseIsReady_returns_true_when_slave_signals_data_is_ready);
    RUN_TEST(SpiResponseIsReady_returns_false_when_slave_signals_data_not_ready);
    //
    setUp = SetUp_SpiMasterInit; tearDown = TearDown_SpiMasterInit;
    RUN_TEST(SpiMasterInit_pulls_Ss_high);
    RUN_TEST(SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs);
    RUN_TEST(SpiMasterInit_makes_this_mcu_the_SPI_master);
    RUN_TEST(SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8);
    RUN_TEST(SpiMasterInit_enables_the_SPI_hardware_module);
    RUN_TEST(SpiMasterInit_protects_against_false_SpiResponseIsReady_signals);
    RUN_TEST(SpiMasterInit_clears_pending_SPI_interrupt);
    //
    setUp = SetUp_SpiMasterWaitForResponse;
    tearDown = TearDown_SpiMasterWaitForResponse;
    RUN_TEST(SpiMasterWaitForResponse_waits_until_slave_signals_ready);
    //
    setUp = SetUp_SpiMasterWrite; tearDown = TearDown_SpiMasterWrite;
    RUN_TEST(SpiMasterWrite_byte_loads_SPI_tx_buffer_with_byte);
    RUN_TEST(SpiMasterWrite_byte_waits_for_transmission_to_complete);
    //
    setUp = SetUp_SpiMasterRead; tearDown = TearDown_SpiMasterRead;
    RUN_TEST(SpiMasterRead_returns_the_SPI_data_register);
    RUN_TEST(SpiMasterRead_waits_for_transmission_to_complete);
    RUN_TEST(SpiMasterRead_loads_SPI_tx_buffer_with_slave_ignore);
    //
}}
void DevelopingSpiSlave(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(SpiEnableInterrupt_enables_the_transfer_is_done_interrupt);
    //
    setUp = SetUp_SpiSlaveInit; tearDown = TearDown_SpiSlaveInit;
    RUN_TEST(SpiSlaveInit_configures_pin_Miso_as_an_output);
    RUN_TEST(SpiSlaveInit_makes_Miso_a_pullup_input_when_no_transfer_is_in_progress);
    RUN_TEST(SpiSlaveInit_enables_the_SPI_hardware_module);
    RUN_TEST(SpiSlaveInit_clears_pending_SPI_interrupt);
    //
    setUp = SetUp_SpiSlaveSignalDataIsReady;
    tearDown = TearDown_SpiSlaveSignalDataIsReady;
    RUN_TEST(SpiSlaveSignalDataIsReady_pulls_Miso_low);
    //
    setUp = SetUp_SpiSlaveRead; tearDown = TearDown_SpiSlaveRead;
    RUN_TEST(SpiSlaveRead_waits_until_transfer_is_done);
    RUN_TEST(SpiSlaveRead_returns_the_SPI_data_register_byte);
    //
    setUp = SetUp_SpiSlaveSendBytes; tearDown = TearDown_SpiSlaveSendBytes;
    RUN_TEST(SpiSlaveSendBytes_waits_for_master_read_between_each_byte);
}}
void DevelopingUartSpi(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(UartSpiInit_loads_0_into_baud_rate_register);
    RUN_TEST(UartSpiInit_sets_Sck_as_an_output);
    RUN_TEST(UartSpiInit_sets_AdcConv_to_idle_low);
    RUN_TEST(UartSpiInit_sets_AdcConv_as_an_output);
    RUN_TEST(UartSpiInit_enables_the_UART_in_Master_SPI_Mode);
    RUN_TEST(UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1);
    RUN_TEST(UartSpiInit_cfgs_SPI_to_transfer_MSB_first);
    RUN_TEST(UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior);
    setUp = SetUp_UartSpiRead; tearDown = TearDown_UartSpiRead;
    RUN_TEST(UartSpiTransferIsDone_returns_true_when_the_transfer_is_done);
    RUN_TEST(UartSpiRead_initiates_adc_conversion_and_readout);
    RUN_TEST(UartSpiRead_does_a_16bit_SPI_transfer_with_the_ADC);
    RUN_TEST(UartSpiRead_writes_the_16bit_adc_reading_to_the_input_address);
    // ADC application examples
    RUN_TEST(StoreAdcReadingsInFrame_fills_an_array_with_adc_readings);
}}
void DevelopingWriteCfgToLis(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(LoadNextCfgBit_outputs_bit_0_of_cfg_on_Lis_Rst_pin);
    RUN_TEST(EnterLisProgrammingMode_outputs_high_on_Lis_PixSelect_pin);
    RUN_TEST(ExitLisProgrammingMode_outputs_low_on_pins_Lis_PixSelect_and_Rst);
    //
    setUp = SetUp_LisWriteCfg; tearDown = TearDown_LisWriteCfg;
    RUN_TEST(LisWriteCfg_outputs_cfg_bits_on_Lis_Rst_pin);
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
}}
void DevelopingLis(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(LisInit_sets_PixSelect_as_an_output);
    RUN_TEST(LisInit_sets_Clk_as_an_output);
    RUN_TEST(LisInit_sets_Rst_as_an_output);
    RUN_TEST(LisInit_sets_Sync_as_an_input);
    RUN_TEST(LisInit_idles_PixSelect_low);
    RUN_TEST(LisInit_idles_Rst_low);
    setUp = SetUp_LisInit; tearDown = TearDown_LisInit;
    RUN_TEST(LisInit_configures_pin_Clk_to_generate_the_clock_signal);
    RUN_TEST(LisInit_turns_on_the_clock_signal);
    //
    setUp = SetUp_LisRunClkAt50kHz; tearDown = TearDown_LisRunClkAt50kHz;
    RUN_TEST(LisRunClkAt50kHz_sets_clock_to_50kHz);
    RUN_TEST(LisRunClkAt50kHz_sets_clock_to_50percent_duty_cycle);
    //
    setUp = SetUp_LisClkOn; tearDown = TearDown_LisClkOn;
    RUN_TEST(LisClkOn_outputs_the_clock_signal_on_pin_Clk);
    //
    setUp = SetUp_LisClkOff; tearDown = TearDown_LisClkOff;
    RUN_TEST(LisClkOff_idles_Clk_low);
}}
void DevelopingPwm(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(PwmResetCounterAtTop_configures_PWM_for_fast_PWM_mode);
    RUN_TEST(PwmTopIsOcr0a_uses_the_value_in_OCR0A_for_TOP);
    RUN_TEST(PwmClkIsCpuClk_uses_the_cpu_clock_with_no_prescaler);
    RUN_TEST(PwmEnableOutputSetUntilMatch_sets_OC0B_at_bottom_and_clears_on_match);
    RUN_TEST(PwmDisableOutput_disconnects_OC0B_and_restores_normal_io_output);
    RUN_TEST(WaitForPwmOutputRisingEdge_blocks_until_the_rising_edge_of_pwm_out);
    RUN_TEST(MacroWaitForPwmRisingEdge_blocks_until_the_rising_edge_of_pwm_out);
    RUN_TEST(MacroWaitForPwmRisingEdge_clears_the_interrupt_flag);
    RUN_TEST(MacroWaitForPwmFallingEdge_blocks_until_the_falling_edge_of_pwm_out);
    RUN_TEST(MacroWaitForPwmFallingEdge_clears_the_interrupt_flag);
}}
void DevelopingAuto(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    // Throw-away tests
    RUN_TEST(test_NticsExposureToHitTarget_returns_target_minus_peak_counts);
    RUN_TEST(test_peak_algorithm);
}}
void DevelopingUsbReadOneByte(bool run_test) {if (run_test) {
    setUp = SetUp_UsbRead; tearDown = TearDown_UsbRead;
    RUN_TEST(UsbReadOneByte_copies_the_next_available_byte_to_the_input_read_buffer);
    RUN_TEST(UsbReadOneByte_returns_0_if_there_are_no_bytes_to_read);
    RUN_TEST(UsbReadOneByte_returns_1_if_there_is_at_least_one_byte_to_read);
    RUN_TEST(UsbReadOneByte_example_readings_several_bytes);
}}
void UsbCmdParser_JumpTableSandbox(bool run_test) {if (run_test) {
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    RUN_TEST(LookupCmd_returns_Nth_fn_for_Nth_key);
    RUN_TEST(LookupCmd_returns_NULL_if_key_is_not_in_jump_table);
    RUN_TEST(LookupCmd_example_calling_the_command);
    RUN_TEST(LookupCmd_example_storing_the_returned_pointer);
    RUN_TEST(UsbWriteStatusOk_tells_UsbHost_command_was_success);
    RUN_TEST(UsbWriteStatusInvalid_sends_error_byte_and_echoes_invalid_command);
    RUN_TEST(UsbWriteStatusMismatch_sends_error_byte_and_echoes_invalid_command);
    RUN_TEST(UsbWriteStatusTimedOut_sends_error_byte_and_echoes_invalid_command);
    RUN_TEST(LookupCmd_sad_example_using_UsbWriteStatus_API);
    RUN_TEST(LookupCmd_happy_example_using_UsbWriteStatus_API);
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
    RUN_TEST(CmdCfgLis_returns_StatusMismatch_if_cfg_bytes_are_invalid);
    RUN_TEST(CmdCfgLis_sends_cfg_to_mBrd_and_reads_back_new_cfg_before_reporting_StatusOk);

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
    DevelopingSpiPlumbing     (Nope);
    DevelopingSpiMaster       (Nope);
    DevelopingSpiSlave        (Nope);
    DevelopingUartSpi         (Nope);
    DevelopingLis             (Nope);
    DevelopingPwm             (Nope);
    DevelopingAuto            (Nope); // tabled -- see test
    DevelopingUsbReadOneByte  (Nope);
    DevelopingWriteCfgToLis   (Nope);
    UsbCmdParser_JumpTableSandbox (Yep);
    return UNITY_END();
}
