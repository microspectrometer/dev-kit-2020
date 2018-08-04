#ifndef _TEST_SPI_H
#define _TEST_SPI_H

//
/* =====[ Plumbing for all SPI devices ]===== */
//
void SetUp_ClearPendingSpiInterrupt(void);
void TearDown_ClearPendingSpiInterrupt(void);
void ClearPendingSpiInterrupt_reads_SPSR_and_SPDR(void);

//
/* =====[ SPI Slave ]===== */
//
void SpiSlaveInit_configures_pin_Miso_as_an_output(void);
void SpiSlaveInit_pulls_Miso_high(void);
void SpiSlaveInit_enables_the_SPI_hardware_module(void);
void SpiEnableInterrupt_enables_the_transfer_is_done_interrupt(void);
void SpiSlaveSignalDataIsReady_pulls_Miso_low(void);
void SpiSlaveSignalDataIsNotReady_pulls_Miso_high(void);
//
void SetUp_SpiSlaveRead(void); void TearDown_SpiSlaveRead(void);
void SpiSlaveRead_waits_until_transfer_is_done(void);
void SpiSlaveRead_returns_the_SPI_data_register_byte(void);

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
//

#endif // _TEST_SPI_H
