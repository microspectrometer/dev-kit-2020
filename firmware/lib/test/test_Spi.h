#ifndef _TEST_SPI_H
#define _TEST_SPI_H

/* =====[ ClearSpiInterruptFlag ]===== */
void ClearSpiInterruptFlag_first_reads_SPI_status_register(void);
void ClearSpiInterruptFlag_then_reads_SPI_data_register(void);

/* =====[ _SpiTransferIsDone ]===== */
void SpiTransferIsDone_returns_true_if_the_SPI_Interrupt_Flag_is_set(void);
void SpiTransferIsDone_returns_false_if_the_SPI_Interrupt_Flag_is_clear(void);

#endif // _TEST_SPI_H
