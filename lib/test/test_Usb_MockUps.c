#include "Ft1248.h"
#include "mock_Ft1248.h"
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

static bool (*FtRead_Saved)(uint8_t *);
static void Stub_FtRead(void) {
    FtRead_Saved = FtRead;
    FtRead = FtRead_Stubbed;
}
static void Unstub_FtRead(void) {
    FtRead = FtRead_Saved;
}
void SetUpMock_UsbRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtBusTurnaround();  // DOF
    Stub_FtRead();  // DOF
}
void TearDownMock_UsbRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtBusTurnaround();  // DOF
    Unstub_FtRead();  // DOF
}
void SetUpMock_DetailsOf_UsbRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtSendCommand();  // DOF
    Stub_FtBusTurnaround();  // DOF
    Stub_FtDeactivateInterface();  // DOF
    Stub_FtRead();  // DOF
}
void TearDownMock_DetailsOf_UsbRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtSendCommand();  // DOF
    Unstub_FtBusTurnaround();  // DOF
    Unstub_FtDeactivateInterface();  // DOF
    Unstub_FtRead();  // DOF
}

/* bool (*FtWrite)(uint8_t const *write_buffer) = FtWrite_Implementation; */
static bool (*FtWrite_Saved)(uint8_t const *);
static void Stub_FtWrite(void) {
    FtWrite_Saved = FtWrite;
    FtWrite = FtWrite_Stubbed;
}
static void Unstub_FtWrite(void) {
    FtWrite = FtWrite_Saved;
}
void SetUpMock_UsbWrite(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtBusTurnaround();  // DOF
    Stub_FtWrite();  // DOF
}
void TearDownMock_UsbWrite(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtBusTurnaround();  // DOF
    Unstub_FtWrite();  // DOF
}

void SetUpMock_DetailsOf_UsbWrite(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtSendCommand();  // DOF
    Stub_FtBusTurnaround();  // DOF
    Stub_FtDeactivateInterface();  // DOF
    Stub_FtWrite();  // DOF
}
void TearDownMock_DetailsOf_UsbWrite(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtSendCommand();  // DOF
    Unstub_FtBusTurnaround();  // DOF
    Unstub_FtDeactivateInterface();  // DOF
    Unstub_FtWrite();  // DOF
}

static bool (*FtHasDataToRead_Saved)(void);
static void Stub_FtHasDataToRead(void) {
    FtHasDataToRead_Saved = FtHasDataToRead;
    FtHasDataToRead = FtHasDataToRead_Stubbed;
}
static void Unstub_FtHasDataToRead(void) {
    FtHasDataToRead = FtHasDataToRead_Saved;
}
void SetUpMock_UsbHasDataToRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtHasDataToRead();  // DOF
}
void TearDownMock_UsbHasDataToRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtHasDataToRead();  // DOF
}

static bool (*FtHasRoomToWrite_Saved)(void);
static void Stub_FtHasRoomToWrite(void) {
    FtHasRoomToWrite_Saved = FtHasRoomToWrite;
    FtHasRoomToWrite = FtHasRoomToWrite_Stubbed;
}
static void Unstub_FtHasRoomToWrite(void) {
    FtHasRoomToWrite = FtHasRoomToWrite_Saved;
}
void SetUpMock_UsbHasRoomToWrite(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtHasRoomToWrite();  // DOF
}
void TearDownMock_UsbHasRoomToWrite(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtHasRoomToWrite();  // DOF
}

