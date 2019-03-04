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
// UsbHasDataToRead behavioral tests
    // [x] UsbHasDataToRead returns true if the rx buffer has data
    // [x] UsbHasDataToRead returns false if the rx buffer is empty
// UsbHasRoomToWrite behavioral tests
    // [x] UsbHasRoomToWrite returns true if the tx buffer is not full
    // [x] UsbHasRoomToWrite returns false if the tx buffer is full
// UsbRead behavioral tests
    // [x] UsbRead should indicate data was read
    //  - [x] UsbRead returns 0 if there was no data to read
    //  - [x] UsbRead returns N if there were N bytes to read
    // [x] UsbRead turns LED red if there was no data to read
    // [x] UsbRead copies bytes to the input read buffer
    //  - tests that UsbRead is incrementing the read buffer address
// UsbRead behavioral system tests
    // [ ] UsbRead copies bytes from MIOSIO to the input read buffer address
    //  - this is a system test, not a unit test because it cares about matching
    //    the value on MIOSIO
    //  - mock FtReadData? do not mock FtRead?
// UsbRead implementation details
    //=====[ sketch of UsbRead implementation details ]=====
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
    // [x] UsbRead sad path
    //  - Note *this is different* from testing that UsbRead returns 0
    //  - and from testing that UsbRead turns LED red.
    //  - This tests the implementation of the sad path.
    //  - If the implementation changes, this test fails, but UsbRead returns 0
    //    still passes.
    //  - That is *very helpful* to identify that the high-level behavior is
    //    still correct and that the test only fails because of an
    //    implementation change.
    // [x] UsbRead happy path
    //  - UsbRead stops reading bytes when the rx buffer is empty
    //  - test checks that UsbRead loops calling FtRead as long as it returns ACK
    //  - Note *this is different* from testing that UsbRead returns N bytes.
    //  - If this fails, but UsbRead returns N bytes still passes, it means the
    //    behavior is still correct but I changed the implementation.
// UsbWrite behavioral tests
    // [x] UsbWrite returns the number of bytes sent
    // [x] UsbWrite calls FtWrite for each byte to send
    //  - tests that UsbWrite is incrementing the write buffer address
    // [x] UsbWrite stops sending bytes if all bytes are sent
    // [x] UsbWrite stops sending bytes if the tx buffer is full
    // [x] UsbWrite turns LED red if the tx buffer was already full
    // [x] UsbWrite returns 0 if the tx buffer was already full
    // ~~[-] UsbWrite copies bytes from the input write buffer~~
    //  - this test is impossible: there is nothing to look at
    //  - see instead `UsbWrite calls FtWrite for each byte to send`
// UsbWrite behavioral system tests
    // [ ] UsbWrite copies bytes from the input write buffer to MIOSIO
    //  - this is a system test, not a unit test because it cares about matching
    //  the value on MIOSIO
// UsbWrite implementation details
    // [x] UsbWrite sad path
    // [x] UsbWrite happy path
    //
/* void SetUp_NothingForUsb(void){} */
/* void TearDown_NothingForUsb(void){} */

//=====[ UsbHasDataToRead behavioral tests ]=====
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

//=====[ UsbHasRoomToWrite behavioral tests ]=====
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
    FtHasRoomToWrite_StubbedReturnValue = true; // simulate tx buffer is not full
    //
    //=====[ Operate and Test ]=====
    TEST_ASSERT_TRUE(UsbHasRoomToWrite());
}
void UsbHasRoomToWrite_returns_false_if_the_tx_buffer_is_full(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    FtHasRoomToWrite_StubbedReturnValue = false; // simulate tx buffer is full
    //
    //=====[ Operate and Test ]=====
    TEST_ASSERT_FALSE(UsbHasRoomToWrite());
}

