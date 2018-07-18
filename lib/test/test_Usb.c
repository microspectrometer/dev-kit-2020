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
    // [x] UsbHasDataToRead returns true if the rx buffer has data
    // [x] UsbHasDataToRead returns false if the rx buffer is empty
    // [x] UsbHasRoomToWrite returns true if the tx buffer is not full
    // [ ] UsbHasRoomToWrite returns false if the tx buffer is full
    // [x] UsbRead should indicate data was read
    //  - [x] UsbRead returns 0 if there was no data to read
    //  - [x] UsbRead returns N if there were N bytes to read
    // [ ] UsbRead turns LED red if there was no data to read
    // [ ] UsbRead stops reading bytes when the rx buffer is empty
    // UsbRead should copy data to the read buffer input
    // [x] UsbRead copies bytes to the input read buffer address
    //  - tests that UsbRead is incrementing the read buffer address
    // [ ] UsbRead copies bytes from MIOSIO to the input read buffer address
    //  - this is a system test, not a unit test because it cares about matching
    //  the value on MIOSIO
    //  - mock FtReadData? do not mock FtRead?
    // [x] UsbWrite returns the number of bytes sent
    // [x] UsbWrite calls FtWrite for each byte to send
    // [x] UsbWrite stops sending bytes if all bytes are sent
    // [ ] UsbWrite stops sending bytes if the tx buffer is full
    // [ ] UsbWrite copies bytes from the input write buffer
    //  - tests that UsbWrite is incrementing the write buffer address
    // [ ] UsbWrite copies bytes from the input write buffer to MIOSIO
    //  - this is a system test, not a unit test because it cares about matching
    //  the value on MIOSIO
//=====[ List of tests that check implementation details ]=====
    // UsbRead implementation:
    //  - call FtSendCommand(FtCmd_Read)
    //  - check if ok to proceed after FtBusTurnaround()
    //  - loop FtRead() until buffer is empty
    //      - FtRead() leaves SCK low.
    //          - This is good.
    //          - SCK should be low before deactivating the interface.
    //  - then call FtDeactivateInterface() to end Ft1248 session
    //      - call in happy path
    //      - call in sad path
    // UsbRead sad path implementation:
    // [x] UsbRead turns LED red if there was no data to read
    //  - note *this is different* from testing that UsbRead returns 0
    //  - this tests the implementation of the sad path
    //  - if the implementation changes, this test fails, but UsbRead returns 0
    //  still passes
    //  - that is *very helpful* to identify that the high-level behavior is
    //  still correct and that the test only fails because of an implementation
    //  change
    // UsbRead happy path implementation:
    // [x] UsbRead should read until buffer is empty
    //  - this checks that UsbRead loops calling FtRead as long as it returns ACK
    //  - note *this is different* from testing that UsbRead returns N bytes
    //  - if this fails, but UsbRead returns N bytes still passes, it means the
    //  behavior is still correct but I changed the implementation
    //
