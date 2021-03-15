#include "UartSpi_faked.h"
#include "Mock.h" // record call history in "mock"
static RecordedCall * Record_StartAdcConversion(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "StartAdcConversion";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void StartAdcConversion_fake(void)
{
    RecordActualCall(mock, Record_StartAdcConversion());
}
