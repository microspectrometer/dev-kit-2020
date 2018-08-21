#ifndef _LIS_H
#define _LIS_H

#include <stdint.h>
#include <stdbool.h>

/* =====[ Lis API ]===== */
void LisInit(void);
extern void (*LisRunClkAt50kHz)(void);
extern void (*LisClkOn)(void);
void LisClkOff(void);

/* =====[ Hardware dependencies to be resolved in Lis-Hardware.h ]===== */
/* ---I/O Registers--- */
extern uint8_t volatile * const Lis_ddr1;
extern uint8_t volatile * const Lis_ddr2;
extern uint8_t volatile * const Lis_port1;
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
extern uint8_t const Lis_Sync;      // port1 and ddr1

#endif // _LIS_H
