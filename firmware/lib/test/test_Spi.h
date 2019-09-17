#ifndef _TEST_SPI_H
#define _TEST_SPI_H

//
/* =====[ Plumbing for all SPI devices ]===== */
//
// no setup or teardown:
void SetUp_ClearPendingSpiInterrupt(void);
void TearDown_ClearPendingSpiInterrupt(void);
void ClearPendingSpiInterrupt_reads_SPSR_and_SPDR(void);

//
/* =====[ SPI Slave ]===== */
//
// no setup or teardown:
void SpiEnableInterrupt_enables_the_transfer_is_done_interrupt(void);
//
void SetUp_SpiSlaveInit(void); void TearDown_SpiSlaveInit(void);
void SpiSlaveInit_configures_pin_Miso_as_an_output(void);
void SpiSlaveInit_makes_Miso_a_pullup_input_when_no_transfer_is_in_progress(void);
void SpiSlaveInit_enables_the_SPI_hardware_module(void);
void SpiSlaveInit_clears_pending_SPI_interrupt(void);
//
void SetUp_SpiSlaveSignalDataIsReady(void);
void TearDown_SpiSlaveSignalDataIsReady(void);
void SpiSlaveSignalDataIsReady_pulls_Miso_low(void);
//
void SetUp_SpiSlaveRead(void); void TearDown_SpiSlaveRead(void);
void SpiSlaveRead_waits_until_transfer_is_done(void);
void SpiSlaveRead_returns_the_SPI_data_register_byte(void);
//
void SetUp_SpiSlaveSendBytes(void); void TearDown_SpiSlaveSendBytes(void);
void SpiSlaveSendBytes_waits_for_master_read_between_each_byte(void);
//
/* =====[ SPI Master ]===== */
//
void SpiMasterOpenSpi_selects_the_SPI_slave(void);
void SpiMasterCloseSpi_unselects_the_SPI_slave(void);
void SpiTransferIsDone_returns_true_when_the_transfer_is_done(void);
void SpiTransferIsDone_returns_false_when_the_transfer_is_not_done(void);
void SpiResponseIsReady_returns_true_when_slave_signals_data_is_ready(void);
void SpiResponseIsReady_returns_false_when_slave_signals_data_not_ready(void);
//
void SetUp_SpiMasterInit(void); void TearDown_SpiMasterInit(void);
void SpiMasterInit_pulls_Ss_high(void);
void SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs(void);
void SpiMasterInit_makes_this_mcu_the_SPI_master(void);
void SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8(void);
void SpiMasterInit_enables_the_SPI_hardware_module(void);
void SpiMasterInit_protects_against_false_SpiResponseIsReady_signals(void);
void SpiMasterInit_clears_pending_SPI_interrupt(void);
//
void SetUp_SpiMasterWrite(void); void TearDown_SpiMasterWrite(void);
void SpiMasterWrite_byte_loads_SPI_tx_buffer_with_byte(void);
void SpiMasterWrite_byte_waits_for_transmission_to_complete(void);
//
void SetUp_SpiMasterWaitForResponse(void); void TearDown_SpiMasterWaitForResponse(void);
void SpiMasterWaitForResponse_waits_until_slave_signals_ready(void);
//
void SetUp_SpiMasterRead(void); void TearDown_SpiMasterRead(void);
void SpiMasterRead_returns_the_SPI_data_register(void);
void SpiMasterRead_waits_for_transmission_to_complete(void);
void SpiMasterRead_loads_SPI_tx_buffer_with_slave_ignore(void);
//
void LookupSensorCmd_returns_Nth_fn_for_Nth_key(void);
void LookupSensorCmd_returns_NULL_if_key_is_not_in_jump_table(void);
void LookupSensorCmd_example_calling_the_returned_command(void);
void SpiSlaveWrite_StatusOk_sends_0x00_0x02_0x00_valid_cmd(void);
void SpiSlaveWrite_StatusInvalid_sends_0x00_0x02_0xFF_invalid_cmd_name(void);

/* 2019-03-04 WIP: inline version of SpiMasterWrite */
void SpiMasterWriteN_NoInlineHelpers_sends_N_bytes_to_SpiSlave(void);
void SpiMasterWriteByte_sends_one_byte_to_SpiSlave(void);

#endif // _TEST_SPI_H