void SetUp_NothingForUsb(void){}
void TearDown_NothingForUsb(void){}
void SetUp_UsbHasDataToRead(void){
    SetUpMock_UsbHasDataToRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_UsbHasDataToRead(void){
    TearDownMock_UsbHasDataToRead();    // destroy the mock object
    // other teardown code
}
void UsbHasDataToRead_returns_true_if_the_rx_buffer_has_data(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    FtHasDataToRead_StubbedReturnValue = true; // simulate rx buffer has data
    //
    //=====[ Operate and Test ]=====
    TEST_ASSERT_TRUE(UsbHasDataToRead());
}
void UsbHasDataToRead_returns_false_if_the_rx_buffer_is_empty(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    FtHasDataToRead_StubbedReturnValue = false; // simulate rx buffer is empty
    //
    //=====[ Operate and Test ]=====
    TEST_ASSERT_FALSE(UsbHasDataToRead());
}
void SetUp_UsbHasRoomToWrite(void){
    SetUpMock_UsbHasRoomToWrite();    // create the mock object to record calls
    // other setup code
}
void TearDown_UsbHasRoomToWrite(void){
    TearDownMock_UsbHasRoomToWrite();    // destroy the mock object
    // other teardown code
}
void UsbHasRoomToWrite_returns_true_if_the_tx_buffer_is_not_full(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    FtHasRoomToWrite_StubbedReturnValue = false; // simulate tx buffer is not full
    //
    //=====[ Operate and Test ]=====
    TEST_ASSERT_TRUE(UsbHasRoomToWrite);
}

void SetUp_UsbWrite(void){
    SetUpMock_UsbWrite();    // create the mock object to record calls
    // other setup code
}
void TearDown_UsbWrite(void){
    TearDownMock_UsbWrite();    // destroy the mock object
    // other teardown code
}
void UsbWrite_returns_the_number_of_bytes_sent(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    // FtBusTurnaround returns a bool
    FtBusTurnaround_StubbedReturnValue = true; // simulate tx buffer has room
    //
    // Simulate room for 6 more bytes.
    // FtWrite returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    bool ack_nak_sequence[] = {true, true, true, true, true, true, false};
    FtWrite_StubbedReturnValue = ack_nak_sequence;
    //=====[ Check test code for desired scenario ]=====
    uint16_t tx_buffer_byte_limit = sizeof(ack_nak_sequence)-1;
    TEST_ASSERT_EQUAL_UINT16(6, tx_buffer_byte_limit);
    //
    //=====[ Operate ]=====
    uint8_t write_buffer[5];
    uint16_t num_bytes_to_send = sizeof(write_buffer);
    TEST_ASSERT_LESS_THAN_UINT16_MESSAGE(tx_buffer_byte_limit, num_bytes_to_send,
        "Expected test scenario was the transmit buffer has enough room "
        "to send all the bytes in the write buffer, "
        "but the transmit buffer does not have enough room."
        );
    uint16_t num_bytes_sent = UsbWrite(write_buffer, num_bytes_to_send);
    //
    //=====[ Test ]=====
    TEST_ASSERT_EQUAL_UINT16(num_bytes_to_send, num_bytes_sent);
}
void UsbWrite_calls_FtWrite_for_each_byte_to_send(void)
{
    //  This test checks for two things at the same time:
        //- tests that UsbWrite calls FtWrite N times
        //    - checking for N calls is a check for the correct scenario
        //    - but this also makes sure the implementation does not make extra
        //    calls to FtWrite, i.e.:
        //    -[x] UsbWrite stops sending bytes if all bytes are sent
        //- tests that UsbWrite walks the write buffer
        //    - comparing expected and actual calls checks that the write buffer
        //    address is incremented
        //- note that both of these tests require comparing call lists
        //- to minimize mocking, I only stubbed calls that required injecting
        //the return value to mock-up the correct scenario
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    // FtBusTurnaround returns a bool
    FtBusTurnaround_StubbedReturnValue = true; // simulate tx buffer has room
    //
    // Simulate room for 6 more bytes.
    // FtWrite returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    bool ack_nak_sequence[] = {true, true, true, true, true, true, false};
    FtWrite_StubbedReturnValue = ack_nak_sequence;
    //=====[ Check test code for desired scenario ]=====
    uint16_t tx_buffer_byte_limit = sizeof(ack_nak_sequence)-1;
    TEST_ASSERT_EQUAL_UINT16(6, tx_buffer_byte_limit);
    //
    //=====[ Operate ]=====
    uint8_t write_buffer[4];
    uint16_t num_bytes_to_send = sizeof(write_buffer);
    TEST_ASSERT_LESS_THAN_UINT16_MESSAGE(tx_buffer_byte_limit, num_bytes_to_send,
        "Expected test scenario was the transmit buffer has enough room "
        "to send all the bytes in the write buffer, "
        "but the transmit buffer does not have enough room."
        );
    uint16_t num_bytes_sent = UsbWrite(write_buffer, num_bytes_to_send);
    //
    //=====[ Check test code for desired scenario ]=====
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(num_bytes_to_send, num_bytes_sent,
            "Expected test scenario was all bytes sent, "
            "but number of bytes in write buffer "
            "does not equal the number of bytes sent.");
    //
    //=====[ Set expectations ]=====
    Expect_FtBusTurnaround();
    // Expect FtWrite is called once for every byte in the buffer.
    uint16_t num_calls_to_FtWrite = 0;
    uint8_t *p_write_buffer = write_buffer;
    while (num_calls_to_FtWrite < num_bytes_to_send)
    {
        Expect_FtWrite(p_write_buffer++);
        num_calls_to_FtWrite++;
    }
    //=====[ Check test code for desired scenario ]=====
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(num_calls_to_FtWrite, num_bytes_sent,
            "Expected test scenario was all bytes sent, "
            "but number of calls to FtWrite "
            "does not equal the number of bytes sent.");
    //=====[ Test: was FtWrite called with the expected input arg ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );

}

void SetUp_UsbRead(void){
    SetUpMock_UsbRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_UsbRead(void){
    TearDownMock_UsbRead();    // destroy the mock object
    // other teardown code
}

//=====[ UsbRead should indicate data was read ]=====
void UsbRead_returns_0_if_there_was_no_data_to_read(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    // FtBusTurnaround returns a bool
    FtBusTurnaround_StubbedReturnValue = false; // simulate no bytes in rx buffer
    //
    // FtRead returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    bool ack_nak_sequence[] = {false};
    FtRead_StubbedReturnValue = ack_nak_sequence;
    //=====[ Check test code for desired scenario ]=====
    uint16_t num_bytes_in_buffer = sizeof(ack_nak_sequence)-1;
    TEST_ASSERT_EQUAL_UINT16(0, num_bytes_in_buffer);
    //
    // FtRead copies data from rx buffer to address of buffer passed in.
    uint8_t *rx_buffer = NULL;   // no data to read, this is never accessed
    FtRead_StubbedDataBusPtr = rx_buffer;
    //
    //=====[ Operate ]=====
    uint8_t *read_buffer = NULL; // no data to read; this is never accessed
    uint16_t num_bytes_read = UsbRead(read_buffer);
    //=====[ Test ]=====
    TEST_ASSERT_EQUAL_UINT16(0, num_bytes_read);
}
void UsbRead_returns_N_if_there_were_N_bytes_to_read(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    // FtBusTurnaround returns a bool
    FtBusTurnaround_StubbedReturnValue = true;  // bus is OK
    //
    // FtRead returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    bool ack_nak_sequence[] = {true, true, true, true, false};
    FtRead_StubbedReturnValue = ack_nak_sequence;
    //=====[ Check test code for desired scenario ]=====
    uint16_t num_bytes_in_buffer = sizeof(ack_nak_sequence)-1;
    TEST_ASSERT_EQUAL_UINT16(4, num_bytes_in_buffer);
    //
    // FtRead copies data from rx buffer to address of buffer passed in.
    uint8_t fake_rx_buffer[] = {0, 1, 2, 3};
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, sizeof(fake_rx_buffer));
    FtRead_StubbedDataBusPtr = fake_rx_buffer;
    //
    //=====[ Operate ]=====
    uint8_t read_buffer[num_bytes_in_buffer];
    uint16_t num_bytes_read = UsbRead(read_buffer);
    //=====[ Test ]=====
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, num_bytes_read);
}
//=====[ UsbRead should copy data to the read buffer input ]=====
void UsbRead_copies_bytes_to_the_input_read_buffer_address(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    // FtBusTurnaround returns a bool
    FtBusTurnaround_StubbedReturnValue = true;  // bus is OK
    //
    // FtRead returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    bool ack_nak_sequence[] = {true, true, true, false};
    FtRead_StubbedReturnValue = ack_nak_sequence;
    //=====[ Check test code for desired scenario ]=====
    uint16_t num_bytes_in_buffer = sizeof(ack_nak_sequence)-1;
    TEST_ASSERT_EQUAL_UINT16(3, num_bytes_in_buffer);
    //
    // FtRead copies data from rx buffer to address of buffer passed in.
    uint8_t fake_rx_buffer[] = {0, 1, 2};
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, sizeof(fake_rx_buffer));
    FtRead_StubbedDataBusPtr = fake_rx_buffer;
    //
    //=====[ Operate ]=====
    uint8_t read_buffer[num_bytes_in_buffer];
    uint16_t num_bytes_read = UsbRead(read_buffer);
    //=====[ Check test code for desired scenario ]=====
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, num_bytes_read);
    //=====[ Test ]=====
    TEST_ASSERT_EQUAL_UINT8_ARRAY(fake_rx_buffer, read_buffer, num_bytes_read);
}

//=====[ tests that check implementation details of UsbRead ]=====
void UsbRead_turns_LED_red_if_there_was_no_data_read(void) // sad path
{
    //=====[ Set expectations ]=====
    Expect_FtSendCommand(FtCmd_Read);
    Expect_FtBusTurnaround(); // false := error: nothing to read
    Expect_DebugLedTurnRedToShowError();  _MOCK_DEBUGLED_H;
    // exit before attempting to read any bytes from the buffer
    Expect_FtDeactivateInterface();
    //
    //=====[ Mock-up test scenario by defining return values ]=====
    FtBusTurnaround_StubbedReturnValue = false;
    //
    //=====[ Operate ]=====
    uint8_t *read_buffer = NULL; // no data to read; this is never accessed
    UsbRead(read_buffer);
    //
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void UsbRead_should_read_until_buffer_is_empty(void) // happy path
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
