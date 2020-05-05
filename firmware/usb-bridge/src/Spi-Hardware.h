#ifndef _SPI_HARDWARE_H
#define _SPI_HARDWARE_H

#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "Spi.h"

// ---i/o Registers---
//! Atmel DDR
spi_reg Spi_PortDirection = &DDRB;
//! Atmel PIN
spi_reg Spi_PortInput     = &PINB;
//! Atmel PORT
spi_reg Spi_PortOutput    = &PORTB;
//! Atmel PORT alias
spi_reg Spi_PortPullup    = &PORTB;
//! SPI Control Register
spi_reg Spi_SPCR = &SPCR;
//! SPI Status Register
spi_reg Spi_SPSR = &SPSR;
//! SPI Status Register
spi_reg Spi_SPDR = &SPDR;

// ---i/o Pins---
spi_bit Spi_DataReady = PORTB1;
spi_bit Spi_Ss        = PORTB2;
spi_bit Spi_Mosi      = PORTB3;
spi_bit Spi_Miso      = PORTB4;
spi_bit Spi_Sck       = PORTB5;

// ---i/o Bits---
spi_bit Spi_MasterSlaveSelect = MSTR;  // bit 4 in SPCR (0x2C)
spi_bit Spi_ClockBit0         = SPR0;  // bit 0 in SPCR (0x2C)
spi_bit Spi_ClockBit1         = SPR1;  // bit 1 in SPCR (0x2C)
spi_bit Spi_DoubleClock       = SPI2X; // bit 0 in SPSR (0x2D)
spi_bit Spi_Enable            = SPE;   // bit 6 in SPCR (0x2C)
spi_bit Spi_InterruptEnable   = SPIE;  // bit 7 in SPCR (0x2C)
spi_bit Spi_InterruptFlag     = SPIF;  // bit 7 in SPSR (0x2D)

#endif // _SPI_HARDWARE_H
