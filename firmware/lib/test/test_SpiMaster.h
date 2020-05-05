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

#endif // _TEST_SPIMASTER_H
