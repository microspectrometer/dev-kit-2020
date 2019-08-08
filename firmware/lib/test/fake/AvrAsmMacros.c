#include "AvrAsmMacros.h"
/* =====[ Fake the AVR asm macros with empty definitions ]===== */
// sei()
static void GlobalInterruptEnable_Implementation(void) {}
void (*GlobalInterruptEnable)(void) = GlobalInterruptEnable_Implementation;
//
// cli()
static void GlobalInterruptDisable_Implementation(void) {}
void (*GlobalInterruptDisable)(void) = GlobalInterruptDisable_Implementation;
//
// _delay_loop_1(uint8_t count)  // execution blocks until count decrements to 0
static void Delay3CpuCyclesPerTick_Implementation(uint8_t ticks){ ticks--; }
void (*Delay3CpuCyclesPerTick)(uint8_t) = Delay3CpuCyclesPerTick_Implementation;
