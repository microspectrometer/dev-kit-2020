#ifndef _AVR_ASM_MACROS_H
#define _AVR_ASM_MACROS_H

#include <stdint.h>  // uint8_t

extern void (*GlobalInterruptEnable)(void);
extern void (*GlobalInterruptDisable)(void);
extern void (*Delay3CpuCyclesPerTick)(uint8_t);

#endif // _AVR_ASM_MACROS_H