//=====[ UsbWrite behavioral tests ]=====
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
    uint8_t write_buffer[5]; uint16_t num_bytes_to_send = sizeof(write_buffer);
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
void UsbWrite_stops_sending_bytes_if_the_tx_buffer_is_full(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    // FtBusTurnaround returns a bool
    FtBusTurnaround_StubbedReturnValue = true; // simulate tx buffer has room
    //
    // Simulate room for 3 more bytes.
    // FtWrite returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    bool ack_nak_sequence[] = {true, true, true, false};
    FtWrite_StubbedReturnValue = ack_nak_sequence;
    //=====[ Check test code for desired scenario ]=====
    uint16_t tx_buffer_byte_limit = sizeof(ack_nak_sequence)-1;
    TEST_ASSERT_EQUAL_UINT16(3, tx_buffer_byte_limit);
    //=====[ Operate ]=====
    uint8_t write_buffer[4];
    uint16_t num_bytes_to_send = sizeof(write_buffer);
    TEST_ASSERT_GREATER_THAN_UINT16_MESSAGE(tx_buffer_byte_limit, num_bytes_to_send,
        "Expected test scenario was the transmit buffer "
        "does not have enough room to send all the bytes in the write buffer, "
        "but the transmit buffer does have enough room."
        );
    uint16_t num_bytes_sent = UsbWrite(write_buffer, num_bytes_to_send);
    //
    //=====[ Test ]=====
    TEST_ASSERT_EQUAL_UINT16(tx_buffer_byte_limit, num_bytes_sent);
}
void UsbWrite_turns_LED_red_if_the_tx_buffer_was_already_full(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    FtBusTurnaround_StubbedReturnValue = false; // simulate tx buffer is full
    //
    //=====[ Set expectations ]=====
    Expect_FtBusTurnaround();
    Expect_DebugLedTurnRedToShowError();
    //
    //=====[ Operate ]=====
    uint8_t write_buffer[1];
    uint16_t num_bytes_to_send = sizeof(write_buffer);
    UsbWrite(write_buffer, num_bytes_to_send);
    //
    //=====[ Test: compare call lists to see LED turned red ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void UsbWrite_returns_0_if_the_tx_buffer_was_already_full(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    FtBusTurnaround_StubbedReturnValue = false; // simulate tx buffer is full
    //
    //=====[ Operate ]=====
    uint8_t write_buffer[5]; uint16_t num_bytes_to_send = sizeof(write_buffer);
    uint16_t num_bytes_sent = UsbWrite(write_buffer, num_bytes_to_send);
    //=====[ Test ]=====
    TEST_ASSERT_EQUAL_UINT16(0, num_bytes_sent);
}

