#ifndef _PWM_H
#define _PWM_H

#include <stdint.h>

/* =====[ Pwm API ]===== */
extern void (*PwmResetCounterAtTop)(void);
extern void (*PwmTopIsOcr0a)(void);
extern void (*PwmClkIsCpuClk)(void);
extern void (*PwmEnableOutputSetUntilMatch)(void);

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

#endif // _PWM_H
