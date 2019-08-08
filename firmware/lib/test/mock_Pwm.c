#include "mock_Pwm.h"
#include <Mock.h>
#include <RecordedCall.h>
#include <RecordedArg.h>

static RecordedCall * Mock_PwmResetCounterAtTop(void)
{
    char const *call_name = "PwmResetCounterAtTop";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_PwmResetCounterAtTop(void) {
    RecordExpectedCall(mock, Mock_PwmResetCounterAtTop());
}
void PwmResetCounterAtTop_Stubbed(void) {
    RecordActualCall(mock, Mock_PwmResetCounterAtTop());
}

static RecordedCall * Mock_PwmTopIsOcr0a(void)
{
    char const *call_name = "PwmTopIsOcr0a";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_PwmTopIsOcr0a(void) {
    RecordExpectedCall(mock, Mock_PwmTopIsOcr0a());
}
void PwmTopIsOcr0a_Stubbed(void) {
    RecordActualCall(mock, Mock_PwmTopIsOcr0a());
}

static RecordedCall * Mock_PwmClkIsCpuClk(void)
{
    char const *call_name = "PwmClkIsCpuClk";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_PwmClkIsCpuClk(void) {
    RecordExpectedCall(mock, Mock_PwmClkIsCpuClk());
}
void PwmClkIsCpuClk_Stubbed(void) {
    RecordActualCall(mock, Mock_PwmClkIsCpuClk());
}

static RecordedCall * Mock_PwmEnableOutputSetUntilMatch(void)
{
    char const *call_name = "PwmEnableOutputSetUntilMatch";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_PwmEnableOutputSetUntilMatch(void) {
    RecordExpectedCall(mock, Mock_PwmEnableOutputSetUntilMatch());
}
void PwmEnableOutputSetUntilMatch_Stubbed(void) {
    RecordActualCall(mock, Mock_PwmEnableOutputSetUntilMatch());
}

static RecordedCall * Mock_PwmDisableOutput(void)
{
    char const *call_name = "PwmDisableOutput";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_PwmDisableOutput(void) {
    RecordExpectedCall(mock, Mock_PwmDisableOutput());
}
void PwmDisableOutput_Stubbed(void) {
    RecordActualCall(mock, Mock_PwmDisableOutput());
}
