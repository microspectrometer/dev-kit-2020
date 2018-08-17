#ifndef _LIS_HW_H
#define _LIS_HW_H

#include <stdint.h>  // uint8_t
#include <avr/io.h>  // defines IO addresses and pin names

// =====[ Use This File With "Lis.h" ]=====
// This file resolves hardware dependencies declared extern in "Lis.h".
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
uint8_t volatile * const Lis_ddr1    =   &DDRD;  // data direction in/out
uint8_t volatile * const Lis_ddr2    =   &DDRB;  // data direction in/out
uint8_t volatile * const Lis_port1   =   &PORTD; // output (Port out)
uint8_t volatile * const Lis_port2   =   &PORTB; // output (Port out)

/* =====[ Lis pin connections on `mBrd` ]===== */
uint8_t const Lis_PixSelect = PB0;
uint8_t const Lis_Clk       = PD5; // ATmega328 OC0B       |clock (PWM)
uint8_t const Lis_Rst       = PD6;
uint8_t const Lis_Sync      = PD7;

#endif // _LIS_HW_H
