#include "DebugLed.h"
#include "Ft1248.h"                 // DOF lib includes FtCmd constants
#include "mock_Ft1248.h"            // mocked version of DOF lib
#include "mock_DebugLed.h"          // mocked version of DOF lib
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
    // [x] UsbRead returns false if there was no data read
    //  - SCK is already low
    //  - next step is to call FtDeactivateInterface to end the Ft1248 session
    // [x] UsbRead turns LED red if there was no data read
    // [x] UsbRead returns true if there is data read
    // [ ] UsbRead should read until buffer is empty
    // [ ] UsbRead calls FtDeactivateInterface to end the Ft1248 session
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
    // return false
    // exit before reading any bytes from the buffer
    // end the Ft1248 session
    //=====[ Mock-up values returned by stubbed functions ]=====
    FtBusTurnaround_StubbedReturnValue = false;
    //=====[ Set expectations ]=====
    Expect_FtSendCommand(FtCmd_Read);
    Expect_FtBusTurnaround();
    Expect_DebugLedTurnRedToShowError();
    Expect_FtDeactivateInterface();
    //=====[ Operate ]=====
    bool there_was_data_to_read = UsbRead();
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
    TEST_ASSERT_FALSE(there_was_data_to_read);
}
void UsbRead_turns_LED_red_if_there_was_no_data_read(void)
{
    //=====[ Mock-up values returned by stubbed functions ]=====
    FtBusTurnaround_StubbedReturnValue = false;
    //=====[ Set expectations ]=====
    Expect_FtSendCommand(FtCmd_Read);
    Expect_FtBusTurnaround();
    Expect_DebugLedTurnRedToShowError(); _MOCK_DEBUGLED_H;
    Expect_FtDeactivateInterface();
    //=====[ Operate ]=====
    UsbRead();
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

void UsbRead_returns_true_if_there_is_data_to_read(void)
{
    //=====[ Mock-up values returned by stubbed functions ]=====
    FtBusTurnaround_StubbedReturnValue = true;
    //=====[ Operate ]=====
    bool there_was_data_to_read = UsbRead();
    //=====[ Test ]=====
    TEST_ASSERT_TRUE(there_was_data_to_read);
}
void UsbRead_should_read_until_buffer_is_empty(void)
{
    //=====[ Mock-up values returned by stubbed functions ]=====
    FtBusTurnaround_StubbedReturnValue = true;
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
