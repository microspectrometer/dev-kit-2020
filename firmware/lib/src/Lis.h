#ifndef _LIS_H
#define _LIS_H
#include <stdint.h>
#include "ReadWriteBits.h"
#include "LisConfig.h"
#ifdef USE_FAKES
#include "Lis_faked.h" // declare fakes
#endif
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
    // Lis_TCCR0A is 0x24: expect direct reg-access in/out
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
    /** Pin LisClk outputs a 50kHz square wave.\n 
     * */
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
    // OCR0A is 0x27, OCR0B is 0x28: expect "out" writes to reg
    // ldi	r24, 0xC8	; 200
    // out	0x27, r24	; 39
    // ldi	r24, 0x64	; 100
    // out	0x28, r24	; 40
}
inline void LisClkOn(void)
{
    /** Pin LisClk goes HIGH when Counter0 is 0.\n 
     *  Pin LisClk goes LOW when Counter0 matches OCR0B.
     * */
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
inline void _ConfigAs28bits(uint8_t *config)
{
    /** ConfigAs28bits behavior:\n 
      * - writes config as little endian ie binning is config byte0 bit0\n 
      * - sets config byte0 bit0 if BINNING ON\n 
      * - clears config byte0 bit0 if BINNING OFF\n 
      * - byte0 bit1 clear and bit2 clear if GAIN 1X\n 
      * - byte0 bit1 clear and bit2 set if GAIN 2X5\n 
      * - byte0 bit1 set and bit2 clear if GAIN 4X\n 
      * - byte0 bit1 set and bit2 set if GAIN 5X\n 
      * - bit3 to bit27 set if ALL ROWS ACTIVE\n 
      * - b3b8b13b18b23 set if ROW 1 ACTIVE\n 
      * - b4b9b14b19b24 set if ROW 2 ACTIVE\n 
      * - b5b10b15b20b25 set if ROW 3 ACTIVE\n 
      * - b6b11b16b21b26 set if ROW 4 ACTIVE\n 
      * - b7b12b17b22b27 set if ROW 5 ACTIVE\n 
      * */

    // ---Expected Assembly---
    // Y is r28 and r29
    // std (store indirect with displacement)
    // std Y+1, r24 -- stores value in r24 at address in Y plus 1
    // The following stores the 4-byte config at the address in Y:
    // config byte 0 is 0xF9: 11111001 (bits 7 to 0)
    // config byte 1 is 0xFF: 11111111 (bits 15 to 8)
    // config byte 2 is 0xFF: 11111111 (bits 23 to 16)
    // config byte 3 is 0x0F: 00001111 (bits 31 to 24)
    // ldi	r24, 0xF9	; 249
    // std	Y+1, r24	; 0x01
    // ldi	r24, 0xFF	; 255
    // std	Y+2, r24	; 0x02
    // std	Y+3, r24	; 0x03
    // ldi	r24, 0x0F	; 15
    // std	Y+4, r24	; 0x04

    // Clear all bits in array at input address `config`.
    config[0]=0x00; config[1]=0x00; config[2]=0x00; config[3]=0x00;
    // binning is bit 0 of byte 0
    uint8_t bit = 0;
    if (BINNING_ON == binning) config[0] |= 1<<(bit++);
    else bit++;
    // bit 1 of byte 0 is datasheet "gain bit G2"
    // bit 2 of byte 0 is datasheet "gain bit G1"
    // {G2,G1}: {0,0} 1x; {0,1} 2.5x; {1,0} 4x; {1,1} 5x
    if      (GAIN_2X5 == gain) { bit++; config[0] |= 1<<(bit++); }
    else if (GAIN_4X == gain)  { config[0] |= 1<<(bit++); bit++; }
    else if (GAIN_5X == gain)  { config[0] |= 1<<(bit++); config[0] |= 1<<(bit++); }
    else { bit++; bit++; }
    // bit 3 to 27 are pixel groups P25 to P1 to select active rows
    // Example with binning_on and gain1x
    // ----3----  ----2----  ----1----  ----0-(---) // byte
    // 7654 3210  7654 3210  7654 3210  7654 3(210) // bit
    // xxxx 1111  1111 1111  1111 1111  1111 1(001) // all rows on
    // xxxx 0000  1000 0100  0010 0001  0000 1(001) // row 1 (or 5?)
    // xxxx 0001  0000 1000  0100 0010  0001 0(001) // row 2 (or 4?)
    // xxxx 0010  0001 0000  1000 0100  0010 0(001) // row 3
    // xxxx 0100  0010 0001  0000 1000  0100 0(001) // row 4 (or 2?)
    // xxxx 1000  0100 0010  0001 0000  1000 0(001) // row 5 (or 1?)
    uint8_t const row1 = 0; uint8_t const row1_mask[] = {0x00,0x84,0x21,0x08};
    uint8_t const row2 = 1; uint8_t const row2_mask[] = {0x01,0x08,0x42,0x10};
    uint8_t const row3 = 2; uint8_t const row3_mask[] = {0x02,0x10,0x84,0x20};
    uint8_t const row4 = 3; uint8_t const row4_mask[] = {0x04,0x21,0x08,0x40};
    uint8_t const row5 = 4; uint8_t const row5_mask[] = {0x08,0x42,0x10,0x80};
    // byte orders are mirrored below because
    // rowN_mask[] is big endian, but
    // config[] is little endian
    if (active_rows&(1<<row1))
    {
        config[0] |= row1_mask[3];
        config[1] |= row1_mask[2];
        config[2] |= row1_mask[1];
        config[3] |= row1_mask[0];
    }
    if (active_rows&(1<<row2))
    {
        config[0] |= row2_mask[3];
        config[1] |= row2_mask[2];
        config[2] |= row2_mask[1];
        config[3] |= row2_mask[0];
    }
    if (active_rows&(1<<row3))
    {
        config[0] |= row3_mask[3];
        config[1] |= row3_mask[2];
        config[2] |= row3_mask[1];
        config[3] |= row3_mask[0];
    }
    if (active_rows&(1<<row4))
    {
        config[0] |= row4_mask[3];
        config[1] |= row4_mask[2];
        config[2] |= row4_mask[1];
        config[3] |= row4_mask[0];
    }
    if (active_rows&(1<<row5))
    {
        config[0] |= row5_mask[3];
        config[1] |= row5_mask[2];
        config[2] |= row5_mask[1];
        config[3] |= row5_mask[0];
    }
}
inline void _WaitForLisClkLow(void)
{
    /** WaitForLisClkLow behavior:\n 
      * - clears flag PwmTimerMatchesOCF0B\n 
      * - waits until flag PwmTimerMatchesOCF0B is set\n 
      * */
    // Clear flag that is set when Counter0 matches OCR0B
    SetBit(Lis_TIFR0, Lis_OCF0B); // set bit to clear flag: sbi	0x15, 2
    // Wait for flag to set again
    while(BitIsClear(Lis_TIFR0, Lis_OCF0B)); // sbis	0x15, 2
}
inline void _WaitForLisClkHigh(void)
{
    /** WaitForLisClkHigh behavior:\n 
      * - clears flag PwmTimerMatchesOCF0A\n 
      * - waits until flag PwmTimerMatchesOCF0A is set\n 
      * */
    // Clear flag that is set when Counter0 matches OCR0A
    SetBit(Lis_TIFR0, Lis_OCF0A);
    // Wait for flag to set again
    while(BitIsClear(Lis_TIFR0, Lis_OCF0A));
}

#ifdef USE_FAKES
#define _WaitForLisClkLow _WaitForLisClkLow_fake
#endif // USE_FAKES
inline void _EnterLisProgrammingMode(void)
{
    /** EnterLisProgrammingMode behavior:\n 
      * - waits for LisClk LOW\n 
      * - asserts LisPixSelect to program Lis\n 
      * */
    // Total number of cycles: 8
    // Total number of instructions: 4
    _WaitForLisClkLow();
    // ---Expected Assembly---
    //  708:	sbi	0x15, 2	; 21
    //  70a:	sbis	0x15, 2	; 21
    //  70c:	rjmp	.-4      	; 0x70a
    SetBit(Lis_port2, Lis_PixSelect);
    // ---Expected Assembly---
    //  70e:	sbi	0x05, 0	; 5
}
#ifdef USE_FAKES
#undef _WaitForLisClkLow
#endif // USE_FAKES

inline void _ExitLisProgrammingMode(void)
{
    /** ExitLisProgrammingMode behavior:\n 
      * - outputs LOW on pin LisRst\n 
      * - outputs LOW on pin LisPixSelect\n 
      * */
    // Total number of cycles: 4
    // Total number of instructions: 2
    ClearBit(Lis_port1, Lis_Rst);
    // ---Expected Assembly---
    //  788:	cbi	0x0b, 6	; 11
    ClearBit(Lis_port2, Lis_PixSelect);
    // ---Expected Assembly---
    //  78a:	cbi	0x05, 0	; 5
}

#ifdef USE_FAKES
#define _WaitForLisClkLow _WaitForLisClkLow_fake
#define _WaitForLisClkHigh _WaitForLisClkHigh_fake
#endif // USE_FAKES
inline void _WriteLisConfigBit(uint8_t const * config, uint8_t bit_index)
{
    /** **WriteLisConfigBit** writes one bit of the LIS 28-bit
     * programming sequence.
     *
     * To **write one bit**:
     * - output bit value on pin `Lis_Rst`
     * - clock the LIS
     *
     * Input parameters:
     * - `config` points to one of the four config bytes
     * - `bit_index` is the bit to write (bit7:0) from the
     *   `config` byte
     * */
    /** WriteLisConfigBit behavior:\n 
      * - outputs bit on LisRst\n 
      * - waits for LisClk HIGH\n 
      * - waits for LisClk LOW\n 
      * */
    // Set up pin `Lis_Rst` with value to write
    BitIsSet((uint8_t *)config, bit_index) ?
        SetBit(Lis_port1, Lis_Rst)
        :
        ClearBit(Lis_port1, Lis_Rst);
    // Clock in the value
    _WaitForLisClkHigh();
    _WaitForLisClkLow();
}
#ifdef USE_FAKES
#undef _WaitForLisClkLow
#undef _WaitForLisClkHigh
#endif // USE_FAKES

#ifdef USE_FAKES
#define _WriteLisConfigBit _WriteLisConfigBit_fake
#endif // USE_FAKES
inline void _Write28bitLisConfig(uint8_t const *config)
{
    /** Write28bitLisConfig behavior:\n 
      * - writes 28bits starting at byte0 bit0 and ending at byte3 bit3\n 
      * */
    // Write all bits in the first three bytes of config
    for (uint8_t cfg_byte_i = 0; cfg_byte_i < 3; cfg_byte_i++)
    {
        for (uint8_t bit_i = 0; bit_i<8; bit_i++)
        {
            _WriteLisConfigBit(config, bit_i);
        }
        config++;
    }
    // Write first four bits of last byte of config
    uint8_t bit_i = 0;
    while(bit_i < 4) _WriteLisConfigBit(config, bit_i++);
}
#ifdef USE_FAKES
#undef _WriteLisConfigBit
#endif // USE_FAKES

// ---API---
/** \file Lis.h
 * # API
 * void LisInit(void);\n 
 * */

/** LIS-770i exposure time
 * - exposure is a 16-bit word
 * - exposure is in units of clock ticks
 * - multiply exposure by 20e-6 seconds to get exposure time in seconds
 *   - one clock tick is 20e-6 seconds because the LIS-770i is clocked at 50kHz
 * */
uint16_t exposure_ticks;
/** LIS-770i maximum number of pixels
 * - with binning *off* there are at most 784 pixels
 * - this includes the 13 optically dark and one dummy pixel
 * - with binning *on* there are at most 392 pixels
 * */
#define MAX_NUM_PIXELS 784
inline uint8_t MSB(uint16_t msb_lsb)
{
    /** MSB behavior:\n 
      * - returns most significant bit of 16bit input\n 
      * */
    return msb_lsb >> 8;
}
inline uint8_t LSB(uint16_t msb_lsb)
{
    /** LSB behavior:\n 
      * - returns least significant bit of 16bit input\n 
      * */
    return msb_lsb & 0xFF;
}

inline void LisInit(void)
{
    /** LisInit behavior:\n 
      * - sets PixSelect as an output\n 
      * - idles PixSelect low\n 
      * - sets Rst as an output\n 
      * - idles Rst low\n 
      * - sets Sync as an input\n 
      * - sets Clk as an output\n 
      * - resets PWM timer at top\n 
      * - PWM timer top is OCR0A\n 
      * - PWM timer is clocked by CPU with no prescaling\n 
      * - sets PWM frequency at 50kHz\n 
      * - sets PWM duty cycle to 50 percent\n 
      * - outputs the PWM clock on pin Clk\n 
      * */
    Pin_LisPixSelect_SetOutput();
    LisPixSelectIdleLow();
    Pin_LisClk_SetOutput();
    Pin_LisRst_SetOutput();
    LisRstIdleLowAfterPowerUp();
    Pin_LisSync_SetInput();
    LisClkFreq50kHz();
    LisClkOn();
}
inline bool LisConfigIsValid(
    uint8_t binning,
    uint8_t gain,
    uint8_t active_rows)
{
    /** LisConfigIsValid behavior:\n 
      * - returns false if binning is invalid\n 
      * - returns false if gain is invalid\n 
      * - returns false if active rows is invalid\n 
      * - returns true if config is valid\n 
      * */
    return
        (
            (binning == BINNING_OFF) || (binning == BINNING_ON)
        )
        &&
        (
            (gain == GAIN_1X)  ||
            (gain == GAIN_2X5) ||
            (gain == GAIN_4X)  ||
            (gain == GAIN_5X)
        )
        &&
        (
            (active_rows & 0xE0) == 0x00
        );
}

#ifdef USE_FAKES
#define _ConfigAs28bits _ConfigAs28bits_fake
#define _EnterLisProgrammingMode _EnterLisProgrammingMode_fake
#define _Write28bitLisConfig _Write28bitLisConfig_fake
#define _ExitLisProgrammingMode _ExitLisProgrammingMode_fake
#endif // USE_FAKES
inline void LisWriteConfig(void)
{
    /** LisWriteConfig behavior:\n 
      * - converts config to 28bit sequence\n 
      * - enters LIS programming mode\n 
      * - writes 28bits to LIS setup register\n 
      * - exits LIS programming mode\n 
      * */
    // Store global lis_cfg parameters as a 28-bit config
    uint8_t config[4]; // memory for 28-bit config
    _ConfigAs28bits(config);
    // Program LIS with 28-bit config
    _EnterLisProgrammingMode();
    _Write28bitLisConfig(config);
    _ExitLisProgrammingMode();
}
#ifdef USE_FAKES
#undef _ConfigAs28bits
#undef _EnterLisProgrammingMode
#undef _Write28bitLisConfig
#undef _ExitLisProgrammingMode
#endif // USE_FAKES

inline void LisExpose(void)
{
    /** LisExpose behavior:\n 
      * - waits for the falling edge of Lis_Clk\n 
      * - starts exposure by driving Lis_Rst HIGH\n 
      * - counts falling edges of Lis_Clk until count equals exposure_ticks\n 
      * - stops exposure by driving Lis_Rst LOW\n 
      * */
    // wait for the falling clock edge
    _WaitForLisClkLow();

    // start exposure
    SetBit(Lis_port1, Lis_Rst); // sbi	0x0b, 6

    // count falling edges as ticks
    uint16_t tick_count = 0;
    while (tick_count++ < exposure_ticks)
    {
        // wait for the falling clock edge
        _WaitForLisClkLow();
    }

    // stop exposure
    ClearBit(Lis_port1, Lis_Rst); // cbi	0x0b, 6
}

#endif // _LIS_H
