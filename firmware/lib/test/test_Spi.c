/** \file
  * *Empty tests are for generating function doc strings*\n 
  * Empty tests contain one line: `TEST_PASS();`
  * */
#include "unity.h"
#include "test_Spi.h"
#include "Spi.h"
#include "ReadWriteBits.h"

/* =====[ ClearSpiInterruptFlag ]===== */
void ClearSpiInterruptFlag_first_reads_SPI_status_register(void)
{
    TEST_PASS();
}
void ClearSpiInterruptFlag_then_reads_SPI_data_register(void)
{
    TEST_PASS();
}

/* =====[ _SpiTransferIsDone ]===== */
void SpiTransferIsDone_returns_true_if_the_SPI_Interrupt_Flag_is_set(void)
{
    /* =====[ Setup ]===== */
    SetBit(Spi_SPSR, Spi_InterruptFlag);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(_SpiTransferIsDone());
}
void SpiTransferIsDone_returns_false_if_the_SPI_Interrupt_Flag_is_clear(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Spi_SPSR, Spi_InterruptFlag);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(_SpiTransferIsDone());
}

