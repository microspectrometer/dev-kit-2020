#include "Pwm.h"
#include "ReadWriteBits.h"
static void ResetCounterAtTop(void)
{
    SetBit(Pwm_tccr0a, Pwm_Wgm00);
    SetBit(Pwm_tccr0a, Pwm_Wgm01);
}
void (*PwmResetCounterAtTop)(void) = ResetCounterAtTop;

static void TopIsOcr0a(void)
{
    SetBit(Pwm_tccr0b, Pwm_Wgm02);
}
void (*PwmTopIsOcr0a)(void) = TopIsOcr0a;

static void PwmClkIsCpuClk_Implementation(void)
{
    SetBit  (Pwm_tccr0b, Pwm_Cs00);
    ClearBit(Pwm_tccr0b, Pwm_Cs01);
    ClearBit(Pwm_tccr0b, Pwm_Cs02);
}
void (*PwmClkIsCpuClk)(void) = PwmClkIsCpuClk_Implementation;

static void EnableOutputSetUntilMatch(void)
{
    ClearBit(Pwm_tccr0a, Pwm_Com0b0);
    SetBit  (Pwm_tccr0a, Pwm_Com0b1);
}
void (*PwmEnableOutputSetUntilMatch)(void) = EnableOutputSetUntilMatch;

static void PwmDisableOutput_Implementation(void)
{
    ClearBit(Pwm_tccr0a, Pwm_Com0b0);
    ClearBit(Pwm_tccr0a, Pwm_Com0b1);
}
void (*PwmDisableOutput)(void) = PwmDisableOutput_Implementation;

void WaitForPwmOutputRisingEdge(void)
{
    while(BitIsClear(Pwm_tifr0, Pwm_Ocf0a)); // wait until flag is set
    SetBit(Pwm_tifr0, Pwm_Ocf0a); // set the bit to clear the flag
}

// inline function in .c is OK because application does #include <Pwm.c>
inline void WaitForPwmFallingEdge(void)
{
    SetBit(Pwm_tifr0, Pwm_Ocf0b); // Clear flag PWM falling edge
    // sbi	0x15, 2	; 21
    while(BitIsClear(Pwm_tifr0, Pwm_Ocf0b)); // Block until flag is set
    // sbis	0x15, 2	; 21
    // rjmp	.-4      	;
    SetBit(Pwm_tifr0, Pwm_Ocf0b); // Clear flag PWM falling edge
    // sbi	0x15, 2	; 21
}
inline void WaitForPwmRisingEdge(void)
{

    // clear rising edge flag
    SetBit(Pwm_tifr0, Pwm_Ocf0a); // sbi	0x15, 1	; 21
    // wait until rising edge flag sets
    while(BitIsClear(Pwm_tifr0, Pwm_Ocf0a)); // sbis	0x15, 1	; 21
    // clear rising edge flag
    SetBit(Pwm_tifr0, Pwm_Ocf0a); // sbi	0x15, 1	; 21
}
