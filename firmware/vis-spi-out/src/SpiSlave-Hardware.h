/** \file
 * This file is not used anymore.\n
 * There used to be separate Spi hardware files for master and
 * slave.
 * Now there is one Spi.h file and Spi-Hardware and
 * Spi-HardwareFake.
 * There are still SpiMaster.h and SpiSlave.h files, but these
 * are for the functions unique to master and slave.
 * */
#define _SPISLAVE_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "SpiSlave.h"
// ---Registers---
spi_ptr Spi_ddr = &DDRB; // controls if pin is input or output
spi_ptr Spi_port = &PORTB; // controls if pin outputs HIGH or LOW
spi_ptr Spi_SPCR = &SPCR; // SPI control register
spi_ptr Spi_SPDR = &SPDR;  // SPI data register
spi_ptr Spi_SPSR = &SPSR;  // SPI status register
// ---Pins---
spi_pin Spi_DataReady = PB1; // slave signal DR to master
spi_pin Spi_Miso = PB4;    // master-in,  slave-out
// ---Bits---
spi_bit Spi_Enable          = SPE;
spi_bit Spi_InterruptEnable = SPIE;
spi_bit Spi_InterruptFlag   = SPIF;
#endif // _SPISLAVE_HARDWARE_H
