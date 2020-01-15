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
// ---Private---
inline void Pin_LisPixSelect_SetOutput(void)
{
    SetBit(Lis_ddr2, Lis_PixSelect);
    // ---Expected Assembly---
    // sbi	0x04, 0	; 4
}
inline void LisPixSelectIdleLow(void)
{
    ClearBit(Lis_port2, Lis_PixSelect);
    // ---Expected Assembly---
    // cbi	0x05, 0	; 5
}
inline void Pin_LisClk_SetOutput(void)
{
    SetBit(Lis_ddr1, Lis_Clk);
    // ---Expected Assembly---
    // sbi  0x0a, 5 ; 10
}
inline void Pin_LisRst_SetOutput(void)
{
    SetBit(Lis_ddr1, Lis_Rst);
    // ---Expected Assembly---
    // sbi	0x0a, 6	; 10
}
inline void LisRstIdleLowAfterPowerUp(void)
{
    ClearBit(Lis_port1, Lis_Rst);
    // ---Expected Assembly---
    // cbi  0x0b, 6 ; 11
}
inline void Pin_LisSync_SetInput(void)
{
    ClearBit(Lis_ddr1, Lis_Sync);
    // ---Expected Assembly---
    // cbi	0x0a, 7	; 10
}
inline void ResetPwmTimerAtTop(void)
{
    SetBit(Lis_TCCR0A, Lis_WGM00);
    SetBit(Lis_TCCR0A, Lis_WGM01);
    // ---Expected Assembly---
    // in	r24, 0x24	; 36
    // ori	r24, 0x01	; 1
    // out	0x24, r24	; 36
    // in	r24, 0x24	; 36
    // ori	r24, 0x02	; 2
    // out	0x24, r24	; 36
}
inline void PwmTimerTopIsOCR0A(void)
{
    SetBit(Lis_TCCR0B, Lis_WGM02);
    // ---Expected Assembly---
    // in	r24, 0x25	; 37
    // ori	r24, 0x08	; 8
    // out	0x25, r24	; 37
}
inline void PwmTimerClockedByCpu_NoPrescaling(void)
{
    SetBit(Lis_TCCR0B, Lis_CS00);
    ClearBit(Lis_TCCR0B, Lis_CS01);
    ClearBit(Lis_TCCR0B, Lis_CS02);
    // ---Expected Assembly---
    // Clear bit 1:
    // in	r24, 0x25	; 37
    // andi	r24, 0xFD	; 253
    // out	0x25, r24	; 37
    // Clear bit 2:
    // in	r24, 0x25	; 37
    // andi	r24, 0xFB	; 251
    // out	0x25, r24	; 37
}
inline void LisClkFreq50kHz(void)
{
    ResetPwmTimerAtTop();
    PwmTimerTopIsOCR0A();
    PwmTimerClockedByCpu_NoPrescaling();
    // timer top = 10MHz CPU clock / desired 50kHz PWM frequency
    // 10.0e6/50.0e3 = 200.0 tics
    *Lis_OCR0A = 200;
    // duty cycle = OCR0B/OCR0A
    // 50% = 100/200
    *Lis_OCR0B = 100;
    // ---Expected Assembly---
    // ldi	r24, 0xC8	; 200
    // out	0x27, r24	; 39
    // ldi	r24, 0x64	; 100
    // out	0x28, r24	; 40
}
inline void LisClkOn(void)
{
    ClearBit(Lis_TCCR0A, Lis_COM0B0);
    SetBit(Lis_TCCR0A, Lis_COM0B1);
    // ---Expected Assembly---
    // Clear bit
    // in	r24, 0x24	; 36
    // andi	r24, 0xEF	; 239
    // out	0x24, r24	; 36
    // Set bit
    // in	r24, 0x24	; 36
    // in	r24, 0x24	; 36
    // ori	r24, 0x20	; 32
    // out	0x24, r24	; 36
}
// ---API---
/** \file Lis.h
 * # API
 * void LisInit(void);\n 
 * */
inline void LisInit(void)
{
    /** LisInit behavior:\n 
      * - sets PixSelect as an output\n 
      * - idles PixSelect low\n 
      * - sets Clk as an output\n 
      * - sets Rst as an output\n 
      * - idles Rst low\n 
      * - sets Sync as an input\n 
      * */
    // ---Expected Assembly---
    // assembly for the following calls
    Pin_LisPixSelect_SetOutput();
    LisPixSelectIdleLow();
    Pin_LisClk_SetOutput();
    Pin_LisRst_SetOutput();
    LisRstIdleLowAfterPowerUp();
    Pin_LisSync_SetInput();
    LisClkFreq50kHz();
    LisClkOn();
}

#endif // _LIS_H