//=====[ UsbWrite implementation details ]=====
void SetUp_DetailsOf_UsbWrite(void){
    SetUpMock_DetailsOf_UsbWrite();    // create the mock object to record calls
    // other setup code
}
void TearDown_DetailsOf_UsbWrite(void){
    TearDownMock_DetailsOf_UsbWrite();    // destroy the mock object
    // other teardown code
}
void UsbWrite_happy_path_is_implemented_like_this(void) // happy path
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    FtBusTurnaround_StubbedReturnValue = true; // simulate tx buffer has room
    // Simulate room for 6 more bytes.
    // FtWrite returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    bool ack_nak_sequence[] = {true, true, true, true, true, true, false};
    FtWrite_StubbedReturnValue = ack_nak_sequence;
    //=====[ Check test code for desired scenario ]=====
    uint16_t tx_buffer_byte_limit = sizeof(ack_nak_sequence)-1;
    TEST_ASSERT_EQUAL_UINT16(6, tx_buffer_byte_limit);
    //
    //=====[ Operate ]=====
    uint8_t write_buffer[5]; uint16_t num_bytes_to_send = sizeof(write_buffer);
    uint16_t num_bytes_sent = UsbWrite(write_buffer, num_bytes_to_send);
    //=====[ Check test code for desired scenario ]=====
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(num_bytes_to_send, num_bytes_sent,
            "Expected test scenario was all bytes sent, "
            "but number of bytes in write buffer "
            "does not equal the number of bytes sent.");
    //
    //=====[ Set expectations ]=====
    Expect_FtSendCommand(FtCmd_Write);
    Expect_FtBusTurnaround();
    // Expect FtWrite is called once for every byte in the buffer.
    uint16_t num_calls_to_FtWrite = 0;
    uint8_t *p_write_buffer = write_buffer;
    while (num_calls_to_FtWrite < num_bytes_to_send)
    {
        Expect_FtWrite(p_write_buffer++);
        num_calls_to_FtWrite++;
    }
    Expect_FtDeactivateInterface();
    //
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void UsbWrite_sad_path_is_implemented_like_this(void) // happy path
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    FtBusTurnaround_StubbedReturnValue = false; // simulate tx buffer is full
    // Simulate room for 0 more bytes.
    // FtWrite returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    bool ack_nak_sequence[] = {false};
    FtWrite_StubbedReturnValue = ack_nak_sequence;
    //=====[ Check test code for desired scenario ]=====
    uint16_t tx_buffer_byte_limit = sizeof(ack_nak_sequence)-1;
    TEST_ASSERT_EQUAL_UINT16(0, tx_buffer_byte_limit);
    //
    //=====[ Operate ]=====
    uint8_t write_buffer[5]; uint16_t num_bytes_to_send = sizeof(write_buffer);
    uint16_t num_bytes_sent = UsbWrite(write_buffer, num_bytes_to_send);
    //=====[ Check test code for desired scenario ]=====
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(0, num_bytes_sent,
        "Expected test scenario was no bytes sent "
        "because the tx buffer is full, but number of bytes sent is not zero.");
    //
    //=====[ Set expectations ]=====
    Expect_FtSendCommand(FtCmd_Write);
    Expect_FtBusTurnaround();
    Expect_DebugLedTurnRedToShowError();
    Expect_FtDeactivateInterface();
    //
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

