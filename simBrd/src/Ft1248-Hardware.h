#ifndef _FT1248_HW_H
#define _FT1248_HW_H

#include <stdint.h>  // uint8_t
#include <avr/io.h>  // defines IO addresses and pin names

// =====[ Use This File With "Ft1248.h" ]=====
// This file resolves hardware dependencies declared extern in "Ft1248.h".
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

//  =====[ I/O Register Address ]=====          =====[ Register's Purpose ]=====
// control lines
uint8_t volatile * const Ft1248_ddr     =   &DDRC;  // data direction in/out
uint8_t volatile * const Ft1248_port    =   &PORTC; // output (Port out)
uint8_t volatile * const Ft1248_pin     =   &PINC;  // input  (Port in)
// bi-directional data lines (driven by both master and slave)
uint8_t volatile * const FtMiosio_ddr   =   &DDRD; 
uint8_t volatile * const FtMiosio_port  =   &PORTD;
uint8_t volatile * const FtMiosio_pin   =   &PIND; 

// =====[ Ft1248 Pin Connection On simBrd ]=====
// control lines
uint8_t const Ft1248_Ss     =   PC0;    // Slave select
uint8_t const Ft1248_Sck    =   PC1;    // Ft1248 bus clock
uint8_t const Ft1248_Miso   =   PC2;    // slave data line: master-in, slave-out
// bi-directional data lines (driven by both master and slave)
uint8_t const FtMiosio0     =   PD0;
uint8_t const FtMiosio1     =   PD1;
uint8_t const FtMiosio2     =   PD2;
uint8_t const FtMiosio3     =   PD3;
uint8_t const FtMiosio4     =   PD4;
uint8_t const FtMiosio5     =   PD5;
uint8_t const FtMiosio6     =   PD6;
uint8_t const FtMiosio7     =   PD7;

#endif // _FT1248_HW_H


