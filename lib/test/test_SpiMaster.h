#ifndef _TEST_SPIMASTER_H
#define _TEST_SPIMASTER_H

void SpiMasterOpenSpi_selects_the_SPI_slave(void);
void SpiMasterCloseSpi_unselects_the_SPI_slave(void);
void SpiMasterInit_pulls_Ss_high(void);
void SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs(void);
void SpiMasterInit_makes_this_mcu_the_SPI_master(void);
void SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8(void);
void SpiMasterInit_enables_the_SPI_hardware_module(void);

void Setup_SpiMasterWrite(void); void TearDown_SpiMasterWrite(void);
void SpiMasterWrite_byte_loads_SPI_tx_buffer_with_byte(void);
void SpiMasterWrite_byte_waits_for_transmission_to_complete(void);

#endif // _TEST_SPIMASTER_H
