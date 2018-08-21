#ifndef _MOCK_PWM_H
#define _MOCK_PWM_H

void Expect_PwmResetCounterAtTop(void);
void PwmResetCounterAtTop_Stubbed(void);
void Expect_PwmTopIsOcr0a(void);
void PwmTopIsOcr0a_Stubbed(void);
void Expect_PwmClkIsCpuClk(void);
void PwmClkIsCpuClk_Stubbed(void);
void Expect_PwmEnableOutputSetUntilMatch(void);
void PwmEnableOutputSetUntilMatch_Stubbed(void);
void Expect_PwmDisableOutput(void);
void PwmDisableOutput_Stubbed(void);

#endif // _MOCK_PWM_H
