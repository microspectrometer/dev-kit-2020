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
lis_prt Lis_TCCR0A = &TCCR0A;
lis_prt Lis_TCCR0B = &TCCR0B;
lis_prt Lis_OCR0A  = &OCR0A;
lis_prt Lis_OCR0B  = &OCR0B;
// ---Pins---
lis_pin Lis_PixSelect = PB0;
lis_pin Lis_Clk       = PD5;
lis_pin Lis_Rst       = PD6;
lis_pin Lis_Sync      = PD7;
#endif // _LIS_HARDWARE_H
