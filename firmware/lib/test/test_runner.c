// Vim: use ;mktgc to build and run unit tests with compiler=gcc
// ---Unit Test Framework---
#include "unity.h"
#include "Mock.h"
// Python-to-Firmware communication status codes
#include "StatusCodes.h"
// LIS-770i configuration
#include "LisConfigs.h"
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
#include "test_BiColorLed.h"
#include "test_ReadWriteBits.h"
#include "test_Flag.h"
#include "test_SpiSlave.h"
#include "test_Queue.h"
#include "test_UartSpi.h"
#include "test_Lis.h"
// test libs for usb-bridge
#include "test_Usb.h"

// ---Fake all hardware---
#include "HardwareFake.h"

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;

void BiColorLed_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(BiColorLedOn_sets_bit_in_ddr);
        RUN_TEST(BiColorLedGreen_clears_bit_in_port);
        RUN_TEST(BiColorLedRed_sets_bit_in_port);
    }
}

void ReadWriteBits_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SetBit_sets_bit_in_register);
        RUN_TEST(ClearBit_clears_bit_in_register);
        RUN_TEST(BitIsSet_returns_true_if_bit_is_set);
        RUN_TEST(BitIsSet_returns_false_if_bit_is_clear);
        RUN_TEST(BitIsClear_returns_true_if_bit_is_clear);
        RUN_TEST(BitIsClear_returns_false_if_bit_is_set);
    }
}

void Flag_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(test_Flag);
    }
}

/* =====[ SpiSlave_tests ]===== */
void Check_SpiSlave_plumbing_for_fakes(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(SpiSlave_faked_calls_are_still_available_for_testing);
    }
}
void Run__SignalDataReady_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SignalDataReady_drives_DataReady_LOW);
    }
}
void Run_ClearSpiInterruptFlag_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(ClearSpiInterruptFlag_first_reads_SPI_status_register);
        RUN_TEST(ClearSpiInterruptFlag_then_reads_SPI_data_register);
    }
}
void Run__SpiTransferIsDone_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SpiTransferIsDone_returns_true_if_the_SPI_Interrupt_Flag_is_set);
        RUN_TEST(SpiTransferIsDone_returns_false_if_the_SPI_Interrupt_Flag_is_clear);
    }
}
void Run_DisableSpiInterrupt_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(DisableSpiInterrupt_clears_the_SPI_Interrupt_Enable_bit);
    }
}
void Run_EnableSpiInterrupt_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(EnableSpiInterrupt_clears_SPI_interrupt_flag);
        RUN_TEST(EnableSpiInterrupt_enables_SPI_transfer_complete_interrupt);
        RUN_TEST(EnableSpiInterrupt_consumes_6_cycles);
    }
}
void Run_SpiSlaveInit_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(SpiSlaveInit_makes_DataReady_an_output_pin);
        RUN_TEST(SpiSlaveInit_idles_DataReady_high);
        RUN_TEST(SpiSlaveInit_makes_Miso_an_output_pin);
        RUN_TEST(SpiSlaveInit_enables_SPI);
        RUN_TEST(SpiSlaveInit_enables_SPI_interrupt);
    }
}
void Run_SpiSlaveTxByte_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(SpiSlaveTxByte_loads_SPI_data_register_with_input_byte);
        RUN_TEST(SpiSlaveTxByte_disables_SPI_ISR_before_signaling_data_ready);
        RUN_TEST(SpiSlaveTxByte_drives_DataReady_LOW_to_signal_data_is_ready);
        RUN_TEST(SpiSlaveTxByte_waits_until_SPI_transfer_is_done);
        RUN_TEST(SpiSlaveTxByte_drives_DataReady_HIGH_immediately_after_SPI_transfer_finishes);
        RUN_TEST(SpiSlaveTxByte_enables_SPI_ISR_after_transfer);
    }
}
void Run_SpiSlaveTx_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(SpiSlaveTx_sends_nbytes_of_input_buffer_to_SpiMaster);
    }
}
void SpiSlave_tests(bool run_test)
{
    if (run_test)
    {
        Check_SpiSlave_plumbing_for_fakes(Yep);
        Run_ClearSpiInterruptFlag_tests(Yep);
        Run__SpiTransferIsDone_tests(Yep);
        Run_DisableSpiInterrupt_tests(Yep);
        Run_EnableSpiInterrupt_tests(Yep);
        Run_SpiSlaveInit_tests(Yep);
        Run__SignalDataReady_tests(Yep);
        Run_SpiSlaveTxByte_tests(Yep);
        Run_SpiSlaveTx_tests(Yep);
    }
}

