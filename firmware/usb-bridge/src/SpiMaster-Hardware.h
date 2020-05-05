/** \file
 * This file is not used anymore.\n
 * There used to be separate Spi hardware files for master and
 * slave.
 * Now there is one Spi.h file and Spi-Hardware and
 * Spi-HardwareFake.
 * There are still SpiMaster.h and SpiSlave.h files, but these
 * are for the functions unique to master and slave.
 * */
#ifndef _SPIMASTER_HARDWARE_H
#define _SPIMASTER_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "SpiMaster.h"
// ---Registers---
spi_reg Spi_ddr  = &DDRB;
spi_reg Spi_pin  = &PINB;
spi_reg Spi_port = &PORTB;
spi_reg Spi_SPCR = &SPCR;
spi_reg Spi_SPSR = &SPSR;
spi_reg Spi_SPDR = &SPDR;
// ---Pins---
spi_bit Spi_DataReady = PORTB1;
spi_bit Spi_Ss        = PORTB2;
spi_bit Spi_Mosi      = PORTB3;
spi_bit Spi_Miso      = PORTB4;
spi_bit Spi_Sck       = PORTB5;
// ---Bits---
spi_bit Spi_MasterSlaveSelect = MSTR;  // bit 4 in SPCR (0x2C)
spi_bit Spi_ClockBit0         = SPR0;  // bit 0 in SPCR (0x2C)
spi_bit Spi_ClockBit1         = SPR1;  // bit 1 in SPCR (0x2C)
spi_bit Spi_DoubleClock       = SPI2X; // bit 0 in SPSR (0x2D)
spi_bit Spi_SpiEnable         = SPE;   // bit 6 in SPCR (0x2C)

#endif // _SPIMASTER_HARDWARE_H

