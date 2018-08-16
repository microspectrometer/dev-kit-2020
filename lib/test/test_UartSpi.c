#include "test_UartSpi.h"
#include "UartSpi.h"                // lib under test
#include "fake/UartSpi-Hardware.h"  // fake hardware dependencies in UartSpi.h
#include <unity.h>                  // unit testing framework

/* =====[ List of UartSpi Plumbing Tests ]===== */
    // [x] UartSpiInit_loads_0_into_baud_rate_register
        // this runs the SPI clock at 5MHz for a 10MHz CPU clock
    // [x] UartSpiInit_sets_Sck_as_an_output
void UartSpiInit_loads_0_into_baud_rate_register(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_br = 1;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8( 0, *UartSpi_br);
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