/* =====[ Queue_tests ]===== */
void Run_QueueInit_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueueInit_returns_a_pointer_to_a_Queue_struct);
        RUN_TEST(QueueInit_memory_for_Queue_struct_is_allocated_in_Queue_object_file);
        RUN_TEST(QueueInit_assigns_input_buffer_as_Queue_buffer);
        RUN_TEST(QueueInit_size_input_is_the_maximum_Queue_length);
        RUN_TEST(QueueInit_initializes_Queue_with_length_0);
    }
}
void Run_QueueLength_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueueLength_increments_after_a_push);
        RUN_TEST(QueueLength_does_not_increase_beyond_max_length);
        RUN_TEST(QueueLength_decrements_after_a_pop);
        RUN_TEST(QueueLength_does_not_decrease_below_zero);
    }
}
void Run_QueuePush_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueuePush_writes_byte_to_Queue_buffer);
        RUN_TEST(QueuePush_writes_next_byte_to_address_after_previous_write);
        RUN_TEST(QueuePush_does_not_write_byte_if_Queue_is_full);
        RUN_TEST(QueuePush_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_full);
    }
}
void Run_QueuePop_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueuePop_removes_oldest_byte_from_Queue);
        RUN_TEST(QueuePop_returns_oldest_byte);
        RUN_TEST(QueuePop_does_not_remove_any_bytes_if_Queue_is_empty);
        RUN_TEST(QueuePop_returns_0_if_Queue_is_empty);
        RUN_TEST(QueuePop_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_empty);
    }
}
void Run_QueueIsFull_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueueIsFull_returns_true_if_Queue_is_full);
        RUN_TEST(QueueIsFull_returns_false_if_Queue_is_not_full);
    }
}
void Run_QueueIsEmpty_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueueIsEmpty_returns_true_if_Queue_is_empty);
        RUN_TEST(QueueIsEmpty_returns_false_if_Queue_is_not_empty);
    }
}
void Queue_tests(bool run_test)
{
    if (run_test)
    {
        Run_QueueInit_tests(Yep);
        Run_QueueLength_tests(Yep);
        Run_QueuePush_tests(Yep);
        Run_QueuePop_tests(Yep);
        Run_QueueIsFull_tests(Yep);
        Run_QueueIsEmpty_tests(Yep);
    }
}

void UartSpi_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(UartSpiInit_clocks_SPI_bus_at_5MHz);
        RUN_TEST(UartSpiInit_sets_Sck_as_an_output);
        RUN_TEST(UartSpiInit_sets_AdcConv_to_idle_low);
        RUN_TEST(UartSpiInit_sets_AdcConv_as_an_output);
        RUN_TEST(UartSpiInit_enables_the_UART_in_Master_SPI_Mode);
        RUN_TEST(UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1);
        RUN_TEST(UartSpiInit_cfgs_SPI_to_transfer_MSB_first);
        RUN_TEST(UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior);
    }
}

