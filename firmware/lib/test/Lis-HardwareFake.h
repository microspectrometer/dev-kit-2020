#ifndef _LIS_HARDWAREFAKE_H
#define _LIS_HARDWAREFAKE_H
#include <stdint.h>
#include "Lis.h"
// ---Fake hardware registers as PC memory for unit tests---
static uint8_t volatile fake_ddr1;
static uint8_t volatile fake_port1;
static uint8_t volatile fake_pin1;
static uint8_t volatile fake_ddr2;
static uint8_t volatile fake_port2;
static uint8_t volatile fake_TCCR0A;
static uint8_t volatile fake_TCCR0B;
static uint8_t volatile fake_OCR0A;
static uint8_t volatile fake_OCR0B;
// ---Registers---
lis_ptr Lis_ddr1 = &fake_ddr1;
lis_ptr Lis_port1 = &fake_port1;
lis_ptr Lis_pin1 = &fake_pin1;
lis_ptr Lis_ddr2 = &fake_ddr2;
lis_ptr Lis_port2 = &fake_port2;
lis_ptr Lis_TCCR0A = &fake_TCCR0A;
lis_ptr Lis_TCCR0B = &fake_TCCR0B;
lis_ptr Lis_OCR0A = &fake_OCR0A;
lis_ptr Lis_OCR0B = &fake_OCR0B;
// ---Pins---
lis_pin Lis_PixSelect = 0;
lis_pin Lis_Clk = 5;
lis_pin Lis_Rst = 6;
lis_pin Lis_Sync = 7;
#endif // _LIS_HARDWAREFAKE_H

