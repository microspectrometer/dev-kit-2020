#include "AvrAsmMacros.h"
#include <avr/interrupt.h>      // defines macros `sei()` and `cli()`
#include <util/delay_basic.h>   // defines microsecond delay loop

// sei()
static void GlobalInterruptEnable_Implementation(void)
{ sei(); }
void (*GlobalInterruptEnable)(void) = GlobalInterruptEnable_Implementation;

// cli()
static void GlobalInterruptDisable_Implementation(void)
{ cli(); }
void (*GlobalInterruptDisable)(void) = GlobalInterruptDisable_Implementation;

// _delay_loop_1(uint8_t count)  // execution blocks until count decrements to 0
static void Delay3CpuCyclesPerTick_Implementation(uint8_t ticks)
{ _delay_loop_1(ticks); }
void (*Delay3CpuCyclesPerTick)(uint8_t) = Delay3CpuCyclesPerTick_Implementation;
inline void Delay3ClocksPerTick(uint8_t ticks)
{ _delay_loop_1(ticks); }
