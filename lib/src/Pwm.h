#ifndef _PWM_H
#define _PWM_H

#include <stdint.h>
#include "ReadWriteBits.h"

/* =====[ Pwm API ]===== */
extern void (*PwmResetCounterAtTop)(void);
extern void (*PwmTopIsOcr0a)(void);
extern void (*PwmClkIsCpuClk)(void);
extern void (*PwmEnableOutputSetUntilMatch)(void);
/* =====[ Pwm low-level macros used in Pwm API macros ]===== */
#define MacroBlockUntilFlagPwmRisingEdgeIsSet() do { \
    while(BitIsClear(Pwm_tifr0, Pwm_Ocf0a)); \
} while(0)
#define MacroBlockUntilFlagPwmFallingEdgeIsSet() do { \
    while(BitIsClear(Pwm_tifr0, Pwm_Ocf0b)); \
} while(0)
#define MacroClearFlagPwmRisingEdge() do { \
    SetBit(Pwm_tifr0, Pwm_Ocf0a); \
} while (0)
#define MacroClearFlagPwmFallingEdge() do { \
    SetBit(Pwm_tifr0, Pwm_Ocf0b); \
} while (0)
/* =====[ Pwm API macros ]===== */
#define MacroWaitForPwmRisingEdge() do { \
    MacroClearFlagPwmRisingEdge(); \
    MacroBlockUntilFlagPwmRisingEdgeIsSet(); \
    MacroClearFlagPwmRisingEdge(); \
} while(0)
#define MacroWaitForPwmFallingEdge() do { \
    MacroClearFlagPwmFallingEdge(); \
    MacroBlockUntilFlagPwmFallingEdgeIsSet(); \
    MacroClearFlagPwmFallingEdge(); \
} while(0)
/* =====[ For developing Pwm API ]===== */
void WaitForPwmOutputRisingEdge(void); // use `Macro` version instead

/* =====[ Hardware dependencies to be resolved in Pwm-Hardware.h ]===== */
/* ---I/O Registers--- */
extern uint8_t volatile * const Pwm_tccr0a; // timer0 control reg A
extern uint8_t volatile * const Pwm_tccr0b; // timer0 control reg B
/* ---Bit Names--- */
extern uint8_t const Pwm_Wgm00;  // tccr0a
extern uint8_t const Pwm_Wgm01;  // tccr0a
extern uint8_t const Pwm_Wgm02;  // tccr0b
extern uint8_t const Pwm_Cs00;   // tccr0b
extern uint8_t const Pwm_Cs01;   // tccr0b
extern uint8_t const Pwm_Cs02;   // tccr0b
extern uint8_t const Pwm_Com0b0; // tccr0a
extern uint8_t const Pwm_Com0b1; // tccr0a
extern void (*PwmDisableOutput)(void);
extern uint8_t volatile * const Pwm_tifr0;  // timer0 interrupt flags
extern uint8_t const Pwm_Ocf0a; // tifr0
extern uint8_t const Pwm_Ocf0b; // tifr0
#endif // _PWM_H
