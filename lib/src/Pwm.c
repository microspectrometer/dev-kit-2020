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
