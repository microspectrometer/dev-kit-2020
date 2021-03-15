#ifndef _S13131_H
#define _S13131_H
#include <stdint.h>
#include <ReadWriteBits.h>
#ifdef USE_FAKES
#include "S13131_faked.h" // declare fakes
#endif

/** \file S13131.h
 * # API
 * void S13131PinSetup(void);\n 
 * void S13131StartClocking(void);\n
 * void S13131Expose(void);\n
 * S13131Readout is defined in VisCmd\n
 * */

/** S13131 exposure time
 * - exposure is a 16-bit word
 * - exposure is in units of clock ticks
 * - multiply exposure by 20e-6 seconds to get exposure time in seconds
 *   - one clock tick is 20e-6 seconds because the S13131 is clocked at 50kHz
 * - minimum allowed exposure time is 180e-6 seconds (exposure_ticks = 9)
 * - exposure_ticks is clamped at 9 ticks
 *   - exposure_ticks is set to 9 if SetExposure attempts a value < 9
 * */
extern uint16_t exposure_ticks;
//! S13131-512 has 512 pixels
#define MAX_NUM_PIXELS 512

// TODO(sustainablelab): eliminate redundant MSB/LSB definitions in S13131.h and LIS.h
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

typedef uint8_t volatile * const s13131_ptr; // reg address type
typedef uint8_t const s13131_pin; // pin-number type
typedef uint8_t const s13131_bit; // bit-index type

// Register address, pin number, and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h -> S13131-HardwareFake.h
    // "avr-gcc" uses src/S13131-Hardware.h
// ---Registers---
extern s13131_ptr S13131_ddr;
extern s13131_ptr S13131_port;
extern s13131_ptr S13131_pin; // PortINput reg, NOT pin-number
extern s13131_ptr S13131_TCCR0A; // PWM timer 0 ctrl reg A
extern s13131_ptr S13131_TCCR0B; // PWM timer 0 ctrl reg B
extern s13131_ptr S13131_TIFR0;  // PWM timer 0 interrupt flags
extern s13131_ptr S13131_OCR0A;  // PWM period: fcpu/OCR0A
extern s13131_ptr S13131_OCR0B;  // PWM duty cycle: OCR0B/OCR0A

// ---Pins---
extern s13131_pin S13131_Clk; // port and ddr (PWM)
extern s13131_pin S13131_St;  // port and ddr
extern s13131_pin S13131_Eos; // pin and ddr

// ---Bits---
extern s13131_bit S13131_WGM00;  // PWM
extern s13131_bit S13131_WGM01;  // PWM
extern s13131_bit S13131_WGM02;  // PWM
extern s13131_bit S13131_CS00;   // PWM
extern s13131_bit S13131_CS01;   // PWM
extern s13131_bit S13131_CS02;   // PWM
extern s13131_bit S13131_COM0B0; // PWM
extern s13131_bit S13131_COM0B1; // PWM
extern s13131_bit S13131_OCF0A;  // PWM
extern s13131_bit S13131_OCF0B;  // PWM

// ---API---
inline void S13131PinSetup(void)
{
    // S13131_Clk is an OUTPUT pin
    SetBit(S13131_ddr, S13131_Clk); // sbi 0xa,5
    // S13131_CLK idles LOW
    ClearBit(S13131_port, S13131_Clk); // cbi 0xb,5

    // S13131_St is an OUTPUT pin
    SetBit(S13131_ddr, S13131_St); // sbi 0xa,6
    // S13131_ST idles LOW
    ClearBit(S13131_port, S13131_St); // cbi 0xb,6

    // S13131_Eos is an INPUT pin
    ClearBit(S13131_ddr, S13131_Eos); // cbi 0xa,7
    // Disable internal pull-up resistor on S13131_Eos
    ClearBit(S13131_port, S13131_Eos); // cbi 0xb,7
}

