#ifndef _LIS_H
#define _LIS_H

#include <stdint.h>
#include <stdbool.h>

/* =====[ Lis API ]===== */
#define npixels 784
extern uint8_t binning; extern uint8_t gain; extern uint8_t active_rows;
#define binning_off 0x00
#define binning_on 0x01
// TODO: change type name to match usage or just macro define all of these
typedef uint8_t const config_byte;
// binning_on binning_off MUST be macro-defined because:
// value is needed by inline definition in this file
// but by defining in the header, each translation unit thinks *it* defines it
config_byte gain1x;
config_byte gain25x;
config_byte gain4x;
config_byte gain5x;
config_byte all_rows_active;
inline uint16_t NumPixelsInFrame(void)
{
    /** NumPixelsInFrame behavior:\n 
      * - depends on constant `npixels` equal to 784\n 
      * - returns 784 if binning is off\n 
      * - returns 392 if binning is on\n 
      * */
    uint16_t npixels_in_frame;
    if (binning == binning_on) npixels_in_frame = npixels >> 1;
    else npixels_in_frame = npixels;
    return npixels_in_frame;
}
// LisWriteConfigBitN is also inline but function body is in Lis.c.
// This impacts the build recipes.
// For embedded target:
    // #include Lis.c (not .h)
    // do not link vis-spi-out.o to Lis.o
    // -> function is inline on embedded target
// For unit test target:
    // #include Lis.h
    // link test_runner.o to Lis.o
    // -> function is still inline
    // -> hardware definitions are unknown when Lis.o is built, but it makes no
    // difference since gcc has no special assembly instructions for hardware
    // I/O registers
void LisWriteConfig(uint8_t const *config);
void LisWriteConfigBitN(uint8_t const *config, uint8_t bit_index);

void LisInit(void);
extern void (*LisRunClkAt50kHz)(void);
extern void (*LisClkOn)(void);
void LisClkOff(void);
extern bool Lis_exposure_is_done;
extern uint16_t Lis_nticks_exposure;
/* void LisExpose(uint16_t nticks); */
/* =====[ Lis Frame Readout Macro API ]===== */
#define LisWaitForClkRiseEdge()  MacroWaitForPwmRisingEdge()
#define LisWaitForClkFallEdge() MacroWaitForPwmFallingEdge()
/* #define LisExposureStart() LisRstHigh() */
/* #define LisExposureStop()  LisRstLow() */
/* #define LisWaitForSyncRiseEdge() do { \ */
/*     while(BitIsClear(Lis_pin1, Lis_Sync)); \ */
/* } while(0) */
#define LisWaitForSyncFallEdge() do { \
    while(BitIsSet(Lis_pin1, Lis_Sync)); \
} while(0)
/* =====[ Lis Frame Readout low-level macros used in API macros ]===== */

#define LisRstHigh() do { \
    SetBit(Lis_port1, Lis_Rst); \
} while(0)
#define LisRstLow() do { \
    ClearBit(Lis_port1, Lis_Rst); \
} while(0)

/* =====[ Hardware dependencies to be resolved in Lis-Hardware.h ]===== */
/* ---I/O Registers--- */
extern uint8_t volatile * const Lis_ddr1;
extern uint8_t volatile * const Lis_ddr2;
extern uint8_t volatile * const Lis_port1;
extern uint8_t volatile * const Lis_pin1;
extern uint8_t volatile * const Lis_port2;
/* =====[ Use 8-bit PWM for Lis clock ]===== */
extern uint8_t volatile * const Lis_clktop;  // sets PWM period in ticks
extern uint8_t volatile * const Lis_clkth;  // sets PWM time high in ticks
// PWM frequency = fcpu/Lis_clktop
// PWM duty cycle = Lis_clkth/Lis_clktop
/* ---Pin Names--- */
extern uint8_t const Lis_PixSelect; // port2 and ddr2
extern uint8_t const Lis_Clk;       // port1 and ddr1
extern uint8_t const Lis_Rst;       // port1 and ddr1
extern uint8_t const Lis_Sync;      // pin1 and ddr1

/* ----------------------------------------- */
/* | 2019-03-01: refactoring WriteCfgToLis | */
/* ----------------------------------------- */
/* =====[ Internal helper functions. Exposed for testing. Not part of API ]===== */
void EnterLisProgrammingMode(void);
void ExitLisProgrammingMode(void);

/* TODO: rename Lis_port1 Lis_portio and Lis_port2 Lis_portprog */

// ---2019-11-12 correct use of inline for optimized assembly code---
void LisWaitForClockRisingEdge(void);
void LisWaitForClockFallingEdge(void);
void LisWaitForSyncRisingEdge(void);
void LisWaitForSyncFallingEdge(void);
void ExposePhotodiodeArray(uint16_t exposure_ticks);
void LisStartExposure(void);
void LisStopExposure(void);
void LisWaitForReadoutToStart(void);
void LisReadout(void);

#endif // _LIS_H
