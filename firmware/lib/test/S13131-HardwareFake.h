#ifndef _S13131_HARDWAREFAKE_H
#define _S13131_HARDWAREFAKE_H
#include <stdint.h>
#include "S13131.h"

// ---Fake hardware registers as PC memory for unit tests---
static uint8_t volatile fake_ddr;
static uint8_t volatile fake_port;
static uint8_t volatile fake_pin;
static uint8_t volatile fake_TCCR0A;
static uint8_t volatile fake_TCCR0B;
static uint8_t volatile fake_TIFR0;
static uint8_t volatile fake_OCR0A;
static uint8_t volatile fake_OCR0B;

// ---Registers---
s13131_ptr S13131_ddr    = &fake_ddr;
s13131_ptr S13131_port   = &fake_port;
s13131_ptr S13131_pin    = &fake_pin;
s13131_ptr S13131_TCCR0A = &fake_TCCR0A;
s13131_ptr S13131_TCCR0B = &fake_TCCR0B;
s13131_ptr S13131_TIFR0  = &fake_TIFR0;
s13131_ptr S13131_OCR0A  = &fake_OCR0A;
s13131_ptr S13131_OCR0B  = &fake_OCR0B;

// ---Pins---
s13131_pin S13131_Clk = 5; // PWM
s13131_pin S13131_St  = 6; // Start
s13131_pin S13131_Eos = 7; // End of scan

// ---Bits---
s13131_bit S13131_WGM00  = 0;  // PWM
s13131_bit S13131_WGM01  = 1;  // PWM
s13131_bit S13131_WGM02  = 3;  // PWM
s13131_bit S13131_CS00   = 0;   // PWM
s13131_bit S13131_CS01   = 1;   // PWM
s13131_bit S13131_CS02   = 2;   // PWM
s13131_bit S13131_COM0B0 = 4; // PWM
s13131_bit S13131_COM0B1 = 5; // PWM
s13131_bit S13131_OCF0A  = 1;  // PWM
s13131_bit S13131_OCF0B  = 2;  // PWM
#endif // _S13131_HARDWAREFAKE_H
