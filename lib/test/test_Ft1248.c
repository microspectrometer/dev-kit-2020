#include "Ft1248.h"
#include "test_Ft1248_MockUps.h"
#include "test_Ft1248.h"
#include "mock_Ft1248.h"            // mocked version of DOF lib
#include <Mock.h>                   // RanAsHoped, WhyDidItFail
#include <unity.h>                  // all the TEST_blah macros

//=====[ List of tests to write ]=====
    // FtSendCommand(cmd) handles the entire command phase.
    // [x] FtSendCommand(FtCmd_Read) should
    //     handle the entire command phase for the Read command:
    //     [x] FtActivateInterface()    (SS low)
    //     [x] FtPushData()             (SCK high)
    //     [x] FtOutputByte(FtCmd_Read) (Master outputs the read command on MIOSIO)
    //     [x] FtPullData()             (SCK low)

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
    Expect_FtActivateInterface();
    Expect_FtPushData();
    Expect_FtOutputByte(FtCmd_Read);
    Expect_FtPullData();
    //=====[ Operate ]=====
    FtSendCommand(FtCmd_Read);
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

