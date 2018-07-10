#include "Ft1248.h"
#include "mock_Ft1248.h"
#include <Mock.h>
#include "test_Usb_MockUps.h"
void dummy_Usb_MockUps(void){}

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
void SetUpMock_UsbRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtSendCommand();  // DOF
    Stub_FtBusTurnaround();  // DOF
}
void TearDownMock_UsbRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtSendCommand();  // DOF
    Unstub_FtBusTurnaround();  // DOF
}
