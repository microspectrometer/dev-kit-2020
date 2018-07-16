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
    // [ ] UsbRead should return the number of bytes read
    //  - change return type from `bool` to `uint16_t`
    //  - [ ] UsbRead should return 0 if there was no data to read
    //  - ~~[x] UsbRead returns false if there was no data read~~
    //      - SCK is already low
    //      - next call FtDeactivateInterface to end the Ft1248 session
    //  - [ ] UsbRead returns 3 if there were 3 bytes to read
    //  - ~~[x] UsbRead returns true if there is data read~~
    // [x] UsbRead turns LED red if there was no data read
    //
    // [ ] UsbRead should read until buffer is empty
    //  - mock FtRead
    //  - set up a list of return values: [true, true, true, false]
    //  - this is a test that UsbRead loops until a NAK is received
    // [ ] UsbRead copies bytes from MIOSIO to the input read buffer address
    //  - this is a system test, not a unit test
    //  - mock FtReadData, do not mock FtRead
    // [x] UsbRead calls FtDeactivateInterface to end the Ft1248 session
    //  - [x] called when returning false
    //  - [x] called when returning true
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
    TEST_FAIL_MESSAGE("Change return type from bool to uint16_t.");
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
    uint8_t read_buffer = 0x00;
    uint8_t *read_buffer_address = &read_buffer;
    bool there_was_data_to_read = UsbRead(read_buffer_address);
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
    uint8_t read_buffer = 0x00;
    uint8_t *read_buffer_address = &read_buffer;
    UsbRead(read_buffer_address);
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

void UsbRead_returns_true_if_there_is_data_to_read(void)
{
    TEST_FAIL_MESSAGE("Change return type from bool to uint16_t.");
    //=====[ Mock-up values returned by stubbed functions ]=====
    FtBusTurnaround_StubbedReturnValue = true;
    //=====[ Operate ]=====
    uint8_t read_buffer = 0x00;
    uint8_t *read_buffer_address = &read_buffer;
    bool there_was_data_to_read = UsbRead(read_buffer_address);
    //=====[ Test ]=====
    TEST_ASSERT_TRUE(there_was_data_to_read);
}
void UsbRead_should_read_until_buffer_is_empty(void)
{  // USB rx buffer has bytes -- UsbRead reads until there are no more bytes
    //=====[ Mock-up values returned by stubbed functions ]=====
    FtBusTurnaround_StubbedReturnValue = true;  // bus is OK
    bool ack_nack_list[] = {true, true, true, false};  // false := buffer empty
    //=====[ Set expectations ]=====
    Expect_FtSendCommand(FtCmd_Read);
    Expect_FtBusTurnaround();
    // Set up the buffer as just a pointer or use an array to reserve a size?
    uint8_t read_buffer;
    uint8_t *read_buffer_address = &read_buffer;
    for (unsigned long i=0; i<sizeof(ack_nack_list); i++)
    {
        FtRead_StubbedReturnValue = ack_nack_list[i];
        Expect_FtRead(read_buffer_address++); _MOCK_FT1248_H;
    }
    Expect_FtDeactivateInterface();
    //=====[ Operate ]=====
    UsbRead(read_buffer_address); // copy USB rx buffer to the read buffer
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
    /* TEST_ASSERT_EQUAL_UINT8(expected_data_byte, read_buffer); */
    TEST_FAIL_MESSAGE("TODO: mock a list of FtRead return values and test UsbRead for the number of bytes read.");
}
