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
void UartSpiInit_sets_AdcConv_to_idle_low(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_port = 0xFF;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        UartSpi_AdcConv, *UartSpi_port, "Failed for pin AdcConv."
        );
}
void UartSpiInit_sets_AdcConv_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_ddr = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_AdcConv, *UartSpi_ddr, "Failed for pin AdcConv."
        );
}
void UartSpiInit_enables_the_UART_in_Master_SPI_Mode(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_UCSR0C = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_UMSEL00,
        *UartSpi_UCSR0C,
        "Failed for bit ModeSelect0."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_UMSEL01,
        *UartSpi_UCSR0C,
        "Failed for bit ModeSelect1."
        );
}
void UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_UCSR0C = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_UCPOL0,
        *UartSpi_UCSR0C,
        "Failed for bit ClockPolarity."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_UCPHA0,
        *UartSpi_UCSR0C,
        "Failed for bit ClockPhase."
        );
}
void UartSpiInit_cfgs_SPI_to_transfer_MSB_first(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_UCSR0C = 0xFF;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        UartSpi_UDORD0,
        *UartSpi_UCSR0C,
        "Failed for bit DataOrder."
        );
}
void UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_UCSR0B = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_RXEN0,
        *UartSpi_UCSR0B,
        "Failed for bit RxEnable."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_TXEN0,
        *UartSpi_UCSR0B,
        "Failed for bit TxEnable."
        );
}
