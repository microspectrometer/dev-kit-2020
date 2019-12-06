#include <stdint.h>
#include "SpiSlave.h"

/* =====[ Fake i/o registers for unit tests ]===== */
static uint8_t volatile value_in_fake_Spi_ddr;
static uint8_t volatile value_in_fake_Spi_port;

spi_ptr Spi_ddr  = &value_in_fake_Spi_ddr;
spi_ptr Spi_port = &value_in_fake_Spi_port;

spi_pin Spi_DataReady = 1;
