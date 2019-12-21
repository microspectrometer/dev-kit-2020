#include "unity.h"
#include "test_UartSpi.h"
#include "UartSpi.h"
void UartSpiInit_clocks_SPI_bus_at_5MHz(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_UBRR0 = 1;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(0, *UartSpi_UBRR0);
}
void UartSpiInit_sets_Sck_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_ddr = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_Sck, *UartSpi_ddr, "Failed for pin Sck."
        );
}
