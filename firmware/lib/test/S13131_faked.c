#include "S13131_faked.h"
#include "Mock.h" // record call history in "mock"
#include "S13131.h"

static RecordedCall * Record_S13131Readout(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "S13131Readout";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void S13131Readout_fake(void)
{
    RecordActualCall(mock, Record_S13131Readout());
}

static RecordedCall * Record_S13131Expose(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "S13131Expose";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void S13131Expose_fake(void)
{
    RecordActualCall(mock, Record_S13131Expose());
}

static RecordedCall * Record_WaitForS13131ClkLow(void)
{
    char const *call_name = "WaitForS13131ClkLow";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void WaitForS13131ClkLow_fake(void)
{
    RecordActualCall(mock, Record_WaitForS13131ClkLow());
}
