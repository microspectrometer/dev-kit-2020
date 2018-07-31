#ifndef _SPI_HW_H
#define _SPI_HW_H

#include <stdint.h>  // uint8_t
#include <avr/io.h>  // defines IO addresses and pin names

// =====[ Use This File With "Spi.h" ]=====
// This file resolves hardware dependencies declared extern in "Spi.h".
//
// Hardware dependencies declared in this file are included by avr-gcc:
    // avr-gcc ... -mmcu=atmega328p -B ${atmega328_lib} ...
    // # atmega328_lib is the path to the .o and .a lib files and the spec file.
    // atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'
    // These lib object files are included with Atmel Studio 7.0. They are not
    // built when make builds the client project. The compiler uses the
    // atmega328 source headers to check macro values like 'DDRC'.
    // I take it on faith that Atmel's .o lib objects match their .h API files.
// The atmega328 hardware dependency API is in <avr/iom328p.h>
    // To open this file with 'gf', add the avr include path to the Vim path
    // Example:
    //      let avr_include='/cygdrive/c/Program\ Files\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include'
    //      let &path = &path . ',' . avr_include

//  =====[ I/O Register ]=====          =====[ Register's Purpose ]=====
uint8_t volatile * const Spi_ddr    =   &DDRB;  // data direction in/out
uint8_t volatile * const Spi_port   =   &PORTB; // output (Port out)
uint8_t volatile * const Spi_pin    =   &PINB;  // input (Port in)
uint8_t volatile * const Spi_spcr   =   &SPCR;  // SPI control register
uint8_t volatile * const Spi_spsr   =   &SPSR;  // SPI status register
uint8_t volatile * const Spi_spdr   =   &SPDR;  // SPI data register
/* =====[ SpiMaster Pin Connection On simBrd ]===== */
uint8_t const Spi_Ss    =   PB2;    // slave select driven by the master
uint8_t const Spi_Mosi  =   PB3;    // master-out, slave-in
uint8_t const Spi_Miso  =   PB4;    // master-in,  slave-out
uint8_t const Spi_Sck   =   PB5;    // Spi clock driven by the master

/* =====[ SPI control register bits ]===== */
uint8_t const Spi_MasterSlaveSelect = MSTR;
uint8_t const Spi_ClockRateBit0     = SPR0;
uint8_t const Spi_ClockRateBit1     = SPR1;
uint8_t const Spi_Enable            = SPE;

/* =====[ SPI status register bits ]===== */
uint8_t const Spi_DoubleClockRate   = SPI2X;
uint8_t const Spi_InterruptFlag     = SPIF;

#endif // _SPI_HW_H

