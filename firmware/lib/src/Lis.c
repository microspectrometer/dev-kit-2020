/** \file */
#include "Lis.h"
static void SetPixSelectAsOutput(void)
{
    SetBit(Lis_ddr2, Lis_PixSelect);
    // ---Expected Assembly---
    // sbi	0x04, 0	; 4
}
static void PixSelectIdleLow(void)
{
    ClearBit(Lis_port2, Lis_PixSelect);
    // ---Expected Assembly---
    // cbi	0x05, 0	; 5
}
static void SetClkAsOutput(void)
{
    SetBit(Lis_ddr1, Lis_Clk);
    // ---Expected Assembly---
    // sbi  0x0a, 5 ; 10
}
static void SetRstAsOutput(void)
{
    SetBit(Lis_ddr1, Lis_Rst);
    // ---Expected Assembly---
    // sbi	0x0a, 6	; 10
}
static void RstIdleLowAfterPowerUp(void)
{
    ClearBit(Lis_port1, Lis_Rst);
    // ---Expected Assembly---
    // cbi  0x0b, 6 ; 11
}
static void SetSyncAsInput(void)
{
    ClearBit(Lis_ddr1, Lis_Sync);
    // ---Expected Assembly---
    // cbi	0x0a, 7	; 10
}
static void ResetPwmTimerAtTop(void)
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
static void PwmTimerTopIsOCR0A(void)
{
    SetBit(Lis_TCCR0B, Lis_WGM02);
    // ---Expected Assembly---
    // in	r24, 0x25	; 37
    // ori	r24, 0x08	; 8
    // out	0x25, r24	; 37
}
static void PwmTimerClockedByCpu_NoPrescaling(void)
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
static void LisClkFreq50kHz(void)
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
static void LisClkOn(void)
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
void LisInit(void)
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
    // call	0x21a	; 0x21a <LisInit>
    SetPixSelectAsOutput();
    PixSelectIdleLow();
    SetClkAsOutput();
    SetRstAsOutput();
    RstIdleLowAfterPowerUp();
    SetSyncAsInput();
    LisClkFreq50kHz();
    LisClkOn();
}
