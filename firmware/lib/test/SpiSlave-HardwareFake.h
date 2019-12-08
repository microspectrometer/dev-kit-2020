#include <stdint.h>
#include "SpiSlave.h"

/* =====[ Fake i/o registers for unit tests ]===== */
static uint8_t volatile fake_ddr;
static uint8_t volatile fake_port;
static uint8_t volatile fake_SPCR;
static uint8_t volatile fake_SPDR;
static uint8_t volatile fake_SPSR;

spi_ptr Spi_ddr  = &fake_ddr;
spi_ptr Spi_port = &fake_port;
spi_ptr Spi_spcr = &fake_SPCR; // SPI control register
spi_ptr Spi_spdr = &fake_SPDR; // SPI data register
spi_ptr Spi_spsr = &fake_SPSR; // SPI status register

spi_pin Spi_DataReady = 1;
spi_pin Spi_Miso = 4;    // master-in,  slave-out
spi_bit Spi_Enable = 6; // bit 6 in SPCR
spi_bit Spi_InterruptEnable = 7; // bit 7 in SPCR