void Run_LisInit_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(LisInit_sets_PixSelect_as_an_output);
        RUN_TEST(LisInit_idles_PixSelect_low);
        RUN_TEST(LisInit_sets_Rst_as_an_output);
        RUN_TEST(LisInit_idles_Rst_low);
        RUN_TEST(LisInit_sets_Sync_as_an_input);
        RUN_TEST(LisInit_sets_Clk_as_an_output);
        RUN_TEST(LisInit_resets_PWM_timer_at_top);
        RUN_TEST(LisInit_PWM_timer_top_is_OCR0A);
        RUN_TEST(LisInit_PWM_timer_is_clocked_by_CPU_with_no_prescaling);
        RUN_TEST(LisInit_sets_PWM_frequency_at_50kHz);
        RUN_TEST(LisInit_sets_PWM_duty_cycle_to_50_percent);
        RUN_TEST(LisInit_outputs_the_PWM_clock_on_pin_Clk);
    }
}
void Run_LisConfigIsValid_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(LisConfigIsValid_returns_false_if_binning_is_invalid);
        RUN_TEST(LisConfigIsValid_returns_false_if_gain_is_invalid);
        RUN_TEST(LisConfigIsValid_returns_false_if_active_rows_is_invalid);
        RUN_TEST(LisConfigIsValid_returns_true_if_config_is_valid);
    }
}
void Run__ConfigAs28bits_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(ConfigAs28bits_writes_config_as_little_endian_ie_binning_is_config_byte0_bit0);
        RUN_TEST(ConfigAs28bits_sets_config_byte0_bit0_if_BINNING_ON);
        RUN_TEST(ConfigAs28bits_clears_config_byte0_bit0_if_BINNING_OFF);
        RUN_TEST(ConfigAs28bits_byte0_bit1_clear_and_bit2_clear_if_GAIN_1X);
        RUN_TEST(ConfigAs28bits_byte0_bit1_clear_and_bit2_set_if_GAIN_2X5);
        RUN_TEST(ConfigAs28bits_byte0_bit1_set_and_bit2_clear_if_GAIN_4X);
        RUN_TEST(ConfigAs28bits_byte0_bit1_set_and_bit2_set_if_GAIN_5X);
        RUN_TEST(ConfigAs28bits_bit3_to_bit27_set_if_ALL_ROWS_ACTIVE);
        RUN_TEST(ConfigAs28bits_b3b8b13b18b23_set_if_ROW_1_ACTIVE);
        RUN_TEST(ConfigAs28bits_b4b9b14b19b24_set_if_ROW_2_ACTIVE);
        RUN_TEST(ConfigAs28bits_b5b10b15b20b25_set_if_ROW_3_ACTIVE);
        RUN_TEST(ConfigAs28bits_b6b11b16b21b26_set_if_ROW_4_ACTIVE);
        RUN_TEST(ConfigAs28bits_b7b12b17b22b27_set_if_ROW_5_ACTIVE);
    }
}
void Run__WaitForLisClkLow_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(WaitForLisClkLow_clears_flag_PwmTimerMatchesOCF0B);
        RUN_TEST(WaitForLisClkLow_waits_until_flag_PwmTimerMatchesOCF0B_is_set);
    }
}
void Run__WaitForLisClkHigh_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(WaitForLisClkHigh_clears_flag_PwmTimerMatchesOCF0A);
        RUN_TEST(WaitForLisClkHigh_waits_until_flag_PwmTimerMatchesOCF0A_is_set);
    }
}
void Run__EnterLisProgrammingMode_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(EnterLisProgrammingMode_waits_for_LisClk_LOW);
        RUN_TEST(EnterLisProgrammingMode_asserts_LisPixSelect_to_program_Lis);
    }
}
void Run__WriteLisConfigBit_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(WriteLisConfigBit_outputs_bit_on_LisRst);
        RUN_TEST(WriteLisConfigBit_waits_for_LisClk_HIGH);
        RUN_TEST(WriteLisConfigBit_waits_for_LisClk_LOW);
    }
}
void Run__Write28bitLisConfig_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(Write28bitLisConfig_writes_28bits_starting_at_byte0_bit0_and_ending_at_byte3_bit3);
    }
}
void Run__ExitLisProgrammingMode_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(ExitLisProgrammingMode_outputs_LOW_on_pin_LisRst);
        RUN_TEST(ExitLisProgrammingMode_outputs_LOW_on_pin_LisPixSelect);
    }
}
void Run_LisWriteConfig_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(LisWriteConfig_converts_config_to_28bit_sequence);
        RUN_TEST(LisWriteConfig_enters_LIS_programming_mode);
        RUN_TEST(LisWriteConfig_writes_28bits_to_LIS_setup_register);
        RUN_TEST(LisWriteConfig_exits_LIS_programming_mode);
    }
}
void Lis_tests(bool run_test)
{
    if (run_test)
    {
        Run_LisInit_tests(Yep);
        Run_LisConfigIsValid_tests(Yep);
        Run__ConfigAs28bits_tests(Yep);
        Run__WaitForLisClkLow_tests(Yep);
        Run__WaitForLisClkHigh_tests(Yep);
        Run__EnterLisProgrammingMode_tests(Yep);
        Run__WriteLisConfigBit_tests(Yep);
        Run__Write28bitLisConfig_tests(Yep);
        Run__ExitLisProgrammingMode_tests(Yep);
        Run_LisWriteConfig_tests(Yep);
    }
}

/* =====[ test libs for usb-bridge ]===== */

