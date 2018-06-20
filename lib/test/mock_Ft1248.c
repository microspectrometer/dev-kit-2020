#include <Mock.h>
#include <RecordedCall.h>
#include <RecordedArg.h>
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
void FtIsBusOk_Stubbed(void) {
    RecordActualCall(mock, Mock_FtIsBusOk());
}