inline void S13131StartClocking(void)
{
    // S13131_Clk is a PWM pin
    // Output a 50kHz clock

    // ---ResetPwmTimerAtTop---
    SetBit(S13131_TCCR0A, S13131_WGM00);
    // in r24,0x24
    // ori r24,lo8(1) ; WGM00 is bit 0
    // out 0x24,r24
    //
    SetBit(S13131_TCCR0A, S13131_WGM01);
    // in r24,0x24
    // ori r24,lo8(2) ; WGM01 is bit 1
    // out 0x24,r24
    //
    // PwmTimerTopIsOCR0A
    SetBit(S13131_TCCR0B, S13131_WGM02);
    // in r24,0x25
    // ori r24,lo8(8) ; WGM02 is bit 3
    // out 0x25,r24

    // ---PwmTimerClockedByCpu_NoPrescaling---
    SetBit(S13131_TCCR0B, S13131_CS00);
    // in r24,0x25
    // ori r24,lo8(1) ; CS00 is bit 0
    // out 0x25,r24
    //
    ClearBit(S13131_TCCR0B, S13131_CS01);
    // in r24,0x25
    // andi r24,lo8(-3) ; CS01 is bit 1 and -3 is 0b 1111 1101
    // out 0x25,r24
    //
    ClearBit(S13131_TCCR0B, S13131_CS02);
    // in r24,0x25
    // andi r24,lo8(-5) ; CS02 is bit 2 and -5 is 0b 1111 1011
    // out 0x25,r24

    // ---Set50kHzClockFreq---
    // Timer top = 10MHz CPU clock / desired 50kHz PWM frequency
    // 10.0e6/50.0e3 = 200.0 tics
    *S13131_OCR0A = 200;
    // ldi r24,lo8(-56) ; 256-56 is 200
    // out 0x27,r24

    // ---Set 50% duty cycle---
    // duty cycle = OCR0B/OCR0A
    // 50% = 100/200
    *S13131_OCR0B = 100;
    // ldi r24,lo8(100)
    // out 0x28,r24

    // ---Turn on the Clock Signal---
    // Clock goes HIGH when timer restarts, and LOW when timer hits 100.
    ClearBit(S13131_TCCR0A, S13131_COM0B0);
    // in r24,0x24
    // andi r24,lo8(-17) ; COM0B0 is bit 3 and -17 is 0b 1110 1111
    // out 0x24,r24
    SetBit(S13131_TCCR0A, S13131_COM0B1);
    // in r24,0x24
    // ori r24,lo8(32) ; COM0B1 is bit 5
    // out 0x24,r24
}

inline void WaitForS13131ClkLow(void)
{
    // Clear flag that is set when Counter0 matches OCR0B
    // SET BIT to clear flag in TIFR0
    SetBit(S13131_TIFR0, S13131_OCF0B);
    // set bit to clear flag: sbi	0x15, 2

    // Wait for flag to set again
    while(BitIsClear(S13131_TIFR0, S13131_OCF0B));
    // .L2:
    //     sbis 0x15,2
    //     rjmp .L2
}

inline void WaitForS13131ClkHigh(void)
{
    // Clear flag that is set when Counter0 matches OCR0A
    // SET BIT to clear flag in TIFR0
    SetBit(S13131_TIFR0, S13131_OCF0A);
    // sbi 0x15,1

    // Wait for flag to set again
    while(BitIsClear(S13131_TIFR0, S13131_OCF0A));
    //.L7:
    //    sbis 0x15,1
    //    rjmp .L7
}

inline void S13131Expose(void)
{
    // Expose S13131.
    // Exposure time is set by global variable exposure_ticks.

    // -------------------------------------------------
    // TODO(sustainablelab): move this clamp code to response to VisCmd.SetExposure.
    // Clamp exposure_ticks to minimum allowed value.
    if (exposure_ticks < 9)
        exposure_ticks = 9;
    // -------------------------------------------------

    // Compensate for the 6 cycles S13131 adds to exposure set by ST
    exposure_ticks -= 6;

    // ---ONLY TRANSITION ST AFTER A CLK FALLING EDGE---

    // STATE: CLK is running and ST is idling LOW
    WaitForS13131ClkLow();
    SetBit(S13131_port, S13131_St); // sbi 0xb,6

    // Count CLK rising edges until count == exposure_ticks
    for(uint16_t r_count = 0; r_count < exposure_ticks; r_count++)
    {
        WaitForS13131ClkHigh();

        // 0  < 4: Wait for CLK rising-edge (r_count 0->1)
        // 1  < 4: Wait for CLK rising-edge (r_count 1->2)
        // 2  < 4: Wait for CLK rising-edge (r_count 2->3)
        // There have been three rising edges so far.
        // STATE: Exposure Starts

        // 3  < 4: Wait for CLK rising-edge (r_count 3->4)
        // ...
        // Eventually r_count == exposure_ticks!
        // -> r_count !< exposure_ticks: Exit loop
        // Total of number of rising edges so far == r_count == exposure_ticks
        // Exposed for a total of (r_count - 3) clock cycles so far.
        // Loop exits *just after* the rising edge where r_count == exposure_ticks.
    }

    WaitForS13131ClkLow();
    ClearBit(S13131_port, S13131_St); // cbi 0xb,6
    // STATE: Exposure will stop 9 clock rising-edges later

    // Clock-in ST LOW
    WaitForS13131ClkHigh(); // (1st of the 9 rising edges)

    // Start the falling-edge count-down to pixel readout.
    // Exit this count-down just after the 13th clock falling edge.
    for(uint16_t f_count = 0; f_count < (14-1); f_count++)
    {
        // Loop enters *just after* 5th rising edge.
        WaitForS13131ClkLow();
        //  0  < 13: Wait for CLK falling-edge (f_count  0 ->  1)
        //  1  < 13: Wait for CLK falling-edge (f_count  1 ->  2)
        //  ...
        // 12  < 13: Wait for CLK falling-edge (f_count 12 -> 13)
        // 13  < 13: Exit loop
    }

    // STATE: Readout pixel 1 on next falling edge of CLK
}

#endif // _S13131_H
