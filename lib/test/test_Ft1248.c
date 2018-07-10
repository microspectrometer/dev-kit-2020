#include "Ft1248.h"
#include "test_Ft1248_MockUps.h"
#include "test_Ft1248.h"
#include "fake/Ft1248-Hardware.h"   // fake the io pins in avr/io.h
#include "mock_Ft1248.h"            // mocked version of DOF lib
#include <Mock.h>                   // RanAsHoped, WhyDidItFail
//#include <ReturnValues.h>           // StubReturnsFalse, StubReturnsTrue
#include <unity.h>                  // all the TEST_blah macros
#include "ReadWriteBits.h"

//=====[ List of tests to write ]=====
    // *Inactive state* control signals from the FT1248 slave:
    // [x] FtHasDataToRead() returns true if MISO is low
    // [x] FtHasDataToRead returns false if MISO is high
    //     - FtHasDataToRead is an alias for FtIsBusOk
    // [x] FtHasRoomToWrite() returns true if MIOSIO bit 0 is low
    // [x] FtHasRoomToWrite() returns false if MIOSIO bit 0 is high
    // - in the *Active bus state* both of these signals are on MISO
    // - during the *Active bus state* check MISO with FtIsBusOk()
    //
    // The rest of the functionality is for the *Active bus state*.
    // [x] FtActivateInterface() pulls SS low.
    // [x] FtPushData() pulls SCK high.
    // [x] FtLetMasterDriveBus() configures MIOSIO port for MCU output.
    // [x] FtOutputByte(cmd) outputs a byte on port MIOSIO.
    // [x] FtPullData() pulls SCK low.
    // [x] FtSendCommand(cmd) handles the entire command phase.
    //     FtSendCommand(FtCmd_Read) should:
    //     - FtActivateInterface()    (SS low)
    //     - FtPushData()             (SCK high)
    //     - FtLetMasterDriveBus()    (cfg MIOSIO[0:7] as output pins)
    //     - FtOutputByte(FtCmd_Read) (Master outputs the read command on MIOSIO)
    //     - FtPullData()             (SCK low)
    // [x] FtLetSlaveDriveBus() configures MIOSIO port for MCU input.
    // [x] FtIsBusOk returns true if MISO is low
    // [x] FtIsBusOk returns false if MISO is high
    // [x] FtBusTurnaround handles the entire bus turnaround.
    //     FtBusTurnaround should:
    //     - FtLetSlaveDriveBus()   (MIOSIO becomes inputs)
    //     - FtPushData(): SCK high, slave drives MISO with RxBufferEmpty
    //     - FtPullData(): SCK low, now its ok to pull data
    //     - FtIsBusOk(): master reads MISO
    //     happy path:
    //     if MISO says BusIsOk, it means there is data to read,
    //     go ahead and do the read
    // [x] FtBusTurnaround returns false if not ok to proceed
    //     - FtIsBusOK(): it is ok to proceed if this returns true
    //     - for a read, this means there is still data to read
    //     - for a write, this means there is room to write
    // [x] FtReadData() returns the value on MIOSIO pins
    // [x] FtDeactivateInterface() pulls SS high
    // [x] FtRead reads bytes from MIOSIO:
    //     FtRead should:
    //     - FtPushData(): SCK high, slave drives Miosio with data and MISO with ACK
    //     - FtPullData(): SCK low, ok to pull data
    //     - FtIsBusOk(): check MISO for ACK/NAK
    //     if MISO is an ACK, pull from Miosio
    //     - FtReadData(): store value on Miosio
    // - FtRead happy path:
    // [x] FtRead should store data if slave sends ACK
    // [x] FtRead should return true if slave sends ACK
    // - FtRead sad path:
    // [x] FtRead should not store data if slave sends NAK
    // [x] FtRead should return false if slave sends NAK
// void SetUp_NothingForFt1248(void){}
// void TearDown_NothingForFt1248(void){}

