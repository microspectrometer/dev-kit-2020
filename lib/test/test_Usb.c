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
    // [x] UsbRead should return the number of bytes read
    //  - change return type from `bool` to `uint16_t`
    //  - [x] UsbRead should return 0 if there was no data to read
    //      - SCK is already low
    //      - next call FtDeactivateInterface to end the Ft1248 session
    //  - [x] UsbRead turns LED red if there was no data read
    //  - [x] UsbRead returns N if there were N bytes to read
    //  - [x] UsbRead should read until buffer is empty
    //      - mock FtRead
    //      - set up a list of return values: [true, true, true, false]
    //      - this is a test that UsbRead loops until a NAK is received
    //
    // [ ] UsbRead copies bytes from MIOSIO to the input read buffer address
    //  - this is a system test, not a unit test because it cares about matching
    //  the value from MIOSIO
    //  - mock FtReadData? do not mock FtRead?
    // [ ] UsbRead copies bytes to the input read buffer address
    //  - this is a unit test: UsbRead is writing to the input address
    //  - I only need to test that it is incrementing the address
    //  - check by writing the byte number to each byte in the read buffer
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
void UsbRead_should_return_0_if_there_was_no_data_to_read(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    FtBusTurnaround_StubbedReturnValue = false; // simulate no bytes in rx buffer
    //=====[ Operate ]=====
    uint8_t read_buffer; uint8_t *read_buffer_address = &read_buffer;
    uint16_t actual_num_bytes_read = UsbRead(read_buffer_address);
    //=====[ Test: UsbRead returns 0 if there is no data to read ]=====
    uint16_t expected_num_bytes_read = 0; // expect 0 bytes read
    TEST_ASSERT_EQUAL_UINT16(expected_num_bytes_read, actual_num_bytes_read);
}
void UsbRead_turns_LED_red_if_there_was_no_data_read(void)
{
    //=====[ Set expectations ]=====
    Expect_FtSendCommand(FtCmd_Read);
    Expect_FtBusTurnaround(); // false := error: nothing to read
    Expect_DebugLedTurnRedToShowError();  _MOCK_DEBUGLED_H;
    // exit before attempting to read any bytes from the buffer
    Expect_FtDeactivateInterface();
    //=====[ Mock-up test scenario by defining return values ]=====
    FtBusTurnaround_StubbedReturnValue = false;
    //=====[ Operate ]=====
    uint8_t read_buffer; uint8_t *read_buffer_address = &read_buffer;
    UsbRead(read_buffer_address);
    //=====[ Test: should follow sad path and show error with red LED ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

void UsbRead_returns_N_if_there_were_N_bytes_to_read(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    FtBusTurnaround_StubbedReturnValue = true;  // bus is OK
    // ACK (true)  is received before reading each byte.
    // NAK (false) is received when the buffer is empty.
    /* bool ack_nak_list[] = {false};                      // 0 bytes */
    /* bool ack_nak_list[] = {true, false};                // 1 byte */
    /* bool ack_nak_list[] = {true, true, false};          // 2 bytes */
    /* bool ack_nak_list[] = {true, true, true, false};    // 3 bytes */
    bool ack_nak_list[] = {true, true, true, true, false};    // 4 bytes
    FtRead_StubbedReturnValue = ack_nak_list;  // point at first list element
    //=====[ Operate ]=====
    uint8_t read_buffer; uint8_t *read_buffer_address = &read_buffer;
    uint16_t num_bytes_read = UsbRead(read_buffer_address);
    //=====[ Test ]=====
    uint16_t expected_num_bytes = sizeof(ack_nak_list)-1;
    TEST_ASSERT_EQUAL_UINT16(expected_num_bytes, num_bytes_read);
}

void UsbRead_should_read_until_buffer_is_empty(void)
{
    //=====[ Set expectations for the happy path ]=====
    Expect_FtSendCommand(FtCmd_Read);
    Expect_FtBusTurnaround();
    // Mock-up scenario: buffer has three bytes.
    bool ack_nak_list[] = {true, true, true, false};  // false := buffer empty
    uint16_t num_bytes_in_buffer = sizeof(ack_nak_list)-1;
    TEST_ASSERT_EQUAL_UINT16(3, num_bytes_in_buffer);
    uint8_t read_buffer[num_bytes_in_buffer];
    // Expect FtRead is called once for every byte in the buffer.
    uint16_t byte_num = 0;
    uint8_t *read_buffer_address = read_buffer;
    while (byte_num++ < num_bytes_in_buffer) Expect_FtRead(read_buffer_address++);
    // Expect FtRead is called one more time to find out the buffer is empty.
    Expect_FtRead(read_buffer_address);
    Expect_FtDeactivateInterface();

    //=====[ Mock-up test scenario by defining return values ]=====
    FtBusTurnaround_StubbedReturnValue = true;  // bus is OK
    FtRead_StubbedReturnValue = ack_nak_list;  // point at first list element

    //=====[ Operate ]=====
    uint16_t actual_num_bytes_read = UsbRead(read_buffer);

    //=====[ Test: UsbRead read the buffer until it was empty ]=====
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, actual_num_bytes_read);
    //=====[ Test: UsbRead followed its happy path ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

void UsbRead_copies_bytes_to_the_input_read_buffer_address(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    // FtBusTurnaround: receive buffer is not empty
    FtBusTurnaround_StubbedReturnValue = true;
    // FtRead: buffer has three bytes.
    bool ack_nak_list[] = {true, true, true, false};  // false := buffer empty
    uint16_t num_bytes_in_buffer = sizeof(ack_nak_list)-1;
    TEST_ASSERT_EQUAL_UINT16(3, num_bytes_in_buffer);
    FtRead_StubbedReturnValue = ack_nak_list;  // point at first list element
    // FtRead: make up the values of the bytes in the buffer
    uint8_t expected_read_buffer[] = {0, 1, 2};  // stub uses this fake data
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, sizeof(expected_read_buffer));
    FtRead_StubbedDataBusPtr = expected_read_buffer;

    //=====[ Operate ]=====
    uint8_t actual_read_buffer[num_bytes_in_buffer];
    uint16_t num_bytes_read = UsbRead(actual_read_buffer);
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, num_bytes_read);
    //=====[ Test ]=====
    TEST_ASSERT_EQUAL_UINT8_ARRAY(
        expected_read_buffer,
        actual_read_buffer,
        sizeof(expected_read_buffer)
        );
}
