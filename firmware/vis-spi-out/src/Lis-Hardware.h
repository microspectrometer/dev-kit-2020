/** \file
 * Lis uses general purpose I/O pins and a PWM pin to communicate
 * with the Dynamax LIS-770i linear photodiode array.
 * */
#ifndef _LIS_HARDWARE_H
#define _LIS_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "Lis.h"
// ---Registers---
lis_ptr Lis_ddr1   = &DDRD;
lis_ptr Lis_port1  = &PORTD;
lis_ptr Lis_pin1   = &PIND;
lis_ptr Lis_ddr2   = &DDRB;
lis_ptr Lis_port2  = &PORTB;
lis_ptr Lis_TCCR0A = &TCCR0A; // PWM
lis_ptr Lis_TCCR0B = &TCCR0B; // PWM
lis_ptr Lis_TIFR0  = &TIFR0;  // PWM
lis_ptr Lis_OCR0A  = &OCR0A;  // PWM
lis_ptr Lis_OCR0B  = &OCR0B;  // PWM
// ---Pins---
lis_pin Lis_PixSelect = PB0;
lis_pin Lis_Clk       = PD5; // PWM
lis_pin Lis_Rst       = PD6;
lis_pin Lis_Sync      = PD7;
// ---Bits---
lis_bit Lis_WGM00  = WGM00;  // PWM
lis_bit Lis_WGM01  = WGM01;  // PWM
lis_bit Lis_WGM02  = WGM02;  // PWM
lis_bit Lis_CS00   = CS00;   // PWM
lis_bit Lis_CS01   = CS01;   // PWM
lis_bit Lis_CS02   = CS02;   // PWM
lis_bit Lis_COM0B0 = COM0B0; // PWM
lis_bit Lis_COM0B1 = COM0B1; // PWM
lis_bit Lis_OCF0A  = OCF0A;  // PWM
lis_bit Lis_OCF0B  = OCF0B;  // PWM
#endif // _LIS_HARDWARE_H
