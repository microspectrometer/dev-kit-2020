#include "Ft1248.h"                 // DOF lib
#include "mock_Ft1248.h"            // mocked version of DOF lib
#include "test_Usb_MockUps.h"       // mock setups for LUT
#include <Mock.h>                   // RanAsHoped, WhyDidItFail
#include "test_Usb.h"               // LUT test code
#include "Usb.h"                    // LUT source code
#include "unity.h"                  // all the TEST_blah macros
//=====[ List of tests to write ]=====
    // [x] UsbRead should:
    //  - call FtSendCommand(FtCmd_Read)
    //  - check if ok to proceed after FtBusTuraround()
    //  - loop FtRead() until buffer is empty
    //  - then call FtDeactivateInterface() to end Ft1248 session
    //      - FtRead() leaves SCK low.
    //          - This is good.
    //          - SCK should be low before deactivating the interface.
    // [ ] UsbRead returns false if there was no data read
    // [ ] UsbRead turns LED red if there was no data read
    // [ ] UsbRead returns true if there is data read
    // [ ] UsbRead should read until buffer is empty
// void SetUp_NothingForUsb(void){}
// void TearDown_NothingForUsb(void){}
void SetUp_UsbRead(void){
    SetUpMock_UsbRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_UsbRead(void){
    TearDownMock_UsbRead();    // destroy the mock object
    // other teardown code
}
void UsbRead_returns_false_if_there_was_no_data_read(void)
{
    TEST_FAIL_MESSAGE("Implement test.");
}
//void UsbRead_turns_LED_red_if_there_was_no_data_read(void)
//void UsbRead_returns_true_if_there_is_data_read(void)
void UsbRead_should_read_until_buffer_is_empty(void)
{
    //=====[ Mock-up values returned by stubbed functions ]=====
    bool FtBusTurnaround_StubbedReturnValue = true;
    //=====[ Set expectations ]=====
    Expect_FtSendCommand(FtCmd_Read);
    Expect_FtBusTurnaround();
    //=====[ Operate ]=====
    UsbRead();
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}


