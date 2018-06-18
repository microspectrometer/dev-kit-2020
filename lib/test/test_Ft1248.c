#include "test_Ft1248.h"
#include "test_Ft1248_MockUps.h"    // mockups for testing each FUT in FUT lib
#include <Ft1248.h>                 // FUT lib and DOF lib
#include "mock_Ft1248.h"            // mocked version of DOF lib
#include <Mock.h>                   // RanAsHoped, WhyDidItFail
#include <unity.h>                  // all the TEST_blah macros

void SetUp_FtSendCommand(void){
    SetUpMock_FtSendCommand();    // create the mock object to record calls
    // other setup code
}
void TearDown_FtSendCommand(void){
    TearDownMock_FtSendCommand(); // destroy the mock object
    // other teardown code
}

void FtSendCommand_Read_does_entire_command_phase_for_ReadCmd(void)
{
    //ExpectCall(mock, "FtPushData");
    //Expect_FtPushData();
    //ExpectCall(mock, "FtLetMasterDriveBus");
    Expect_FtOutputCmdOnMiosio(FtCmd_Read);
    //Example of what to do if there is more than one mocked lib:
    //_MOCK_FT1248_H; Expect_FtOutputCmdOnMiosio(FtCmd_Read);
    //ExpectCall(mock, "FtPullData");
    //=====[ Operate ]=====
    FtSendCommand(FtCmd_Read);
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

