#ifndef _TEST_SPISLAVE_H
#define _TEST_SPISLAVE_H

/* =====[ Check_SpiSlave_plumbing_for_fakes ]===== */
void SpiSlave_faked_calls_are_still_available_for_testing(void);

/* =====[ _SignalDataReady ]===== */
void SignalDataReady_drives_DataReady_LOW(void);

/* =====[ ClearSpiInterruptFlag ]===== */
void ClearSpiInterruptFlag_first_reads_SPI_status_register(void);
void ClearSpiInterruptFlag_then_reads_SPI_data_register(void);

/* =====[ _SpiTransferIsDone ]===== */
void SpiTransferIsDone_returns_true_if_the_SPI_Interrupt_Flag_is_set(void);
void SpiTransferIsDone_returns_false_if_the_SPI_Interrupt_Flag_is_clear(void);

/* =====[ DisableSpiInterrupt ]===== */
void DisableSpiInterrupt_clears_the_SPI_Interrupt_Enable_bit(void);

/* =====[ SpiSlaveInit ]===== */
void SpiSlaveInit_makes_DataReady_an_output_pin(void);
void SpiSlaveInit_idles_DataReady_high(void);
void SpiSlaveInit_makes_Miso_an_output_pin(void);
void SpiSlaveInit_enables_SPI(void);
void SpiSlaveInit_enables_SPI_interrupt(void);

/* =====[ SpiSlaveTx ]===== */
void SpiSlaveTx_sends_nbytes_of_input_buffer_to_SpiMaster(void);

/* =====[ SpiSlaveTxByte ]===== */
void SpiSlaveTxByte_loads_SPI_data_register_with_input_byte(void);
void SpiSlaveTxByte_disables_SPI_ISR_before_signaling_data_ready(void);
void SpiSlaveTxByte_drives_DataReady_LOW_to_signal_data_is_ready(void);
void SpiSlaveTxByte_waits_until_SPI_transfer_is_done(void);
void SpiSlaveTxByte_drives_DataReady_HIGH_immediately_after_SPI_transfer_finishes(void);
void SpiSlaveTxByte_enables_SPI_ISR_after_transfer(void);
void EnableSpiInterrupt_clears_SPI_interrupt_flag(void);
void EnableSpiInterrupt_enables_SPI_transfer_complete_interrupt(void);
void EnableSpiInterrupt_consumes_6_cycles(void);

#endif // _TEST_SPISLAVE_H
