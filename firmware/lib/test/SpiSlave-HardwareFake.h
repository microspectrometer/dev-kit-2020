#ifndef _SPISLAVE_HARDWAREFAKE_H
#define _SPISLAVE_HARDWAREFAKE_H
#include <stdint.h>
#include "SpiSlave.h"

/* =====[ Fake i/o registers for unit tests ]===== */
static uint8_t volatile fake_ddr;
static uint8_t volatile fake_port;
static uint8_t volatile fake_SPCR;
static uint8_t volatile fake_SPDR;
static uint8_t volatile fake_SPSR;
// ---Registers---
spi_ptr Spi_ddr  = &fake_ddr;
spi_ptr Spi_port = &fake_port;
spi_ptr Spi_SPCR = &fake_SPCR; // SPI control register
spi_ptr Spi_SPDR = &fake_SPDR; // SPI data register
spi_ptr Spi_SPSR = &fake_SPSR; // SPI status register
// ---Pins---
spi_pin Spi_DataReady = 1;
spi_pin Spi_Miso = 4;    // master-in,  slave-out
// ---Bits---
spi_bit Spi_Enable = 6; // bit 6 in SPCR
spi_bit Spi_InterruptEnable = 7; // bit 7 in SPCR
spi_bit Spi_InterruptFlag   = 7; // bit 7 in SPSR

#endif // _SPISLAVE_HARDWAREFAKE_H
