#include "test_Pwm.h"
#include "Pwm.h"                // lib under test
#include "fake/Pwm-Hardware.h"  // fake hardware dependencies in Pwm.h
#include <unity.h>              // unit testing framework
#include "ReadWriteBits.h"
/* =====[ List of Tests ]===== */
    // [x] PwmResetCounterAtTop_configures_PWM_for_fast_PWM_mode
    // [x] PwmTopIsOcr0a_uses_the_value_in_OCR0A_for_TOP
    // [x] PwmClkIsCpuClk_uses_the_cpu_clock_with_no_prescaler
    // [x] PwmEnableOutputSetUntilMatch_sets_OC0B_at_bottom_and_clears_on_match
    // [x] PwmDisableOutput_disconnects_OC0B_and_restores_normal_io_output

void PwmResetCounterAtTop_configures_PWM_for_fast_PWM_mode(void)
{
    /* =====[ Operate ]===== */
    PwmResetCounterAtTop();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH( Pwm_Wgm00, *Pwm_tccr0a);
    TEST_ASSERT_BIT_HIGH( Pwm_Wgm01, *Pwm_tccr0a);
}
void PwmTopIsOcr0a_uses_the_value_in_OCR0A_for_TOP(void)
{
    /* =====[ Operate ]===== */
    PwmTopIsOcr0a();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH( Pwm_Wgm02, *Pwm_tccr0b);
}
void PwmClkIsCpuClk_uses_the_cpu_clock_with_no_prescaler(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Pwm_tccr0b, Pwm_Cs00);
    SetBit(Pwm_tccr0b, Pwm_Cs01);
    SetBit(Pwm_tccr0b, Pwm_Cs02);
    /* =====[ Operate ]===== */
    PwmClkIsCpuClk();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH( Pwm_Cs00, *Pwm_tccr0b);
    TEST_ASSERT_BIT_LOW( Pwm_Cs01, *Pwm_tccr0b);
    TEST_ASSERT_BIT_LOW( Pwm_Cs02, *Pwm_tccr0b);
}
void PwmEnableOutputSetUntilMatch_sets_OC0B_at_bottom_and_clears_on_match(void)
{
    /* =====[ Setup ]===== */
    SetBit(Pwm_tccr0a, Pwm_Com0b0);
    ClearBit(Pwm_tccr0a, Pwm_Com0b1);
    /* =====[ Operate ]===== */
    PwmEnableOutputSetUntilMatch();
    TEST_ASSERT_BIT_LOW( Pwm_Com0b0, *Pwm_tccr0a);
    TEST_ASSERT_BIT_HIGH( Pwm_Com0b1, *Pwm_tccr0a);
}
void PwmDisableOutput_disconnects_OC0B_and_restores_normal_io_output(void)
{
    /* =====[ Setup ]===== */
    SetBit(Pwm_tccr0a, Pwm_Com0b0);
    SetBit(Pwm_tccr0a, Pwm_Com0b1);
    /* =====[ Operate ]===== */
    PwmDisableOutput();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW( Pwm_Com0b0, *Pwm_tccr0a);
    TEST_ASSERT_BIT_LOW( Pwm_Com0b1, *Pwm_tccr0a);
}
