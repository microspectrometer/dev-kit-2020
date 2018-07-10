#include "Ft1248.h"
#include "mock_Ft1248.h"
#include "DebugLed.h"
#include "mock_DebugLed.h"
#include <Mock.h>
#include "test_Usb_MockUps.h"

static void (*FtSendCommand_Saved)(uint8_t);
static void Stub_FtSendCommand(void) {
    FtSendCommand_Saved = FtSendCommand;
    FtSendCommand = FtSendCommand_Stubbed;
}
static void Unstub_FtSendCommand(void) {
    FtSendCommand = FtSendCommand_Saved;
}
static bool (*FtBusTurnaround_Saved)(void);
static void Stub_FtBusTurnaround(void) {
    FtBusTurnaround_Saved = FtBusTurnaround;
    FtBusTurnaround = FtBusTurnaround_Stubbed;
}
static void Unstub_FtBusTurnaround(void) {
    FtBusTurnaround = FtBusTurnaround_Saved;
}
static void (*FtDeactivateInterface_Saved)(void);
static void Stub_FtDeactivateInterface(void) {
    FtDeactivateInterface_Saved = FtDeactivateInterface;
    FtDeactivateInterface = FtDeactivateInterface_Stubbed;
}
static void Unstub_FtDeactivateInterface(void) {
    FtDeactivateInterface = FtDeactivateInterface_Saved;
}
static void (*DebugLedTurnRedToShowError_Saved)(void);
static void Stub_DebugLedTurnRedToShowError(void) {
    DebugLedTurnRedToShowError_Saved = DebugLedTurnRedToShowError;
    DebugLedTurnRedToShowError = DebugLedTurnRedToShowError_Stubbed;
}
static void Unstub_DebugLedTurnRedToShowError(void) {
    DebugLedTurnRedToShowError = DebugLedTurnRedToShowError_Saved;
}
void SetUpMock_UsbRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtSendCommand();  // DOF
    Stub_FtBusTurnaround();  // DOF
    Stub_FtDeactivateInterface();  // DOF
    Stub_DebugLedTurnRedToShowError();  // DOF
}
void TearDownMock_UsbRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtSendCommand();  // DOF
    Unstub_FtBusTurnaround();  // DOF
    Unstub_FtDeactivateInterface();  // DOF
    Unstub_DebugLedTurnRedToShowError();  // DOF
}