//=====[ Develop low-level functions ]=====
void SetUp_FtPorts(void)
{
    *Ft1248_port = 0x00;
    *FtMiosio_ddr = 0x00;
    *FtMiosio_port = 0x00;
    *Ft1248_pin   = *Ft1248_port;   // simulate AVR `pin` register
    *FtMiosio_pin = *FtMiosio_port; // simulate AVR `pin` register
}
void TearDown_FtPorts(void)
{
    *Ft1248_port = 0x00;
    *FtMiosio_ddr = 0x00;
}
void FtHasDataToRead_returns_true_if_MISO_is_low(void)
{
    //=====[ Setup ]=====
    ClearBit(Ft1248_port, Ft1248_Miso);
    *Ft1248_pin = *Ft1248_port;  // simulate AVR `pin` register
    //=====[ Operate and Test ]=====
    TEST_ASSERT_TRUE(FtHasDataToRead());
}
void FtHasDataToRead_returns_false_if_MISO_is_high(void)
{
    //=====[ Setup ]=====
    SetBit(Ft1248_port, Ft1248_Miso);
    *Ft1248_pin = *Ft1248_port;  // simulate AVR `pin` register
    //=====[ Operate and Test ]=====
    TEST_ASSERT_FALSE(FtHasDataToRead());
}
void FtHasRoomToWrite_returns_true_if_MIOSIO_bit_0_is_low(void)
{
    TEST_ASSERT_TRUE(FtHasRoomToWrite());
}
void FtHasRoomToWrite_returns_false_if_MIOSIO_bit_0_is_high(void)
{
    //=====[ Setup ]=====
    SetBit(FtMiosio_port, FtMiosio0);
    *FtMiosio_pin = *FtMiosio_port; // simulate AVR `pin` register
    //=====[ Operate and Test ]=====
    TEST_ASSERT_FALSE(FtHasRoomToWrite());
}
void FtActivateInterface_pulls_SS_low(void)
{
    //=====[ Setup ]=====
    SetBit(Ft1248_port, Ft1248_Ss);
    //=====[ Operate ]=====
    FtActivateInterface();
    //=====[ Test ]=====
    uint8_t io_port = *Ft1248_port;
    TEST_ASSERT_BIT_LOW(Ft1248_Ss, io_port);
}
void FtPushData_pulls_SCK_high(void)
{
    //=====[ Operate ]=====
    FtPushData();
    //=====[ Test ]=====
    uint8_t io_port = *Ft1248_port;
    TEST_ASSERT_BIT_HIGH(Ft1248_Sck, io_port);
}
void FtLetMasterDriveBus_configures_MIOSIO_port_for_MCU_output(void)
{
    //=====[ Setup ]=====
    uint8_t expected_ddr = 0xFF;
    //=====[ Operate ]=====
    FtLetMasterDriveBus();
    //=====[ Test ]=====
    uint8_t actual_ddr = *FtMiosio_ddr; // value in data direction register
    TEST_ASSERT_EQUAL_HEX8( expected_ddr, actual_ddr );
}
void FtOutputByte_outputs_a_byte_on_port_MIOSIO(void)
{
    //TEST_FAIL_MESSAGE("Implement test.");
    //=====[ Setup ]=====
    uint8_t byte_to_write = 0xAB;
    uint8_t expected_miosio = byte_to_write;
    //=====[ Operate ]=====
    FtOutputByte(byte_to_write);
    //=====[ Test ]=====
    uint8_t actual_miosio = *FtMiosio_port; // value in miosio port
    TEST_ASSERT_EQUAL_HEX8( expected_miosio, actual_miosio );
}
void FtPullData_pulls_SCK_low(void)
{
    //=====[ Setup ]=====
    SetBit(Ft1248_port, Ft1248_Sck);
    //=====[ Operate ]=====
    FtPullData();
    //=====[ Test ]=====
    uint8_t io_port = *Ft1248_port;
    TEST_ASSERT_BIT_LOW(Ft1248_Sck, io_port);
}

//=====[ Develop the Command Phase ]=====
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
    //=====[ Set expectations ]=====
    Expect_FtActivateInterface();
    Expect_FtPushData();
    Expect_FtLetMasterDriveBus();
    Expect_FtOutputByte(FtCmd_Read);
    Expect_FtPullData();
    //=====[ Operate ]=====
    FtSendCommand(FtCmd_Read);
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

//=====[ Develop the Bus Turnaround Phase ]=====
void SetUp_FtBusTurnaround(void){
    SetUpMock_FtBusTurnaround();    // create the mock object to record calls
    // other setup code
}
void TearDown_FtBusTurnaround(void){
    TearDownMock_FtBusTurnaround();    // destroy the mock object
    // other teardown code
}
void FtBusTurnaround_returns_true_if_ok_to_proceed_with_command(void)
{
    //=====[ Mock-up values returned by stubbed functions]=====
    FtIsBusOk_StubbedReturnValue = true;
    //=====[ Set list of expected calls ]=====
    Expect_FtLetSlaveDriveBus();
    Expect_FtPushData();
    Expect_FtPullData();
    Expect_FtIsBusOk();
    //=====[ Operate ]=====
    bool ok_to_proceed = FtBusTurnaround();
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
    TEST_ASSERT_TRUE(ok_to_proceed);
}
void FtBusTurnaround_returns_false_if_not_ok_to_proceed(void)
{
    //=====[ Mock-up values returned by stubbed functions]=====
    FtIsBusOk_StubbedReturnValue = false;
    //=====[ Set list of expected calls ]=====
    Expect_FtLetSlaveDriveBus();
    Expect_FtPushData();
    Expect_FtPullData();
    Expect_FtIsBusOk();
    //=====[ Operate ]=====
    bool ok_to_proceed = FtBusTurnaround();
    //=====[ Test ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
    TEST_ASSERT_FALSE(ok_to_proceed);
}
void FtLetSlaveDriveBus_configures_MIOSIO_port_for_MCU_input(void)
{
    //=====[ Setup ]=====
    *FtMiosio_ddr = 0xFF;
    uint8_t expected_ddr = 0x00;
    //=====[ Operate ]=====
    FtLetSlaveDriveBus();
    //=====[ Test ]=====
    uint8_t actual_ddr = *FtMiosio_ddr; // value in data direction register
    TEST_ASSERT_EQUAL_HEX8( expected_ddr, actual_ddr );
}
void FtIsBusOk_returns_true_if_MISO_is_low(void)
{
    //=====[ Setup ]=====
    ClearBit(Ft1248_port, Ft1248_Miso);
    *Ft1248_pin = *Ft1248_port;  // simulate AVR `pin` register
    //=====[ Operate and Test ]=====
    TEST_ASSERT_TRUE(FtIsBusOk());
}
void FtIsBusOk_returns_false_if_MISO_is_high(void)
{
    //=====[ Setup ]=====
    SetBit(Ft1248_port, Ft1248_Miso);
    *Ft1248_pin = *Ft1248_port;  // simulate AVR `pin` register
    //=====[ Operate and Test ]=====
    TEST_ASSERT_FALSE(FtIsBusOk());
}

