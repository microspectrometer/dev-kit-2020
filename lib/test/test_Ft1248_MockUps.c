#include "test_Ft1248_MockUps.h"    // SetUpMock_FUT, TearDownMock_FUT
#include "Ft1248.h"                 // lib DOF
#include "mock_Ft1248.h"            // lib DOF_Stubbed
#include <Mock.h>                   // Class: Mock

static void (*FtOutputCmdOnMiosio_Saved)(uint8_t FtCmd);
static void Stub_FtOutputCmdOnMiosio(void) {
    FtOutputCmdOnMiosio_Saved = FtOutputCmdOnMiosio;
    FtOutputCmdOnMiosio = FtOutputCmdOnMiosio_Stubbed;
}
static void Unstub_FtOutputCmdOnMiosio(void) {
    FtOutputCmdOnMiosio = FtOutputCmdOnMiosio_Saved;
}
void SetUpMock_FtSendCommand(void)
{
    mock = Mock_new();
    //
    Stub_FtOutputCmdOnMiosio();
}
void TearDownMock_FtSendCommand(void)
{
    Unstub_FtOutputCmdOnMiosio();
    //
    Mock_destroy(mock); mock=NULL;
}

