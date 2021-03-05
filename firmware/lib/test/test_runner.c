// Pick a sensor
#ifdef LIS // <------------- $ make sensor=LIS
#include "LisConfigs.h"
#include "test_Lis.h"
#endif
#ifdef S13131 // <---------- $ make sensor=S13131
#endif

#include <unity.h>
#include <mock.h>
#include "HardwareFake.h" // Fake hardware (registers, pins)
#include "StatusCodes.h" // Python-to-Firmware communication status codes

#include "test_BiColorLed.h"
#include "test_ReadWriteBits.h"
#include "test_Flag.h"
#include "test_Spi.h"
#include "test_SpiSlave.h"
#include "test_SpiMaster.h"
#include "test_Queue.h"
#include "test_UartSpi.h"
#include "test_Usb.h"
#include "test_StatusCode.h"

void (*setUp)(void);
void (*tearDown)(void);
void EmptySetup(void) {
}
void EmptyTeardown(void) {
}
Mock_s *mock;
void SetUp_Mock(void){
    mock = Mock_new();
}
void TearDown_Mock(void){
    Mock_destroy(mock);
    mock = NULL;
}
bool Yep = true;
bool Nope = false;

void test_BiColorLed(bool run_test)
{
    if (run_test)
    {
        puts("# BiColorLed tests");

        setUp = EmptySetup;
        tearDown = EmptyTeardown;

        puts("## BiColorLedOn");
        RUN_TEST(BiColorLedOn_sets_bit_in_ddr);
        puts("## BiColorLedOff");
        RUN_TEST(BiColorLedOff_clears_bit_in_ddr);
        puts("## BiColorLedGreen");
        RUN_TEST(BiColorLedGreen_clears_bit_in_port);
        puts("## BiColorLedRed");
        RUN_TEST(BiColorLedRed_sets_bit_in_port);

    }
}

void test_ReadWriteBits(bool run_test)
{
    if (run_test)
    {
        puts("# ReadWriteBits tests");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        puts("## SetBit");
        RUN_TEST(SetBit_sets_bit_in_register);
        puts("## ClearBit");
        RUN_TEST(ClearBit_clears_bit_in_register);
        puts("## BitIsSet");
        RUN_TEST(BitIsSet_returns_true_if_bit_is_set);
        RUN_TEST(BitIsSet_returns_false_if_bit_is_clear);
        puts("## BitIsClear");
        RUN_TEST(BitIsClear_returns_true_if_bit_is_clear);
        RUN_TEST(BitIsClear_returns_false_if_bit_is_set);
    }
}

void test_Spi(bool run_test)
{
    if (run_test)
    {
        puts("# Spi tests");

        puts("## ClearSpiInterruptFlag");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(ClearSpiInterruptFlag_first_reads_SPI_status_register);
        RUN_TEST(ClearSpiInterruptFlag_then_reads_SPI_data_register);

        puts("## _SpiTransferIsDone");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(SpiTransferIsDone_returns_true_if_the_SPI_Interrupt_Flag_is_set);
        RUN_TEST(SpiTransferIsDone_returns_false_if_the_SPI_Interrupt_Flag_is_clear);
    }
}

void test_SpiSlave(bool run_test)
{
    if (run_test)
    {
        puts("# SpiSlave tests");

        puts("## _SignalDataReady");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(SignalDataReady_drives_DataReady_LOW);

        puts("## DisableSpiInterrupt");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(DisableSpiInterrupt_clears_the_SPI_Interrupt_Enable_bit);

        puts("## EnableSpiInterrupt");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(EnableSpiInterrupt_clears_SPI_interrupt_flag);
        RUN_TEST(EnableSpiInterrupt_enables_interrupt_SPI_Transfer_Complete);
        RUN_TEST(EnableSpiInterrupt_consumes_6_cycles);

        puts("## SpiSlaveInit");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(SpiSlaveInit_makes_DataReady_an_output_pin);
        RUN_TEST(SpiSlaveInit_idles_DataReady_high);
        RUN_TEST(SpiSlaveInit_makes_Miso_an_output_pin);
        RUN_TEST(SpiSlaveInit_enables_SPI);
        RUN_TEST(SpiSlaveInit_enables_SPI_interrupt);

        puts("## SpiSlaveTxByte");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(SpiSlaveTxByte_loads_SPI_data_register_with_input_byte);
        RUN_TEST(SpiSlaveTxByte_disables_SPI_ISR_before_signaling_data_ready);
        RUN_TEST(SpiSlaveTxByte_drives_DataReady_LOW_to_signal_data_is_ready);
        RUN_TEST(SpiSlaveTxByte_waits_until_SPI_transfer_is_done);
        RUN_TEST(SpiSlaveTxByte_drives_DataReady_HIGH_immediately_after_SPI_transfer_finishes);
        RUN_TEST(SpiSlaveTxByte_enables_SPI_ISR_after_transfer);

        puts("## SpiSlaveTx");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(SpiSlaveTx_sends_nbytes_of_input_buffer_to_SpiMaster);
    }
}

