#ifndef _LIS_H
#define _LIS_H
#include <stdint.h>
#include "ReadWriteBits.h"
//---Hardware types: register addresses, pin numbers, bit numbers---
typedef uint8_t volatile * const lis_ptr; // i/o reg address
typedef uint8_t const lis_pin; // bit index into i/o reg for an i/o pin
typedef uint8_t const lis_bit; // bit index into i/o reg

// Register address, pin number, and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/Lis-Hardware.h
// ---Registers---
extern lis_ptr Lis_ddr1;
extern lis_ptr Lis_port1;
extern lis_ptr Lis_pin1;
extern lis_ptr Lis_ddr2;
extern lis_ptr Lis_port2;
extern lis_ptr Lis_TCCR0A; // PWM timer 0 ctrl reg A
extern lis_ptr Lis_TCCR0B; // PWM timer 0 ctrl reg B
extern lis_ptr Lis_TIFR0;  // PWM timer 0 interrupt flags
extern lis_ptr Lis_OCR0A;  // PWM period: fcpu/OCR0A
extern lis_ptr Lis_OCR0B;  // PWM duty cycle: OCR0B/OCR0A
// ---Pins---
extern lis_pin Lis_PixSelect; // port2 and ddr2
extern lis_pin Lis_Clk; // port1 and ddr1 (PWM)
extern lis_pin Lis_Rst; // port1 and ddr1
extern lis_pin Lis_Sync; // pin1 and ddr1
// ---Bits---
extern lis_bit Lis_WGM00;  // PWM
extern lis_bit Lis_WGM01;  // PWM
extern lis_bit Lis_WGM02;  // PWM
extern lis_bit Lis_CS00;   // PWM
extern lis_bit Lis_CS01;   // PWM
extern lis_bit Lis_CS02;   // PWM
extern lis_bit Lis_COM0B0; // PWM
extern lis_bit Lis_COM0B1; // PWM
extern lis_bit Lis_OCF0A;  // PWM
extern lis_bit Lis_OCF0B;  // PWM
// ---API---
void LisInit(void);

#endif // _LIS_H
