#ifndef _TEST_SPI_H
#define _TEST_SPI_H

//
/* =====[ SPI Master ]===== */
//
void SpiMasterOpenSpi_selects_the_SPI_slave(void);
void SpiMasterCloseSpi_unselects_the_SPI_slave(void);
void SpiTransferIsDone_returns_true_when_the_transfer_is_done(void);
void SpiTransferIsDone_returns_false_when_the_transfer_is_not_done(void);
void SpiMasterInit_pulls_Ss_high(void);
void SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs(void);
void SpiMasterInit_makes_this_mcu_the_SPI_master(void);
void SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8(void);
void SpiMasterInit_enables_the_SPI_hardware_module(void);
//
void SetUp_SpiMasterWrite(void); void TearDown_SpiMasterWrite(void);
void SpiMasterWrite_byte_loads_SPI_tx_buffer_with_byte(void);
void SpiMasterWrite_byte_waits_for_transmission_to_complete(void);

//
/* =====[ SPI Slave ]===== */
//
void SpiSlaveInit_configures_pin_Miso_as_an_output(void);
void SpiSlaveInit_enables_the_SPI_hardware_module(void);
void SpiEnableInterrupt_enables_the_transfer_is_done_interrupt(void);
//
void SetUp_SpiSlaveRead(void); void TearDown_SpiSlaveRead(void);
void SpiSlaveRead_waits_until_transfer_is_done(void);
void SpiSlaveRead_returns_the_SPI_data_register_byte(void);

#endif // _TEST_SPI_H
