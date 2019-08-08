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

static RecordedCall * Mock_LoadNextCfgBit(uint32_t arg1)
{
    char const *call_name = "LoadNextCfgBit";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint32_t);
    *((uint32_t*)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void Expect_LoadNextCfgBit(uint32_t arg1) {
    RecordExpectedCall(mock, Mock_LoadNextCfgBit(arg1));
}
void LoadNextCfgBit_Stubbed(uint32_t arg1) {
    RecordActualCall(mock, Mock_LoadNextCfgBit(arg1));
}