/* =====[ Usb_tests ]===== */
// ---Run_Usb_Private_Ft_tests---
void Run__FtClockDatabus_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(FtClockDatabus_drives_FtClock_HIGH_if_direction_is_FtDrive);
        RUN_TEST(FtClockDatabus_drives_FtClock_LOW_if_direction_is_FtSample);
    }
}
void Run__FtReadDatabus_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(FtReadDatabus_copies_databus_pin_values_to_address_pbyte);
    }
}
void Run__FtWriteDatabus_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(FtWriteDatabus_outputs_byte_on_databus_pins);
    }
}
void Run__FtDatabusPinDirection(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(FtDatabusPinDirection_makes_databus_pins_outputs_if_direction_is_FtOut);
        RUN_TEST(FtDatabusPinDirection_makes_databus_pins_inputs_if_direction_is_FtIn);
    }
}
void Run_Usb_Private_Ft_tests(bool run_test)
{
    if (run_test)
    {
        Run__FtClockDatabus_tests(Yep);
        Run__FtReadDatabus_tests(Yep);
        Run__FtWriteDatabus_tests(Yep);
        Run__FtDatabusPinDirection(Yep);
    }
}
// ---Run_Usb_Ft_tests---
void Run_FtSelectFT221X_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(FtSelectFT221X_drives_FtChipSelect_LOW);
    }
}
void Run_FtUnselectFT221X_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(FtUnselectFT221X_drives_FtChipSelect_HIGH);
    }
}
void Run_FtBusTurnaround_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(FtBusTurnaround_clocks_one_cycle_to_signal_data_drive_then_data_sample);
    }
}
void Run_FtIsOk_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(FtIsOk_returns_true_if_FtMiso_is_LOW);
        RUN_TEST(FtIsOk_returns_false_if_FtMiso_is_HIGH);
    }
}
void Run_FtRead_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(FtRead_clocks_one_byte_out_of_the_FT221X);
        RUN_TEST(FtRead_stores_the_byte_at_address_pbyte);
    }
}
void Run_FtWrite_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(FtWrite_signals_to_drive_data_onto_the_databus);
        RUN_TEST(FtWrite_sets_microcontroller_databus_pins_as_outputs);
        RUN_TEST(FtWrite_outputs_byte_on_databus_pins);
        RUN_TEST(FtWrite_signals_FT221X_to_sample_the_databus);
        RUN_TEST(FtWrite_sets_microcontroller_databus_pins_as_inputs);
    }
}
void Run_Usb_Ft_tests(bool run_test)
{
    if (run_test)
    {
        Run_FtSelectFT221X_tests(Yep);
        Run_FtUnselectFT221X_tests(Yep);
        Run_FtBusTurnaround_tests(Yep);
        Run_FtIsOk_tests(Yep);
        Run_FtRead_tests(Yep);
        Run_FtWrite_tests(Yep);
    }
}
// ---Run_Usb_API_tests---
void Run_UsbRxbufferIsEmpty_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(UsbRxbufferIsEmpty_returns_true_if_pin_FT1248_MISO_is_HIGH);
        RUN_TEST(UsbRxbufferIsEmpty_returns_false_if_pin_FT1248_MISO_is_LOW);
    }
}
void Run_UsbTxbufferIsFull_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(UsbTxbufferIsFull_returns_true_if_pin_MIOSIO0_is_HIGH);
        RUN_TEST(UsbTxbufferIsFull_returns_false_if_pin_MIOSIO0_is_LOW);
    }
}
void Run_UsbReadByte_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(UsbReadByte_selects_the_FT221X);
        RUN_TEST(UsbReadByte_drives_databus_with_read_command);
        RUN_TEST(UsbReadByte_signals_FT221X_to_sample_the_databus);
    }
}
void Run_UsbWriteByte_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
    }
}
void Run_Usb_API_tests(bool run_test)
{
    if (run_test)
    {
        Run_UsbRxbufferIsEmpty_tests(Nope);
        Run_UsbTxbufferIsFull_tests(Nope);
        Run_UsbReadByte_tests(Yep);
        Run_UsbWriteByte_tests(Nope);
    }
}

void Usb_tests(bool run_test)
{
    if (run_test)
    {
        Run_Usb_Private_Ft_tests(Nope);
        Run_Usb_Ft_tests(Nope);
        Run_Usb_API_tests(Yep);
    }
}

int main()
{
    UNITY_BEGIN();
    BiColorLed_tests(Nope);
    ReadWriteBits_tests(Nope);
    Queue_tests(Nope);
    UartSpi_tests(Nope);
    Lis_tests(Nope);
    Flag_tests(Nope);
    SpiSlave_tests(Nope);
    /* =====[ test libs for usb-bridge ]===== */
    Usb_tests(Yep);
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    return UNITY_END();
}