//=====[ UsbRead behavioral tests ]=====
void SetUp_UsbRead(void){
    SetUpMock_UsbRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_UsbRead(void){
    TearDownMock_UsbRead();    // destroy the mock object
    // other teardown code
}
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
void UsbReadN_reads_N_bytes_and_returns_num_bytes_read(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    // FtBusTurnaround returns a bool
    FtBusTurnaround_StubbedReturnValue = true;  // bus is OK
    //
    // FtRead returns a bool: ACK:=true, NAK:=false, num_bytes:=num_ACKs
    // Simulate scenario of CmdCfgLis reading 4 bytes.
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
    /* =====[ Operate ]===== */
    uint8_t read_buffer[num_bytes_in_buffer];
    uint16_t nbytes_read = UsbReadN(read_buffer, num_bytes_in_buffer);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, nbytes_read);
}
void UsbRead_turns_LED_red_if_there_was_no_data_to_read(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    //
    FtBusTurnaround_StubbedReturnValue = false; // simulate rx buffer is empty
    //
    //=====[ Set expectations ]=====
    Expect_FtBusTurnaround();
    Expect_DebugLedTurnRedToShowError();
    //
    //=====[ Operate ]=====
    uint8_t read_buffer[1];
    UsbRead(read_buffer);
    //
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void UsbRead_copies_bytes_to_the_input_read_buffer(void)
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

//=====[ UsbRead implementation details ]=====
void SetUp_DetailsOf_UsbRead(void){
    SetUpMock_DetailsOf_UsbRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_DetailsOf_UsbRead(void){
    TearDownMock_DetailsOf_UsbRead();    // destroy the mock object
    // other teardown code
}
void UsbRead_sad_path_is_implemented_like_this(void)
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
void UsbRead_happy_path_is_implemented_like_this(void) // happy path
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
    //
    //=====[ Mock-up test scenario by defining return values ]=====
    FtBusTurnaround_StubbedReturnValue = true;  // bus is OK
    FtRead_StubbedReturnValue = ack_nak_list;  // point at first list element
    //
    //=====[ Operate ]=====
    uint16_t actual_num_bytes_read = UsbRead(read_buffer);
    //
    //=====[ Test: UsbRead read the buffer until it was empty ]=====
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, actual_num_bytes_read);
    //=====[ Test: UsbRead followed its happy path ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

/* =====[ UsbReadOneByte behavioral tests ]===== */
void UsbReadOneByte_copies_the_next_available_byte_to_the_input_read_buffer(void)
{
    /* =====[ Setup ]===== */
    uint8_t expect_cmd = 0x07;
    /* =====[ Stub dependencies ]===== */
    FtBusTurnaround_StubbedReturnValue = true;  // bus is OK
    bool ack_nak_sequence[] = {true};
    FtRead_StubbedReturnValue = ack_nak_sequence;
    uint8_t fake_rx_buffer[] = {expect_cmd};
    FtRead_StubbedDataBusPtr = fake_rx_buffer;
    /* =====[ Operate ]===== */
    uint8_t cmd;
    uint8_t nbytes = UsbReadOneByte(&cmd);
    TEST_ASSERT_EQUAL(1, nbytes);
    TEST_ASSERT_EQUAL(expect_cmd, cmd);
}
void UsbReadOneByte_returns_0_if_there_are_no_bytes_to_read(void)
{
    /* =====[ Stub dependencies ]===== */
    FtBusTurnaround_StubbedReturnValue = false;  // bus is not OK
    /* =====[ Operate ]===== */
    uint8_t cmd;
    TEST_ASSERT_FALSE(UsbReadOneByte(&cmd));
}
void UsbReadOneByte_returns_1_if_there_is_at_least_one_byte_to_read(void)
{
    /* =====[ Stub dependencies ]===== */
    FtBusTurnaround_StubbedReturnValue = true;  // bus is not OK
    /* =====[ Operate ]===== */
    uint8_t cmd;
    TEST_ASSERT_TRUE(UsbReadOneByte(&cmd));
}
void UsbReadOneByte_example_readings_several_bytes(void)
{
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
    uint8_t const cfg_cmd = 0x07;
    uint8_t const bin_on = 0x06;
    uint8_t const gain_1x = 0x0B;
    uint8_t const all_rows = 0x0C;
    uint8_t fake_rx_buffer[] = {cfg_cmd, bin_on, gain_1x, all_rows};
    TEST_ASSERT_EQUAL_UINT16(num_bytes_in_buffer, sizeof(fake_rx_buffer));
    FtRead_StubbedDataBusPtr = fake_rx_buffer;
    //
    //=====[ Operate and Test ]=====
    uint8_t cmd;
    UsbReadOneByte(&cmd);
    TEST_ASSERT_EQUAL_HEX8(cfg_cmd, cmd);
    typedef struct {
        uint8_t bin_on_off;
        uint8_t gain_select;
        uint8_t row_select;
    } CfgParams_s;
    CfgParams_s cfg;
    UsbReadOneByte(&cfg.bin_on_off);
    TEST_ASSERT_EQUAL_HEX8(bin_on, cfg.bin_on_off);
    UsbReadOneByte(&cfg.gain_select);
    TEST_ASSERT_EQUAL_HEX8(gain_1x, cfg.gain_select);
    UsbReadOneByte(&cfg.row_select);
    TEST_ASSERT_EQUAL_HEX8(all_rows, cfg.row_select);
}
/* =====[ Jump Table Sandbox ]===== */
/* Functions of type `UsbCmd` take nothing and return nothing. */
void LookupCmd_returns_Nth_fn_for_Nth_key(void){
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL(CmdLedRed, LookupCmd(CmdLedRed_key));
    TEST_ASSERT_EQUAL(CmdLedGreen, LookupCmd(CmdLedGreen_key));
}
static jump_index const CmdBlackHat_key = 255; // out-of-bounds: return NULL ptr
void LookupCmd_returns_NULL_if_key_is_not_in_jump_table(void){
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_NULL(LookupCmd(CmdBlackHat_key));
}
void LookupCmd_example_calling_the_command(void){
    //TODO: erase this when I fix the DebugLed.h hardware interface.
    //=====[ Stupid Setup ]=====
    /* DebugLed.h does not follow the simple extern everything format. */
    uint8_t fake_ddr, fake_port, fake_pin; // compiler picks safe fake addresses
    uint8_t *DebugLed_port = &fake_port; uint8_t debug_led = 3; // test only needs these two
    DebugLedInit( &fake_ddr, DebugLed_port, &fake_pin, debug_led); // pass fakes to init
    //=====[ Setup ]=====
    *DebugLed_port = 0x00; // fake port with DebugLed pin green
    /* ------------------------------- */
    /* =====[ Operate ]===== */
    /* Note the parentheses to make it a function call */
    LookupCmd(CmdLedRed_key)(); // call the function returned by lookup
    /* ------------------------------- */
    //=====[ Test ]=====
    TEST_ASSERT_BIT_HIGH(debug_led, *DebugLed_port);
}
void LookupCmd_example_storing_the_returned_pointer(void){
    /* =====[ Setup ]===== */
    jump_index cmd;
    /* =====[ Operate ]===== */
    cmd = CmdLedRed_key;
    UsbCmd* CmdFn = LookupCmd(cmd);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL(CmdLedRed, CmdFn);
    /* =====[ Operate ]===== */
    cmd = CmdBlackHat_key;
    CmdFn = LookupCmd(cmd);
    /* =====[ Test ]===== */
    TEST_ASSERT_NULL(CmdFn);
}
/* TODO: write API for a caller of LookupCmd to respond to all command keys with: */
/* 0 (OK) */
/* or non-0 (error code) */
    /* If command was understood and everything followed a happy path, then first */
    /* byte sent back is always 0 (OK) followed by extra bytes if appropriate. */
    /* If first byte back is *not* zero, look at the error code and see how to */
    /* interpret what follows. */
    /* ---error codes--- */
    /* 1: command key is not recognized */
/* This implies I want to return a function pointer to let the caller send the */
/* appropriate response to the USB host. */
void UsbWriteStatusOk_tells_UsbHost_command_was_success(void)
{
    /* =====[ Setup ]===== */
    jump_index cmd = CmdCfgLis_key;
    /* =====[ Operate ]===== */
    /* StatusOk means the command is valid and was successfully carried out. */
    /* So LookupCmd returns a non-NULL function pointer. */
    TEST_ASSERT_NOT_NULL(LookupCmd(cmd));
    /* simBrd calls CmdFn() and execution enters CmdCfgLis() */
    /* Fake that CmdCfgLis() runs without any errors. */
    bool success = true;
    /* Get a value to unit test without going mock crazy. */
    bool status_sent = false;
    /* =====[ Operate ]===== */
    /* The conditional check shows how UsbWriteStatusOk is used in CmdCfgLis. */
    if (!success) { ; /* Placeholder for code to send error code to UsbHost.*/ }
    else // received valid command and parameters
    {
        ; // Placeholder for code that does the command.
        status_sent = UsbWriteStatusOk();
        ; // Placeholder for code that sends updated Lis cfg value to UsbHost.
    };
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE(status_sent);
}
void UsbWriteStatusInvalid_sends_error_byte_and_echoes_invalid_command(void)
{
    /* =====[ Setup ]===== */
    jump_index cmd = CmdBlackHat_key;
    /* =====[ Operate ]===== */
    /* StatusInvalid means the command is not valid. */
    /* So LookupCmd returns a NULL function pointer. */
    UsbCmd* CmdFn = LookupCmd(cmd);
    TEST_ASSERT_NULL(CmdFn);
    /* Get a value to unit test without going mock crazy. */
    uint8_t num_status_bytes_sent = 0;
    /* =====[ Operate ]===== */
    /* The conditional check shows how UsbWriteStatusInvalid is used in simBrd. */
    if (CmdFn == NULL) num_status_bytes_sent = UsbWriteStatusInvalid(cmd);
    else CmdFn();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL(2, num_status_bytes_sent);
}
void UsbWriteStatusBadArgs_sends_error_byte_and_echoes_invalid_command(void)
{
    /* =====[ Setup ]===== */
    jump_index cmd = CmdCfgLis_key;
    /* A mismatch status means the command is valid, but not its parameters. */
    /* So LookupCmd returns a non-NULL function pointer. */
    TEST_ASSERT_NOT_NULL(LookupCmd(cmd));
    /* simBrd calls CmdFn() and execution enters CmdCfgLis() */
    /* Fake that CmdCfgLis() has an error: CfgByesAreValid() returns false. */
    bool cfg_bytes_are_valid = false;
    /* Get a value to unit test without going mock crazy. */
    uint8_t num_status_bytes_sent = 0;
    /* =====[ Operate ]===== */
    /* Here is how UsbWriteStatusBadArgs is used in CmdCfgLis. */
    if (!cfg_bytes_are_valid) { num_status_bytes_sent = UsbWriteStatusBadArgs(cmd); }
    else {; /* received valid command and parameters, go configure the Lis */ }
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL(2,num_status_bytes_sent);
}
void UsbWriteStatusMissingArgs_sends_error_byte_and_echoes_invalid_command(void){
    /* =====[ Setup ]===== */
    jump_index cmd = CmdCfgLis_key;
    /* TimedOut means the command is valid, but a timer expired */
    /* before receiving the expected number of bytes. */
    /* So LookupCmd returns a non-NULL function pointer. */
    TEST_ASSERT_NOT_NULL(LookupCmd(cmd));
    /* simBrd calls CmdFn() and execution enters CmdCfgLis() */
    /* CmdCfgLis() calls UsbRead to read the four cfg bytes. */
    /* Execution enters UsbRead, but UsbRead timeouts and reads < 4 bytes. */
    /* Fake that UsbReadN() returns nbytes_read < 4. */
    bool nybtes_read_is_less_than_num_cfgbytes = true;
    /* Get a value to unit test without going mock crazy. */
    uint8_t num_status_bytes_sent = 0;
    /* =====[ Operate ]===== */
    /* Here is how UsbWriteStatusMissingArgs is used in CmdCfgLis. */
    if (nybtes_read_is_less_than_num_cfgbytes)
    {
        num_status_bytes_sent = UsbWriteStatusMissingArgs(CmdCfgLis_key);
    }
    else {; /* number of bytes is correct, go check the bytes are valid */ }
    TEST_ASSERT_EQUAL(2,num_status_bytes_sent);
}
void LookupCmd_sad_example_using_UsbWriteStatus_API(void){
    /* =====[ Setup ]===== */
    jump_index cmd = CmdBlackHat_key; // receive an invalid command
    /* =====[ Operate Example of Invalid Command (no test here) ]===== */
    UsbCmd* CmdFn = LookupCmd(cmd);
    if (CmdFn == NULL) // sad
    {
        /* Send two bytes: error-code and cmd */
        UsbWriteStatusInvalid(cmd);
    }
    else CmdFn();
}
void LookupCmd_happy_example_using_UsbWriteStatus_API(void){
    /* =====[ Setup ]===== */
    jump_index cmd = CmdLedRed_key;
    /* =====[ Operate Example of Valid Command (no test here) ]===== */
    UsbCmd* CmdFn = LookupCmd(cmd);
    if (CmdFn == NULL) UsbWriteStatusInvalid(cmd);
    else CmdFn();
    /* It is the CmdFn() responsibility to send UsbWriteStatusOk() at the end of a */
    /*     successful command (or some other code). */
    /* If a CmdFn() sends additional data, it must send UsbWriteStatusOk() prior to */
    /* sending the requested data. */
}
void CmdCfgLis_returns_StatusOk_and_echoes_back_the_4_cfg_bytes(void){
    /* =====[ Setup ]===== */
    jump_index cmd = CmdCfgLis_key;
    /* =====[ Operate ]===== */
    UsbCmd* CmdFn = LookupCmd(cmd);
     // Make sure the command is in the jump table.
    TEST_ASSERT_NOT_NULL(CmdFn);
    if (CmdFn == NULL) UsbWriteStatusInvalid(cmd); // just showing the test as example
    else CmdFn(); // This is `CmdCfgLis()`
    //
    /* =====[ Test ]===== */
    //
    /* CmdFn() sends additional data, it must send UsbWriteStatusOk() prior to */
    /* sending the requested data. */
    /* [-] How do I test that this wrote StatusOK and then echoed the 4 bytes? */
    /*     I can check how many times UsbWrite is called if I mock it out. */
    /* Nah, do systems-level tests for this using Python script. */
}
void CmdCfgLis_returns_StatusMismatch_if_cfg_bytes_are_invalid(void)
{
    TEST_FAIL_MESSAGE("Implement test.");
}
void CmdCfgLis_sends_cfg_to_mBrd_and_reads_back_new_cfg_before_reporting_StatusOk(void)
{
    TEST_FAIL_MESSAGE("Functionality not implemented yet.");
}
void CfgTurnsOffAllPixels_returns_true_if_cfg_turns_off_all_pixels(void)
{
    /* =====[ Setup ]===== */
    /* This is the special case of invalid config: all pixels off. */
    /* Pixel on/off bits are all zero. Test with all bits zero. */
    uint8_t const invalid_cfg[] = {0x00, 0x00, 0x00, 0x00};
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE(CfgTurnsOffAllPixels(invalid_cfg));
}
void CfgTurnsOffAllPixels_ignores_the_3LSB_and_4MSB_of_cfg(void)
{
    /* =====[ Setup ]===== */
    /* This is the special case invalid config. */
    /* Pixel on/off bits are all zero. Test with don't-care bits all one. */
    uint8_t const invalid_cfg[] = {0xF0, 0x00, 0x00, 0x07};
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE(CfgTurnsOffAllPixels(invalid_cfg));
}
void CfgTurnsOffAllPixels_returns_false_if_any_pixels_are_on(void)
{
    /* =====[ Setup ]===== */
    /* There are many (2^25-1) ways for 1 or more of 25 bits to be 1. */
    /* Just test a few. */
    uint8_t const cfg_with_1_pixel_on[] = {0xF1, 0x00, 0x00, 0x07};
    uint8_t const cfg_with_2_pixels_on[] = {0xF1, 0x01, 0x00, 0x07};
    uint8_t const cfg_with_3_pixels_on[] = {0xF0, 0x00, 0x03, 0x17};
    /* =====[ Test ]===== */
    TEST_ASSERT_FALSE(CfgTurnsOffAllPixels(cfg_with_1_pixel_on));
    TEST_ASSERT_FALSE(CfgTurnsOffAllPixels(cfg_with_2_pixels_on));
    TEST_ASSERT_FALSE(CfgTurnsOffAllPixels(cfg_with_3_pixels_on));
}
void XOR_returns_true_if_a_is_true_and_b_is_false(void)
{
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(XOR(true,false));
    TEST_ASSERT_TRUE(XOR(false,true));
    TEST_ASSERT_FALSE(XOR(false,false));
    TEST_ASSERT_FALSE(XOR(true,true));
}
void CfgTurnsRowPartiallyOn_returns_false_if_cfg_turns_on_all_of_row1(void)
{
    /* =====[ Setup ]===== */
    uint8_t row1 = 1;
    /*                 xxxx5432154321543215432154321ggb */
    /* row1        = 0b00000000100001000010000100001001 */
    uint8_t const cfg_row_all_on[] = {0x00, 0x84, 0x21, 0x09};
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE( CfgTurnsRowPartiallyOn(cfg_row_all_on, row1) );
}
void CfgTurnsRowPartiallyOn_returns_true_if_cfg_turns_on_part_of_row1(void)
{
    /* =====[ Setup ]===== */
    uint8_t row1 = 1;
    /*                 xxxx5432154321543215432154321ggb */
    /* row1        = 0b00000000000001000010000100000001 */
    uint8_t const cfg_row_partly_on[] = {0x00, 0x04, 0x21, 0x01};
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE( CfgTurnsRowPartiallyOn(cfg_row_partly_on, row1) );

}
void CfgTurnsRowPartiallyOn_returns_true_if_cfg_turns_on_part_of_row5(void)
{
    /* Row 5 is important to test to catch for indexing off-by-one errors. */
    /* Row_number (not zero-indexed) indexes into */
    /* the array of row_masks (zero-indexed because it's an array). */
    /* =====[ Setup ]===== */
    uint8_t row5 = 5;
    uint8_t const cfg_row_partly_on[] = {0x00, 0x42, 0x10, 0x81};
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE( CfgTurnsRowPartiallyOn(cfg_row_partly_on, row5) );
}
void CfgTurnsRowPartiallyOn_returns_true_if_row_number_is_out_bounds(void)
{
    /* This is to make sure my own programming mistakes don't result in a core */
    /* dump. But I should be careful with this. It could turn into a hard to */
    /* trace bug if everything is compiling but CfgBytesAreValid seems to */
    /* misbehave, all because I accidentally passed it the wrong row number. */
    /* Not sure there's a better solution though. */

    /* =====[ Setup ]===== */
    uint8_t row_whoops = 6;
    /*                 xxxx5432154321543215432154321ggb */
    /* row1        = 0b00000000100001000010000100001001 */
    uint8_t const cfg_row_all_on[] = {0x00, 0x84, 0x21, 0x09};
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        CfgTurnsRowPartiallyOn(cfg_row_all_on, row_whoops),
        "Expected false because cfg turns on *all* of row 1. Received true because "
        "row number is out of bounds. This is the way CfgTurnsRowPartiallyOn() "
        "deals with out of bounds row numbers."
        );
}

