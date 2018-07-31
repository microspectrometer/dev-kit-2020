#include "test_SpiMaster.h"
#include "SpiMaster.h"          // lib under test
#include "fake/Spi-Hardware.h"  // fake hardware dependencies in SpiMaster.h
#include <unity.h>              // unit testing framework
#include "ReadWriteBits.h"
/* =====[ List of Tests ]===== */
    // [x] SpiMasterInit_pulls_Ss_high
    // [x] SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs
    // [x] SpiMasterInit_makes_this_mcu_the_SPI_master
    // [x] SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8
    // [x] SpiMasterInit_enables_the_SPI_hardware_module
    // [x] SpiMasterOpenSpi_selects_the_SPI_slave
    // [x] SpiMasterCloseSpi_unselects_the_SPI_slave
    // [x] SpiMasterTransmit_byte_loads_SPI_tx_buffer_with_byte
    // [ ] SpiMasterTransmit_byte_starts_a_transmission
    //
void SpiMasterInit_pulls_Ss_high(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_Ss, *Spi_port);
}
void SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(Spi_Ss,    *Spi_ddr, "Failed for pin Ss.");
    TEST_ASSERT_BIT_HIGH_MESSAGE(Spi_Mosi,  *Spi_ddr, "Failed for pin Mosi.");
    TEST_ASSERT_BIT_HIGH_MESSAGE(Spi_Sck,   *Spi_ddr, "Failed for pin Sck.");
}
void SpiMasterInit_makes_this_mcu_the_SPI_master(void)
{
    /* =====[ Setup ]===== */
    *Spi_spcr = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_MasterSlaveSelect,
        *Spi_spcr,
        "Expect bit is high if this is the master."
        );
}
void SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8(void)
{
    /* =====[ Setup ]===== */
    ClearBit  (Spi_spcr, Spi_ClockRateBit0);
    SetBit(Spi_spcr, Spi_ClockRateBit1);
    ClearBit(Spi_spsr, Spi_DoubleClockRate);
    //
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_ClockRateBit0,
        *Spi_spcr,
        "Failed for bit: ClockRateBit0."
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_ClockRateBit1,
        *Spi_spcr,
        "Failed for bit: ClockRateBit1."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DoubleClockRate,
        *Spi_spsr,
        "Failed for bit: DoubleClockRate."
        );
}
void SpiMasterInit_enables_the_SPI_hardware_module(void)
{
    /* =====[ Setup ]===== */
    *Spi_spcr = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
            Spi_Enable,
            *Spi_spcr,
            "Bit must be high to enable the SPI."
            );
}
void SpiMasterOpenSpi_selects_the_SPI_slave(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0xFF;
    /* =====[ Operate ]===== */
    SpiMasterOpenSpi();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Spi_Ss, *Spi_port);
}
void SpiMasterCloseSpi_unselects_the_SPI_slave(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterCloseSpi();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_Ss, *Spi_port);
}
void SpiMasterTransmit_byte_loads_SPI_tx_buffer_with_byte(void)
{
    /* =====[ Setup ]===== */
    *Spi_spdr = 0x00;  // start with 0x00 in the tx buffer
    uint8_t expect_byte = 0x09;
    /* =====[ Operate ]===== */
    uint8_t byte_to_send = expect_byte;
    SpiMasterTransmit(byte_to_send);
    /* =====[ Test ]===== */
    uint8_t actual_byte_loaded = *Spi_spdr;
    TEST_ASSERT_EQUAL_UINT8(expect_byte, actual_byte_loaded);
}
