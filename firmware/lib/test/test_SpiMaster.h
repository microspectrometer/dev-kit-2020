#ifndef _TEST_SPIMASTER_H
#define _TEST_SPIMASTER_H

// SpiMasterInit()
void SpiMasterInit_idles_SlaveSelect_high(void);
void SpiMasterInit_makes_SlaveSelect_an_output(void);
void SpiMasterInit_makes_Miso_an_input(void);
void SpiMasterInit_enables_pullup_on_Miso(void);
void SpiMasterInit_makes_DataReady_an_input(void);
void SpiMasterInit_enables_pullup_on_DataReady(void);
void SpiMasterInit_makes_Mosi_an_output(void);
void SpiMasterInit_makes_Sck_an_output(void);
void SpiMasterInit_makes_this_MCU_the_SPI_Master(void);
void SpiMasterInit_sets_SPI_Clock_to_10MHz_ext_osc_divided_by_8(void);
void SpiMasterInit_enables_the_SPI_hardware_module(void);
void SpiMasterInit_clears_SPI_interrupt_flag(void);
// SpiMasterTxByte(uint8_t byte)
void SpiMasterTxByte_selects_the_SPI_slave(void);
void SpiMasterTxByte_loads_SPI_data_reg_with_the_byte_to_send(void);
void SpiMasterTxByte_waits_until_the_transfer_is_done_by_reading_the_SPI_Interrupt_Flag(void);
void SpiMasterTxByte_clears_the_SPI_Interrupt_Flag(void);
void SpiMasterTxByte_unselects_the_SPI_slave(void);

#endif // _TEST_SPIMASTER_H
