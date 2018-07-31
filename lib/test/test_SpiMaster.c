#include "test_SpiMaster.h"
#include "SpiMaster.h"          // lib under test
#include "fake/Spi-Hardware.h"  // fake hardware dependencies in SpiMaster.h
#include <unity.h>              // unit testing framework
/* =====[ List of Tests ]===== */
    // [ ] SpiMasterInit_pulls_Ss_high
    // [ ] SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs
    // [ ] SpiMasterInit_makes_this_mcu_the_SPI_master
    // [ ] SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8
    // [ ] SpiMasterInit_enables_the_SPI_hardware_module
    // [x] SpiMasterOpenSpi_selects_the_SPI_slave
    // [x] SpiMasterCloseSpi_unselects_the_SPI_slave
    // [ ] SpiMasterTransmit_byte_loads_SPI_tx_buffer_with_byte
    // [ ] SpiMasterTransmit_byte_starts_a_transmission
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
