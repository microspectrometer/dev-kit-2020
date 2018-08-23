#ifndef _PWM_HW_H
#define _PWM_HW_H

#include <stdint.h>    // uint8_t
//#include <avr/io.h>  // This is the file being faked.

// =====[ Use This File With lib header "Pwm.h" ]=====
// This file resolves hardware dependencies declared extern in the lib headers.
//
// =====[ Fake the I/O Register Addresses ]=====
// Fake IO by letting the compiler allocate arbitrary static memory addresses.
// Make these `static` because symbols `value_in_fake_PwmBlah` are not to be
// used by the test code. The symbols are only for obtaining addresses.
static uint8_t volatile value_in_fake_Pwm_tccr0a;
static uint8_t volatile value_in_fake_Pwm_tccr0b;
static uint8_t volatile value_in_fake_Pwm_tifr0;

//  =====[ I/O Register Addresses for 8-bit PWM ]=====
uint8_t volatile * const Pwm_tccr0a = &value_in_fake_Pwm_tccr0a;
uint8_t volatile * const Pwm_tccr0b = &value_in_fake_Pwm_tccr0b;
uint8_t volatile * const Pwm_tifr0  = &value_in_fake_Pwm_tifr0;
// =====[ Faking the I/O Register Bits ]=====
/* ---Bit Names--- */
uint8_t const Pwm_Wgm00 = 0;  // tccr0a
uint8_t const Pwm_Wgm01 = 1;  // tccr0a
uint8_t const Pwm_Wgm02 = 3;  // tccr0b
uint8_t const Pwm_Cs00  = 0;  // tccr0b
uint8_t const Pwm_Cs01  = 1;  // tccr0b
uint8_t const Pwm_Cs02  = 2;  // tccr0b
uint8_t const Pwm_Com0b0 = 4; // tccr0a
uint8_t const Pwm_Com0b1 = 5; // tccr0a
uint8_t const Pwm_Ocf0a = 1;  // tifr0
uint8_t const Pwm_Ocf0b = 2;  // tifr0


#endif // _PWM_HW_H