void SetUp_SpiMasterXfrByte(void) {
    SetBit(Spi_SPSR, Spi_InterruptFlag);
}
void TearDown_SpiMasterXfrByte(void) {
    ClearBit(Spi_SPSR, Spi_InterruptFlag);
}
void test_SpiMaster(bool run_test)
{
    if (run_test)
    {

        puts("# SpiMaster tests");

        puts("## SpiMasterInit");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(SpiMasterInit_idles_SlaveSelect_high);
        RUN_TEST(SpiMasterInit_makes_SlaveSelect_an_output);
        RUN_TEST(SpiMasterInit_makes_Miso_an_input);
        RUN_TEST(SpiMasterInit_enables_pullup_on_Miso);
        RUN_TEST(SpiMasterInit_makes_DataReady_an_input);
        RUN_TEST(SpiMasterInit_enables_pullup_on_DataReady);
        RUN_TEST(SpiMasterInit_makes_Mosi_an_output);
        RUN_TEST(SpiMasterInit_makes_Sck_an_output);
        RUN_TEST(SpiMasterInit_makes_this_MCU_the_SPI_Master);
        RUN_TEST(SpiMasterInit_sets_SPI_Clock_to_10MHz_ext_osc_divided_by_8);
        RUN_TEST(SpiMasterInit_enables_the_SPI_hardware_module);
        RUN_TEST(SpiMasterInit_clears_SPI_interrupt_flag);

        puts("## SpiMasterXfrByte");
        setUp = SetUp_SpiMasterXfrByte;
        tearDown = TearDown_SpiMasterXfrByte;
        RUN_TEST(SpiMasterXfrByte_selects_the_SPI_slave);
        RUN_TEST(SpiMasterXfrByte_loads_SPI_data_reg_with_the_byte_to_send);
        RUN_TEST(SpiMasterXfrByte_waits_until_the_transfer_is_done_by_reading_the_SPI_Interrupt_Flag);
        RUN_TEST(SpiMasterXfrByte_unselects_the_SPI_slave);
        RUN_TEST(SpiMasterXfrByte_clears_the_SPI_Interrupt_Flag_by_reading_the_SPI_data_reg);
        RUN_TEST(SpiMasterXfrByte_returns_the_byte_in_the_SPI_data_reg);
    }
}

void test_Queue(bool run_test)
{
    if (run_test)
    {
        puts("# Queue tests");

        puts("## QueueInit");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(QueueInit_returns_a_pointer_to_a_Queue_struct);
        RUN_TEST(QueueInit_memory_for_Queue_struct_is_allocated_in_Queue_object_file);
        RUN_TEST(QueueInit_assigns_input_buffer_as_Queue_buffer);
        RUN_TEST(QueueInit_size_input_is_the_maximum_Queue_length);
        RUN_TEST(QueueInit_initializes_Queue_with_length_0);

        puts("## QueueLength");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(QueueLength_increments_after_a_push);
        RUN_TEST(QueueLength_does_not_increase_beyond_max_length);
        RUN_TEST(QueueLength_decrements_after_a_pop);
        RUN_TEST(QueueLength_does_not_decrease_below_zero);

        puts("## QueuePush");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(QueuePush_writes_byte_to_Queue_buffer);
        RUN_TEST(QueuePush_writes_next_byte_to_address_after_previous_write);
        RUN_TEST(QueuePush_does_not_write_byte_if_Queue_is_full);
        RUN_TEST(QueuePush_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_full);

        puts("## QueuePop");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(QueuePop_removes_oldest_byte_from_Queue);
        RUN_TEST(QueuePop_returns_oldest_byte);
        RUN_TEST(QueuePop_does_not_remove_any_bytes_if_Queue_is_empty);
        RUN_TEST(QueuePop_returns_0_if_Queue_is_empty);
        RUN_TEST(QueuePop_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_empty);

        puts("## QueueIsFull");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(QueueIsFull_returns_true_if_Queue_is_full);
        RUN_TEST(QueueIsFull_returns_false_if_Queue_is_not_full);

        puts("## QueueIsEmpty");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(QueueIsEmpty_returns_true_if_Queue_is_empty);
        RUN_TEST(QueueIsEmpty_returns_false_if_Queue_is_not_empty);
    }
}

