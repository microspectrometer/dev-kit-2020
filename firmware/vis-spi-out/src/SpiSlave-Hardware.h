#ifndef _SPISLAVE_HARDWARE_H
#define _SPISLAVE_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "SpiSlave.h"
// ---Registers---
spi_ptr Spi_ddr = &DDRB; // controls if pin is input or output
spi_ptr Spi_port = &PORTB; // controls if pin outputs HIGH or LOW
spi_ptr Spi_spcr = &SPCR; // SPI control register
spi_ptr Spi_spdr = &SPDR;  // SPI data register
spi_ptr Spi_spsr = &SPSR;  // SPI status register
// ---Pins---
spi_pin Spi_DataReady = PB1; // slave signal DR to master
spi_pin Spi_Miso = PB4;    // master-in,  slave-out
// ---Bits---
spi_bit Spi_Enable = SPE;
spi_bit Spi_InterruptEnable = SPIE;
#endif // _SPISLAVE_HARDWARE_H
