#include "mock_DebugLed.h"
#include <DebugLed.h>
#include <Mock.h>

static RecordedCall * Mock_DebugLedTurnOn(void)
{
    char const *call_name = "DebugLedTurnOn";
    RecordedCall *record_of_call = RecordedCall_new(call_name);
    return record_of_call;
}
void Expect_DebugLedTurnOn(void) {
    RecordExpectedCall(mock, Mock_DebugLedTurnOn());
}
static void DebugLedTurnOn_Stubbed(void) {
    RecordActualCall(mock, Mock_DebugLedTurnOn());
}
static void (*DebugLedTurnOn_Saved)(void);
void Stub_DebugLedTurnOn(void) {
    DebugLedTurnOn_Saved = DebugLedTurnOn;
    DebugLedTurnOn = DebugLedTurnOn_Stubbed;
}
void Unstub_DebugLedTurnOn(void) {
    DebugLedTurnOn = DebugLedTurnOn_Saved;
}

static RecordedCall * Mock_DebugLedTurnGreen(void)
{
    char const *call_name = "DebugLedTurnGreen";
    RecordedCall *record_of_call = RecordedCall_new(call_name);
    return record_of_call;
}
void Expect_DebugLedTurnGreen(void) {
    RecordExpectedCall(mock, Mock_DebugLedTurnGreen());
}
static void DebugLedTurnGreen_Stubbed(void) {
    RecordActualCall(mock, Mock_DebugLedTurnGreen());
}
static void (*DebugLedTurnGreen_Saved)(void);
void Stub_DebugLedTurnGreen(void) {
    DebugLedTurnGreen_Saved = DebugLedTurnGreen;
    DebugLedTurnGreen = DebugLedTurnGreen_Stubbed;
}
void Unstub_DebugLedTurnGreen(void) {
    DebugLedTurnGreen = DebugLedTurnGreen_Saved;
}

