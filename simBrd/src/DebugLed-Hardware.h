#ifndef _DEBUG_LED_HARDWARE_H
#define _DEBUG_LED_HARDWARE_H

#include <stdint.h>
#include <avr/io.h>

// =====[ Use This File With <DebugLed.h> ]=====
// This file resolves hardware dependencies left undefined in <DebugLed.h>
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
uint8_t volatile * const DebugLed_ddr  = &DDRC;  // controls if input or output
uint8_t volatile * const DebugLed_port = &PORTC; // controls output high/low
uint8_t volatile * const DebugLed_pin  = &PINC;  // reads pin value
// =====[ DebugLed Pin Connection On simBrd ]=====
uint8_t const debug_led = PINC3;

#endif // _DEBUG_LED_HARDWARE_H


