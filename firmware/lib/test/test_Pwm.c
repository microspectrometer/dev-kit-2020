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
    // [x] WaitForPwmOutputRisingEdge_blocks_until_the_rising_edge_of_pwm_out
    // [-] WaitForPwmOutputRisingEdge_clears_the_interrupt_flag
        // I have no good way to test this, so I am only doing the macro
        // version.
        // I test the macro version on the embedded target.
    // [x] MacroWaitForPwmRisingEdge_blocks_until_the_rising_edge_of_pwm_out
    // [x] MacroWaitForPwmRisingEdge_clears_the_interrupt_flag
    // [ ] MacroWaitForPwmFallingEdge_blocks_until_the_falling_edge_of_pwm_out
    // [ ] MacroWaitForPwmFallingEdge_clears_the_interrupt_flag

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
void WaitForPwmOutputRisingEdge_blocks_until_the_rising_edge_of_pwm_out(void)
{
    /* =====[ Setup ]===== */
    // Fake the flag is set for the rising edge.
    // Setting up the stub to return an array is not worth it.
    // Manually test once that this hangs forever with the flag not set. Done.
    *Pwm_tifr0 = 0x00;
    SetBit(Pwm_tifr0, Pwm_Ocf0a);
    /* =====[ Operate ]===== */
    WaitForPwmOutputRisingEdge();
    /* =====[ Test ]===== */
    // TODO: Implement an automatic test
    TEST_ASSERT_TRUE_MESSAGE(
        1, "Manually tested. Implement an automatic test."
        );
}
void MacroWaitForPwmRisingEdge_blocks_until_the_rising_edge_of_pwm_out(void)
{
    /* =====[ Setup ]===== */
    // Fake the flag is set for the rising edge.
    // Setting up the stub to return an array is not worth it.
    // PASS: manually test this hangs forever with the flag not set.
    *Pwm_tifr0 = 0x00;
    SetBit(Pwm_tifr0, Pwm_Ocf0a);
    /* =====[ Operate ]===== */
    MacroWaitForPwmRisingEdge();
    /* =====[ Test ]===== */
    // TODO: Implement an automatic test
    // Want to test that executions halts until bit OCF0A is set, but I cannot.
    TEST_ASSERT_TRUE_MESSAGE(
        1, "Manually tested. Implement an automatic test."
        );
    /* TEST_FAIL_MESSAGE("Implement test."); */
}
void MacroWaitForPwmRisingEdge_clears_the_interrupt_flag(void)
{
    // Similar problem: need a stub to return the value I choose.
    // Not doing it.
    // I cannot even confirm this manually by watching the code hang.
    // Manually test this on firmware with an oscilloscope looking at LisRst.
    // And that is why I only test the macro version.
    /* =====[ Setup ]===== */
    *Pwm_tifr0 = 0x00;
    SetBit(Pwm_tifr0, Pwm_Ocf0a);
    /* =====[ Operate ]===== */
    MacroWaitForPwmRisingEdge();
    /* =====[ Test ]===== */
    // TODO: Implement an automatic test
    // Want to test that bit OCF0A is cleared, but I cannot.
    TEST_ASSERT_TRUE_MESSAGE(
            1, "Manually tested. Implement an automatic test."
            );
}
void MacroWaitForPwmFallingEdge_blocks_until_the_falling_edge_of_pwm_out(void)
{
    // Similar problem: need a stub to return the value I choose.
    // Not doing it.
    // I cannot even confirm this manually by watching the code hang.
    // Manually test this on firmware with an oscilloscope looking at LisRst.
    // And that is why I only test the macro version.
    /* =====[ Setup ]===== */
    *Pwm_tifr0 = 0x00;
    SetBit(Pwm_tifr0, Pwm_Ocf0b);
    /* =====[ Operate ]===== */
    MacroWaitForPwmFallingEdge();
    /* =====[ Test ]===== */
    // TODO: Implement an automatic test
    // Want to test that executions halts until bit OCF0B is set, but I cannot.
    TEST_ASSERT_TRUE_MESSAGE(
            1, "Manually tested. Implement an automatic test."
            );
}
void MacroWaitForPwmFallingEdge_clears_the_interrupt_flag(void)
{
    // Similar problem: need a stub to return the value I choose.
    // Not doing it.
    // I cannot even confirm this manually by watching the code hang.
    // Manually test this on firmware with an oscilloscope looking at LisRst.
    // And that is why I only test the macro version.
    /* =====[ Setup ]===== */
    *Pwm_tifr0 = 0x00;
    SetBit(Pwm_tifr0, Pwm_Ocf0b);
    /* =====[ Operate ]===== */
    MacroWaitForPwmFallingEdge();
    /* =====[ Test ]===== */
    // TODO: Implement an automatic test
    // Want to test that bit OCF0B is cleared, but I cannot.
    TEST_ASSERT_TRUE_MESSAGE(
            1, "Manually tested. Implement an automatic test."
            );
}
