#include "Mock.h" // record call history in "mock"
// What is "mock"?
    // `faked` calls record themselves in `mock`
    // `Mock.h` declares "mock" is defined elsewhere:
    //      `extern Mock_s mock;`
    // The test that calls the `faked` call:
    // - creates `mock` in its `Setup`
    // - destroys `mock` in its `Teardown`

/* =====[ Mock EnableSpiInterrupt() ]===== */
static RecordedCall * Record_EnableSpiInterrupt(void)
{ // Define call recorded when func-under-test calls mocked function.
    char const *call_name = "EnableSpiInterrupt";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void EnableSpiInterrupt_fake(void)
{
    RecordActualCall(mock, Record_EnableSpiInterrupt());
}

