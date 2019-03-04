#include <Mock.h>
#include <RecordedCall.h>
#include <RecordedArg.h>
#include <ReturnValues.h>
#include "mock_Ft1248.h"


static RecordedCall * Mock_FtActivateInterface(void)
{
    char const *call_name = "FtActivateInterface";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtActivateInterface(void) {
    RecordExpectedCall(mock, Mock_FtActivateInterface());
}
void FtActivateInterface_Stubbed(void) {
    RecordActualCall(mock, Mock_FtActivateInterface());
}

static RecordedCall * Mock_FtPushData(void)
{
    char const *call_name = "FtPushData";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtPushData(void) {
    RecordExpectedCall(mock, Mock_FtPushData());
}
void FtPushData_Stubbed(void) {
    RecordActualCall(mock, Mock_FtPushData());
}

static RecordedCall * Mock_FtOutputByte(uint8_t arg1)
{
    char const *call_name = "FtOutputByte";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void Expect_FtOutputByte(uint8_t arg1) {
    RecordExpectedCall(mock, Mock_FtOutputByte(arg1));
}
void FtOutputByte_Stubbed(uint8_t arg1) {
    RecordActualCall(mock, Mock_FtOutputByte(arg1));
}

static RecordedCall * Mock_FtPullData(void)
{
    char const *call_name = "FtPullData";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtPullData(void) {
    RecordExpectedCall(mock, Mock_FtPullData());
}
void FtPullData_Stubbed(void) {
    RecordActualCall(mock, Mock_FtPullData());
}

static RecordedCall * Mock_FtLetMasterDriveBus(void)
{
    char const *call_name = "FtLetMasterDriveBus";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtLetMasterDriveBus(void) {
    RecordExpectedCall(mock, Mock_FtLetMasterDriveBus());
}
void FtLetMasterDriveBus_Stubbed(void) {
    RecordActualCall(mock, Mock_FtLetMasterDriveBus());
}

static RecordedCall * Mock_FtLetSlaveDriveBus(void)
{
    char const *call_name = "FtLetSlaveDriveBus";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtLetSlaveDriveBus(void) {
    RecordExpectedCall(mock, Mock_FtLetSlaveDriveBus());
}
void FtLetSlaveDriveBus_Stubbed(void) {
    RecordActualCall(mock, Mock_FtLetSlaveDriveBus());
}

static RecordedCall * Mock_FtIsBusOk(void)
{
    char const *call_name = "FtIsBusOk";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtIsBusOk(void) {
    RecordExpectedCall(mock, Mock_FtIsBusOk());
}
bool FtIsBusOk_StubbedReturnValue = false;
bool FtIsBusOk_Stubbed(void) {
    RecordActualCall(mock, Mock_FtIsBusOk());
    return FtIsBusOk_StubbedReturnValue;
}

static RecordedCall * Mock_FtReadData(void)
{
    char const *call_name = "FtReadData";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtReadData(void) {
    RecordExpectedCall(mock, Mock_FtReadData());
}
uint8_t FtReadData_StubbedReturnValue = 0x00;
uint8_t FtReadData_Stubbed(void) {
    RecordActualCall(mock, Mock_FtReadData());
    return FtReadData_StubbedReturnValue;
}

//=====[ High-level mocks ]=====
static RecordedCall * Mock_FtSendCommand(uint8_t arg1)
{
    char const *call_name = "FtSendCommand";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void Expect_FtSendCommand(uint8_t arg1) {
    RecordExpectedCall(mock, Mock_FtSendCommand(arg1));
}
void FtSendCommand_Stubbed(uint8_t arg1) {
    RecordActualCall(mock, Mock_FtSendCommand(arg1));
}

static RecordedCall * Mock_FtBusTurnaround(void)
{
    char const *call_name = "FtBusTurnaround";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtBusTurnaround(void) {
    RecordExpectedCall(mock, Mock_FtBusTurnaround());
}
bool FtBusTurnaround_StubbedReturnValue = true;
bool FtBusTurnaround_Stubbed(void) {
    RecordActualCall(mock, Mock_FtBusTurnaround());
    return FtBusTurnaround_StubbedReturnValue;
}

static RecordedCall * Mock_FtDeactivateInterface(void)
{
    char const *call_name = "FtDeactivateInterface";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtDeactivateInterface(void) {
    RecordExpectedCall(mock, Mock_FtDeactivateInterface());
}
void FtDeactivateInterface_Stubbed(void) {
    RecordActualCall(mock, Mock_FtDeactivateInterface());
}

static RecordedCall * Mock_FtRead(uint8_t* arg1)
{
    char const *call_name = "FtRead";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t**)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void Expect_FtRead(uint8_t* arg1) {
    RecordExpectedCall(mock, Mock_FtRead(arg1));
}
static bool FtRead_ack_nack_example[] = {true, true, false};  // example return values
bool *FtRead_StubbedReturnValue = FtRead_ack_nack_example;    // point to first value
static uint8_t FtRead_fake_data_example[] = {6, 28};
uint8_t *FtRead_StubbedDataBusPtr = FtRead_fake_data_example; // point to first value
bool FtRead_Stubbed(uint8_t* arg1) {
    RecordActualCall(mock, Mock_FtRead(arg1));
    // If the test scenario is ACK, read the faked bus data and go to next byte.
    if (*FtRead_StubbedReturnValue) *arg1 = *(FtRead_StubbedDataBusPtr++);
    // Return ACK/NAK and go to next ACK/NAK.
    return *(FtRead_StubbedReturnValue++);
}

static RecordedCall * Mock_FtWrite(uint8_t const * arg1)
{
    char const *call_name = "FtWrite";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t const **)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void Expect_FtWrite(uint8_t const * arg1) {
    RecordExpectedCall(mock, Mock_FtWrite(arg1));
}
static bool FtWrite_ack_nack_example[] = {true, true, false};  // example return values
bool *FtWrite_StubbedReturnValue = FtWrite_ack_nack_example;    // point to first value
bool FtWrite_Stubbed(uint8_t const * arg1) {
    RecordActualCall(mock, Mock_FtWrite(arg1));
    return *(FtWrite_StubbedReturnValue++);
}

static RecordedCall * Mock_FtWriteData(uint8_t arg1)
{
    char const *call_name = "FtWriteData";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t*)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void Expect_FtWriteData(uint8_t arg1) {
    RecordExpectedCall(mock, Mock_FtWriteData(arg1));
}
void FtWriteData_Stubbed(uint8_t arg1) {
    RecordActualCall(mock, Mock_FtWriteData(arg1));
}

static RecordedCall * Mock_FtHasDataToRead(void)
{
    char const *call_name = "FtHasDataToRead";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtHasDataToRead(void) {
    RecordExpectedCall(mock, Mock_FtHasDataToRead());
}
bool FtHasDataToRead_StubbedReturnValue = false;
bool FtHasDataToRead_Stubbed(void) {
    RecordActualCall(mock, Mock_FtHasDataToRead());
    return FtHasDataToRead_StubbedReturnValue;
}

static RecordedCall * Mock_FtHasRoomToWrite(void)
{
    char const *call_name = "FtHasRoomToWrite";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_FtHasRoomToWrite(void) {
    RecordExpectedCall(mock, Mock_FtHasRoomToWrite());
}
bool FtHasRoomToWrite_StubbedReturnValue = false;
bool FtHasRoomToWrite_Stubbed(void) {
    RecordActualCall(mock, Mock_FtHasRoomToWrite());
    return FtHasRoomToWrite_StubbedReturnValue;
}
