#ifndef _PWM_HW_H
#define _PWM_HW_H

#include <stdint.h>  // uint8_t
#include <avr/io.h>  // defines IO addresses and pin names

// =====[ Use This File With "Pwm.h" ]=====
// This file resolves hardware dependencies declared extern in "Pwm.h".
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
uint8_t volatile * const Pwm_tccr0a = &TCCR0A; // timer0 control reg A
uint8_t volatile * const Pwm_tccr0b = &TCCR0B; // timer0 control reg B

/* ---Bit Names--- */
uint8_t const Pwm_Wgm00 = WGM00;   // tccr0a
uint8_t const Pwm_Wgm01 = WGM01;   // tccr0a
uint8_t const Pwm_Wgm02 = WGM02;   // tccr0b
uint8_t const Pwm_Cs00  = CS00;    // tccr0b
uint8_t const Pwm_Cs01  = CS01;    // tccr0b
uint8_t const Pwm_Cs02  = CS02;    // tccr0b
uint8_t const Pwm_Com0b0 = COM0B0; // tccr0a
uint8_t const Pwm_Com0b1 = COM0B1; // tccr0a

#endif // _PWM_HW_H
