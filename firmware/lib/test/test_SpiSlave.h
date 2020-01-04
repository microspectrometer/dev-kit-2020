#ifndef _TEST_SPISLAVE_H
#define _TEST_SPISLAVE_H

void SpiSlaveInit_makes_DataReady_an_output_pin(void);
void SpiSlaveInit_idles_DataReady_high(void);
void SpiSlaveInit_makes_Miso_an_output_pin(void);
void SpiSlaveInit_enables_SPI(void);
void SpiSlaveInit_enables_SPI_interrupt(void);

void SpiSlaveTx_loads_SPI_data_register_with_bytes_from_input_buffer(void);

#endif // _TEST_SPISLAVE_H
