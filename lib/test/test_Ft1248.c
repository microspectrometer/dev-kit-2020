#include "Ft1248.h"
#include "test_Ft1248_MockUps.h"
#include "test_Ft1248.h"
#include "fake/Ft1248-Hardware.h"   // fake the io pins in avr/io.h
#include "mock_Ft1248.h"            // mocked version of DOF lib
#include <Mock.h>                   // RanAsHoped, WhyDidItFail
#include <unity.h>                  // all the TEST_blah macros
#include "ReadWriteBits.h"

//=====[ List of tests to write ]=====
    // [x] FtActivateInterface() pulls SS low.
    // [x] FtPushData() pulls SCK high.
    // [x] FtLetMasterDriveBus() configures MIOSIO port for MCU output.
    // [x] FtOutputByte(cmd) outputs a byte on port MIOSIO.
    // [x] FtPullData() pulls SCK low.
    // FtSendCommand(cmd) handles the entire command phase.
    // [x] FtSendCommand(FtCmd_Read) should
    //     handle the entire command phase for the Read command:
    //     [x] FtActivateInterface()    (SS low)
    //     [x] FtPushData()             (SCK high)
    //     [x] FtLetMasterDriveBus()    (cfg MIOSIO[0:7] as output pins)
    //     [x] FtOutputByte(FtCmd_Read) (Master outputs the read command on MIOSIO)
    //     [x] FtPullData()             (SCK low)
    // [ ] BusTurnaround should:
    //     [ ] FtLetSlaveDriveBus() configures MIOSIO port for MCU input.
    //     [ ] FtPushData(): SCK high, slave drives MISO with RxBufferEmpty
    //     [ ] FtPullData(): SCK low, now its ok to pull data
    //     [ ] FtIsBusOk(): master reads MISO
    //     if MISO says BusIsOk, do the read
    // [ ] Read should:
    //     [ ] FtPushData(): SCK high, slave drives Miosio with data and MISO with ACK
    //     [ ] FtPullData(): SCK low, ok to pull data
    //     [ ] FtIsBusOk(): check MISO for ACK/NAK
    //     if MISO is an ACK, pull from Miosio
    //     [ ] FtReadData(): store value on Miosio
    //     loop Read until a NAK or master is done reading
    //     if MISO is a NAK, return an error (ill-defined so far)
    // [ ] DoneReading should:
    //     [ ] FtPullData(): final OK to pull signal
    //     [ ] FtIsBusOk(): final check for ACK/NAK
    //     [ ] FtReadData(): final store of Miosio
    //     leave SCK low (it went low on FtPullData())
    //     [ ] FtDeactivateInterface(): pull SS high
// void SetUp_NothingForFt1248(void){}
// void TearDown_NothingForFt1248(void){}

void SetUp_FtPorts(void)
{
    *Ft1248_port = 0x00;
    *FtMiosio_ddr = 0x00;
}
void TearDown_FtPorts(void)
{
    *Ft1248_port = 0x00;
    *FtMiosio_ddr = 0x00;
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
    Expect_FtLetMasterDriveBus();
    Expect_FtOutputByte(FtCmd_Read);
    Expect_FtPullData();
    //=====[ Operate ]=====
    FtSendCommand(FtCmd_Read);
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

