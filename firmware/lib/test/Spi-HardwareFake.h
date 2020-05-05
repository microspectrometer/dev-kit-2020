#ifndef _SPI_HARDWAREFAKE_H
#define _SPI_HARDWAREFAKE_H

#include <stdint.h>
#include "Spi.h"

// ---Fake hardware registers as PC memory for unit tests---
static uint8_t volatile fake_Spi_PortDirection;
static uint8_t volatile fake_Spi_PortInput;
static uint8_t volatile fake_Spi_PortOutput;
static uint8_t volatile fake_Spi_PortPullup;
static uint8_t volatile fake_Spi_SPCR;
static uint8_t volatile fake_Spi_SPSR;
static uint8_t volatile fake_Spi_SPDR;

// ---Registers---
spi_reg Spi_PortDirection = &fake_Spi_PortDirection;
spi_reg Spi_PortInput     = &fake_Spi_PortInput;
spi_reg Spi_PortOutput    = &fake_Spi_PortOutput;
spi_reg Spi_SPCR          = &fake_Spi_SPCR;
spi_reg Spi_SPSR          = &fake_Spi_SPSR;
spi_reg Spi_SPDR          = &fake_Spi_SPDR;

// ---Pins---
spi_bit Spi_DataReady = 1;
spi_bit Spi_Ss        = 2;
spi_bit Spi_Mosi      = 3;
spi_bit Spi_Miso      = 4;
spi_bit Spi_Sck       = 5;
// ---Bits---
spi_bit Spi_MasterSlaveSelect = 4;
spi_bit Spi_ClockBit0 = 0;
spi_bit Spi_ClockBit1 = 1;
spi_bit Spi_DoubleClock = 0;
spi_bit Spi_Enable = 6;
spi_bit Spi_InterruptEnable = 7;
spi_bit Spi_InterruptFlag = 7;

#endif // _SPI_HARDWAREFAKE_H
