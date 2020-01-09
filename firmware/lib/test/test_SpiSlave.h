#ifndef _TEST_SPISLAVE_H
#define _TEST_SPISLAVE_H

void SpiSlaveInit_makes_DataReady_an_output_pin(void);
void SpiSlaveInit_idles_DataReady_high(void);
void SpiSlaveInit_makes_Miso_an_output_pin(void);
void SpiSlaveInit_enables_SPI(void);
void SpiSlaveInit_enables_SPI_interrupt(void);

void SpiSlaveTx_sends_nbytes_of_input_buffer_to_SpiMaster(void);
void SpiSlaveTxByte_loads_SPI_data_register_with_input_byte(void);
void SpiSlaveTxByte_drives_DataReady_LOW_to_signal_data_is_ready(void);
void SpiSlave_faked_calls_are_still_available_for_testing(void);

#endif // _TEST_SPISLAVE_H
