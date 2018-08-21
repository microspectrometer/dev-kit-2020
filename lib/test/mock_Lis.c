#include "mock_Lis.h"
#include <Mock.h>
#include <RecordedCall.h>
#include <RecordedArg.h>

static RecordedCall * Mock_LisRunClkAt50kHz(void)
{
    char const *call_name = "LisRunClkAt50kHz";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_LisRunClkAt50kHz(void) {
    RecordExpectedCall(mock, Mock_LisRunClkAt50kHz());
}
void LisRunClkAt50kHz_Stubbed(void) {
    RecordActualCall(mock, Mock_LisRunClkAt50kHz());
}

static RecordedCall * Mock_LisClkOn(void)
{
    char const *call_name = "LisClkOn";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_LisClkOn(void) {
    RecordExpectedCall(mock, Mock_LisClkOn());
}
void LisClkOn_Stubbed(void) {
    RecordActualCall(mock, Mock_LisClkOn());
}

