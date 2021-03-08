/** \file
 * S13131 uses general purpose I/O pins and a PWM pin to communicate
 * with the Hamamatu S13131-512 CMOS image sensor.
 * */
#ifndef _S13131_HARDWARE_H
#define _S13131_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "S13131.h"

// ---Registers---
s13131_ptr S13131_ddr    = &DDRD;
s13131_ptr S13131_port   = &PORTD;
s13131_ptr S13131_pin    = &PIND;
s13131_ptr S13131_TCCR0A = &TCCR0A; // PWM timer 0 ctrl reg A
s13131_ptr S13131_TCCR0B = &TCCR0B; // PWM timer 0 ctrl reg B
s13131_ptr S13131_TIFR0  = &TIFR0;  // PWM timer 0 interrupt flags
s13131_ptr S13131_OCR0A  = &OCR0A;  // PWM period: fcpu/OCR0A
s13131_ptr S13131_OCR0B  = &OCR0B;  // PWM duty cycle: OCR0B/OCR0A

// ---Pins---
s13131_pin S13131_Clk = PD5; // PWM (ZIF pin 4 - LIS_CLK)
s13131_pin S13131_St  = PD6; // Start (ZIF pin 5 - LIS_RST)
s13131_pin S13131_Eos = PD7; // End of scan (ZIF pin 6 - LIS_SYNC)

// ---Bits---
s13131_bit S13131_WGM00  = WGM00;  // PWM
s13131_bit S13131_WGM01  = WGM01;  // PWM
s13131_bit S13131_WGM02  = WGM02;  // PWM
s13131_bit S13131_CS00   = CS00;   // PWM
s13131_bit S13131_CS01   = CS01;   // PWM
s13131_bit S13131_CS02   = CS02;   // PWM
s13131_bit S13131_COM0B0 = COM0B0; // PWM
s13131_bit S13131_COM0B1 = COM0B1; // PWM
s13131_bit S13131_OCF0A  = OCF0A;  // PWM
s13131_bit S13131_OCF0B  = OCF0B;  // PWM
#endif // _S13131_HARDWARE_H
