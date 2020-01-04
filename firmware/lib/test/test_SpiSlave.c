#include "unity.h"
#include "test_SpiSlave.h"
#include "SpiSlave.h"
#include "Mock.h" // record call history in "mock"

void SpiSlaveInit_makes_DataReady_an_output_pin(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_DataReady,
        *Spi_ddr,
        "Cannot run test: must start with ddr bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_ddr,
        "Expect DataReady to be an output."
        );
}
void SpiSlaveInit_idles_DataReady_high(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_DataReady,
        *Spi_port,
        "Cannot run test: must start with port bit clear!"
        );
    /* =====[ Operate ]===== */
    /* printf("Value of Spi_port before is 0x%02x\n", *Spi_port); */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    /* printf("Value of Spi_DataReady is pin %d\n", Spi_DataReady); */
    /* printf("Value of Spi_port after is 0x%02x\n", *Spi_port); */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DataReady,
        *Spi_port,
        "Expect DataReady to idle high."
        );
}
void SpiSlaveInit_makes_Miso_an_output_pin(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_Miso,
        *Spi_ddr,
        "Cannot run test: must start with ddr bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Miso,
        *Spi_ddr,
        "Expect Miso to be an output."
        );
}
void SpiSlaveInit_enables_SPI(void)
{
    /* =====[ Setup ]===== */
    *Spi_SPCR = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_Enable,
        *Spi_SPCR,
        "Cannot run test: must start with bit clear!"
        );
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Enable,
        *Spi_SPCR,
        "Expect bit 6 HIGH to enable SPI module."
        );
}
void SpiSlaveInit_enables_SPI_interrupt(void)
{
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    // check SpiSlaveInit calls `EnableSpiInterrupt`
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "EnableSpiInterrupt"),
        "Expect SpiSlaveInit enables the SPI interrupt."
        );
}

void SpiSlaveTx_loads_SPI_data_register_with_bytes_from_input_buffer(void)
{
    /* =====[ Setup ]===== */
    uint8_t const input_buffer[] = {0x00, 0x01, 0x02};
    uint16_t nbytes = (uint16_t)sizeof(input_buffer);
    /* printf("sizeof(input_buffer)=%zu\n",sizeof(input_buffer)); */
    /* printf("nbytes=%d\n",nbytes); */
    /* =====[ Operate ]===== */
    SpiSlaveTx(input_buffer, nbytes);
    /* =====[ Test ]===== */
    // TODO: test value of *all* bytes sent, not just the last
    TEST_ASSERT_EQUAL_UINT8(input_buffer[nbytes-1], *Spi_SPDR);
}
