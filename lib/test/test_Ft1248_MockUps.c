#include "test_Ft1248_MockUps.h"    // SetUpMock_FUT, TearDownMock_FUT
#include "Ft1248.h"                 // lib DOF
#include "mock_Ft1248.h"            // lib DOF_Stubbed
#include <Mock.h>                   // Class: Mock

static void (*FtActivateInterface_Saved)(void);
static void Stub_FtActivateInterface(void) {
    FtActivateInterface_Saved = FtActivateInterface;
    FtActivateInterface = FtActivateInterface_Stubbed;
}
static void Unstub_FtActivateInterface(void) {
    FtActivateInterface = FtActivateInterface_Saved;
}
static void (*FtPushData_Saved)(void);
static void Stub_FtPushData(void) {
    FtPushData_Saved = FtPushData;
    FtPushData = FtPushData_Stubbed;
}
static void Unstub_FtPushData(void) {
    FtPushData = FtPushData_Saved;
}
static void (*FtOutputByte_Saved)(uint8_t);
static void Stub_FtOutputByte(void) {
    FtOutputByte_Saved = FtOutputByte;
    FtOutputByte = FtOutputByte_Stubbed;
}
static void Unstub_FtOutputByte(void) {
    FtOutputByte = FtOutputByte_Saved;
}
static void (*FtPullData_Saved)(void);
static void Stub_FtPullData(void) {
    FtPullData_Saved = FtPullData;
    FtPullData = FtPullData_Stubbed;
}
static void Unstub_FtPullData(void) {
    FtPullData = FtPullData_Saved;
}
static void (*FtLetMasterDriveBus_Saved)(void);
static void Stub_FtLetMasterDriveBus(void) {
    FtLetMasterDriveBus_Saved = FtLetMasterDriveBus;
    FtLetMasterDriveBus = FtLetMasterDriveBus_Stubbed;
}
static void Unstub_FtLetMasterDriveBus(void) {
    FtLetMasterDriveBus = FtLetMasterDriveBus_Saved;
}
void SetUpMock_FtSendCommand(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_FtActivateInterface();  // DOF
    Stub_FtPushData();  // DOF
    Stub_FtOutputByte();  // DOF
    Stub_FtPullData();  // DOF
    Stub_FtLetMasterDriveBus();  // DOF
}
void TearDownMock_FtSendCommand(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_FtActivateInterface();  // DOF
    Unstub_FtPushData();  // DOF
    Unstub_FtOutputByte();  // DOF
    Unstub_FtPullData();  // DOF
    Unstub_FtLetMasterDriveBus();  // DOF
}