void CfgBytesAreValid_checks_against_all_255_valid_configs(void)
{
    /* Plan of attack: check a handful of valid and invalid cases. */
    /* =====[ Operate and Test ]===== */
    /* CfgBytes are invalid if they turn off *all* rows. */
    uint8_t const invalid_cfg[] = {0x00, 0x00, 0x00, 0x00};
    TEST_ASSERT_FALSE(CfgBytesAreValid(invalid_cfg));
    /* Here are some standard valid configs. */
    uint8_t const all_rows_on[] = {0x0F, 0xFF, 0xFF, 0xF9};
    TEST_ASSERT_TRUE_MESSAGE(CfgBytesAreValid(all_rows_on),"all_rows_on");
    uint8_t const row_1_on[] = {0x00, 0x84, 0x21, 0x09};
    TEST_ASSERT_TRUE(CfgBytesAreValid(row_1_on));
    uint8_t const row_2_on[] = {0x01, 0x08, 0x42, 0x11};
    TEST_ASSERT_TRUE(CfgBytesAreValid(row_2_on));
    uint8_t const row_3_on[] = {0x02, 0x10, 0x84, 0x21};
    TEST_ASSERT_TRUE(CfgBytesAreValid(row_3_on));
    uint8_t const row_4_on[] = {0x04, 0x21, 0x08, 0x41};
    TEST_ASSERT_TRUE(CfgBytesAreValid(row_4_on));
    uint8_t const row_5_on[] = {0x08, 0x42, 0x10, 0x81};
    TEST_ASSERT_TRUE(CfgBytesAreValid(row_5_on));
    /* Here is a valid config that is not the default all rows on. */
    uint8_t const row_1_2_3_on[] = {0x03, 0x9C, 0xE7, 0x39};
    TEST_ASSERT_TRUE(CfgBytesAreValid(row_1_2_3_on));
    /* Here are some invalid configs. */
    uint8_t const invalid_cfg1[] = {0x0F, 0x83, 0xE0, 0xF9};
    TEST_ASSERT_FALSE(CfgBytesAreValid(invalid_cfg1));
    uint8_t const invalid_cfg2[] = {0x0F, 0xFF, 0xE0, 0x01};
    TEST_ASSERT_FALSE(CfgBytesAreValid(invalid_cfg2));
    uint8_t const invalid_cfg3[] = {0x00, 0x03, 0xFF, 0xF9};
    TEST_ASSERT_FALSE(CfgBytesAreValid(invalid_cfg3));
}