void test_UartSpi(bool run_test)
{
    if (run_test)
    {
        puts("# UartSpi tests");

        puts("## UartSpiInit");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
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

void test_Lis(bool run_test)
{
    if (run_test)
    {
        puts("# Lis tests");

        puts("## MSB");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(MSB_returns_most_significant_bit_of_16bit_input);

        puts("## LSB");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(LSB_returns_least_significant_bit_of_16bit_input);

        puts("## LisInit");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
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

        puts("## LisConfigIsValid");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(LisConfigIsValid_returns_false_if_binning_is_invalid);
        RUN_TEST(LisConfigIsValid_returns_false_if_gain_is_invalid);
        RUN_TEST(LisConfigIsValid_returns_false_if_active_rows_is_invalid);
        RUN_TEST(LisConfigIsValid_returns_true_if_config_is_valid);

        puts("## _ConfigAs28bits");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
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

        puts("## _WaitForLisClkLow");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(WaitForLisClkLow_clears_flag_PwmTimerMatchesOCF0B);
        RUN_TEST(WaitForLisClkLow_waits_until_flag_PwmTimerMatchesOCF0B_is_set);

        puts("## _WaitForLisClkHigh");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(WaitForLisClkHigh_clears_flag_PwmTimerMatchesOCF0A);
        RUN_TEST(WaitForLisClkHigh_waits_until_flag_PwmTimerMatchesOCF0A_is_set);

        puts("## _EnterLisProgrammingMode");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(EnterLisProgrammingMode_waits_for_LisClk_LOW);
        RUN_TEST(EnterLisProgrammingMode_asserts_LisPixSelect_to_program_Lis);

        puts("## _WriteLisConfigBit");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(WriteLisConfigBit_outputs_bit_on_LisRst);
        RUN_TEST(WriteLisConfigBit_waits_for_LisClk_HIGH);
        RUN_TEST(WriteLisConfigBit_waits_for_LisClk_LOW);

        puts("## _Write28bitLisConfig");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(Write28bitLisConfig_writes_28bits_starting_at_byte0_bit0_and_ending_at_byte3_bit3);

        puts("## _ExitLisProgrammingMode");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(ExitLisProgrammingMode_outputs_LOW_on_pin_LisRst);
        RUN_TEST(ExitLisProgrammingMode_outputs_LOW_on_pin_LisPixSelect);

        puts("## LisWriteConfig");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(LisWriteConfig_converts_config_to_28bit_sequence);
        RUN_TEST(LisWriteConfig_enters_LIS_programming_mode);
        RUN_TEST(LisWriteConfig_writes_28bits_to_LIS_setup_register);
        RUN_TEST(LisWriteConfig_exits_LIS_programming_mode);

        puts("## LisExpose");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(LisExpose_waits_for_the_falling_edge_of_Lis_Clk);
        RUN_TEST(LisExpose_starts_exposure_by_driving_Lis_Rst_HIGH);
        RUN_TEST(LisExpose_counts_falling_edges_of_Lis_Clk_until_count_equals_exposure_ticks);
        RUN_TEST(LisExpose_stops_exposure_by_driving_Lis_Rst_LOW);
    }
}

void test_Usb(bool run_test)
{
    if (run_test)
    {
        puts("# Usb tests");

        puts("## _FtClockDatabus");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(FtClockDatabus_drives_FtClock_HIGH_if_direction_is_FtDrive);
        RUN_TEST(FtClockDatabus_drives_FtClock_LOW_if_direction_is_FtSample);

        puts("## _FtReadDatabus");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(FtReadDatabus_copies_databus_pin_values_to_address_pbyte);

        puts("## _FtWriteDatabus");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(FtWriteDatabus_outputs_byte_on_databus_pins);

        puts("## _FtDatabusPinDirection");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(FtDatabusPinDirection_makes_databus_pins_outputs_if_direction_is_FtOut);
        RUN_TEST(FtDatabusPinDirection_makes_databus_pins_inputs_if_direction_is_FtIn);

        puts("## FtSelectFT221X");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(FtSelectFT221X_drives_FtChipSelect_LOW);

        puts("## FtUnselectFT221X");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(FtUnselectFT221X_drives_FtChipSelect_HIGH);

        puts("## FtBusTurnaround");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(FtBusTurnaround_clocks_one_cycle_to_signal_data_drive_then_data_sample);

        puts("## FtIsOk");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(FtIsOk_returns_true_if_FtMiso_is_LOW);
        RUN_TEST(FtIsOk_returns_false_if_FtMiso_is_HIGH);

        puts("## FtRead");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(FtRead_clocks_one_byte_out_of_the_FT221X);
        RUN_TEST(FtRead_stores_the_byte_at_address_pbyte);

        puts("## FtWrite");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(FtWrite_signals_to_drive_data_onto_the_databus);
        RUN_TEST(FtWrite_sets_microcontroller_databus_pins_as_outputs);
        RUN_TEST(FtWrite_outputs_byte_on_databus_pins);
        RUN_TEST(FtWrite_signals_FT221X_to_sample_the_databus);
        RUN_TEST(FtWrite_sets_microcontroller_databus_pins_as_inputs);

        puts("## UsbRxbufferIsEmpty");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(UsbRxbufferIsEmpty_returns_true_if_pin_FT1248_MISO_is_HIGH);
        RUN_TEST(UsbRxbufferIsEmpty_returns_false_if_pin_FT1248_MISO_is_LOW);

        puts("## UsbTxbufferIsFull");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(UsbTxbufferIsFull_returns_true_if_pin_MIOSIO0_is_HIGH);
        RUN_TEST(UsbTxbufferIsFull_returns_false_if_pin_MIOSIO0_is_LOW);

        puts("## UsbReadByte");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(UsbReadByte_selects_the_FT221X);
        RUN_TEST(UsbReadByte_drives_databus_with_read_command);
        RUN_TEST(UsbReadByte_signals_FT221X_to_sample_the_databus);
        RUN_TEST(UsbReadByte_reads_the_byte_if_data_transfer_status_is_OK);
        RUN_TEST(UsbReadByte_unselects_the_FT221X);
        RUN_TEST(UsbReadByte_returns_either_FtOK_if_pbyte_has_the_read_data_or_FtError_if_Usb_receive_buffer_was_empty);

        puts("## UsbWriteByte");
        // TODO(sustainablelab): write tests
        puts("FAIL: Implement Test");

    }
}

void test_StatusCode(bool run_test)
{
    if (run_test)
    {
        puts("# StatusCode tests");

        puts("## led_setting_is_valid");
        setUp = EmptySetup;
        tearDown = EmptyTeardown;
        RUN_TEST(led_setting_is_valid_returns_TRUE_if_setting_is_OFF);
        RUN_TEST(led_setting_is_valid_returns_TRUE_if_setting_is_GREEN);
        RUN_TEST(led_setting_is_valid_returns_TRUE_if_setting_is_RED);
        RUN_TEST(led_setting_is_valid_returns_FALSE_if_setting_is_any_other_value);

    }
}

int main()
{
    UNITY_BEGIN();
    test_BiColorLed(Yep);
    test_ReadWriteBits(Yep);
    puts("# Flag test");
    setUp = EmptySetup;
    tearDown = EmptyTeardown;
    RUN_TEST(test_Flag);
    test_Spi(Yep);
    test_SpiSlave(Yep);
    test_SpiMaster(Yep);
    test_Queue(Yep);
    test_UartSpi(Yep);
    test_Lis(Yep);
    test_Usb(Yep);
    test_StatusCode(Yep);

    return UNITY_END();
}