//=====[ Develop the Read Phase ]=====
void SetUp_FtRead(void){
    SetUpMock_FtRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_FtRead(void){
    TearDownMock_FtRead();    // destroy the mock object
    // other teardown code
}
void FtRead_does_not_write_to_mem_and_returns_false_if_NAK(void)
{
    //=====[ Setup ]=====
    uint8_t byte_from_slave = 0xFF;  // garbage pushed by slave
    uint8_t host_msg        = 0x00;  // original value in mem
    uint8_t * host_msg_ptr = &host_msg;  // address passed to FtRead
    // assert that byte_from_slave is different from the value in memory
    TEST_ASSERT_NOT_EQUAL(*host_msg_ptr, byte_from_slave);
    // expect value in mem is unchanged after read
    uint8_t expected_mem_value = host_msg;
    //=====[ Mock-up values returned by stubbed functions]=====
    FtIsBusOk_StubbedReturnValue = false;
    FtReadData_StubbedReturnValue = byte_from_slave;
    //=====[ Set expected call list ]=====
    Expect_FtPushData();
    Expect_FtPullData();
    Expect_FtIsBusOk();
    //=====[ Operate ]=====
    bool byte_was_valid = FtRead(host_msg_ptr);
    //=====[ Test call list ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
    //=====[ Test that memory is not written ]=====
    TEST_ASSERT_EQUAL_HEX8(expected_mem_value, *host_msg_ptr);
    //=====[ Test that FtRead returns false ]=====
    TEST_ASSERT_FALSE(byte_was_valid);
}
void FtRead_should_write_to_mem_and_return_true_if_ACK(void)
{
    //=====[ Setup ]=====
    uint8_t byte_from_slave = 0x23;  // data pushed by slave
    uint8_t host_msg        = 0x00;  // original value in mem
    uint8_t * host_msg_ptr = &host_msg;  // address passed to FtRead
    // assert that byte_from_slave is different from the value in memory
    TEST_ASSERT_NOT_EQUAL(*host_msg_ptr, byte_from_slave);
    // expect value in mem is written with byte_from_slave after read
    uint8_t expected_mem_value = byte_from_slave;
    //=====[ Mock-up values returned by stubbed functions]=====
    FtIsBusOk_StubbedReturnValue = true;  // simulate ACK from slave
    FtReadData_StubbedReturnValue = byte_from_slave;
    //=====[ Set expected call list ]=====
    Expect_FtPushData();
    Expect_FtPullData();
    Expect_FtIsBusOk();
    Expect_FtReadData();
    //=====[ Operate ]=====
    bool byte_was_valid = FtRead(host_msg_ptr);
    //=====[ Test call list ]=====
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
    //=====[ Test that memory is written ]=====
    TEST_ASSERT_EQUAL_HEX8(expected_mem_value, *host_msg_ptr);
    //=====[ Test that FtRead returns true ]=====
    TEST_ASSERT_TRUE(byte_was_valid);
}
void FtReadData_returns_the_value_on_MIOSIO_pins(void)
{
    //=====[ Setup ]=====
    uint8_t expected_byte = 0xBE;
    *FtMiosio_port = expected_byte;
    *FtMiosio_pin  = *FtMiosio_port;  // simulate AVR `pin` register
    //=====[ Operate and Test ]=====
    TEST_ASSERT_EQUAL_HEX8( expected_byte, FtReadData() );
}
void FtDeactivateInterface_pulls_SS_high(void)
{
    //=====[ Operate ]=====
    FtDeactivateInterface();
    //=====[ Test ]=====
    uint8_t io_port = *Ft1248_port;
    TEST_ASSERT_BIT_HIGH(Ft1248_Ss, io_port);
}
