#include "unity.h"
#include "test_UartSpi.h"
#include "UartSpi.h"
void UartSpiInit_loads_0_into_baud_rate_register(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_UBRR0 = 1;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8( 0, *UartSpi_UBRR0);
}
