#ifndef _LIS_H
#define _LIS_H

#include <stdint.h>
#include <stdbool.h>

/* =====[ Lis API ]===== */
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
#define LisExposureStart() LisRstHigh()
#define LisExposureStop()  LisRstLow()
#define LisWaitForSyncRiseEdge() do { \
    while(MacroBitIsClear(Lis_pin1, Lis_Sync)); \
} while(0)
#define LisWaitForSyncFallEdge() do { \
    while(MacroBitIsSet(Lis_pin1, Lis_Sync)); \
} while(0)
/* =====[ Lis Frame Readout low-level macros used in API macros ]===== */

#define LisRstHigh() do { \
    MacroSetBit(Lis_port1, Lis_Rst); \
} while(0)
#define LisRstLow() do { \
    MacroClearBit(Lis_port1, Lis_Rst); \
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
void OutputCfgByte(uint8_t const cfg_byte, uint8_t const nbits);
/* =====[ API ]===== */
void LisWriteCfg(uint8_t const * cfg);
/* ----------------------------------------- */

/* TODO: rename Lis_port1 Lis_portio and Lis_port2 Lis_portprog */
#endif // _LIS_H
