#include "test_Bridge.h"
#include "Bridge.h"         // lib under test
/* ---Unit Test Framework--- */
#include "unity.h"          // unity macros: TEST_BLAH
#include <Mock.h>           // RanAsHoped, WhyDidItFail (mocks record calls/args)
/* ---Project Lib Dependencies--- */
#include "BiColorLed.h"
#include "DebugLeds.h"      // why does this lib exist?
#include "Spi.h"
/* ---Mock Out Libs That Read Hardware Registers--- */
#include "mock_Spi.h"

/** `status_led` is defined in `fake/BiColorLed-Hardware.h`. */
/* I cannot simply include that header because the test suite */
/* has multiple test translation units. */
/* If the Usb and Bicolor tests both include the */
/* BiColorLed-Hardware header, then `status_led` is defined */
/* multiple times: once in `test_Usb.o` and once in `test_BiColor.o`. */
extern uint8_t const status_led;

/* =====[ Jump Table Sandbox ]===== */
/* Functions of type `BridgeCmd` take nothing and return nothing. */
void LookupBridgeCmd_returns_Nth_fn_for_Nth_key(void){
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL(BridgeLedRed, LookupBridgeCmd(BridgeLedRed_key));
    TEST_ASSERT_EQUAL(BridgeLedGreen, LookupBridgeCmd(BridgeLedGreen_key));
}
static bridge_cmd_key const CmdBlackHat_key = 255; // out-of-bounds: return NULL ptr
void LookupBridgeCmd_returns_NULL_if_key_is_not_in_jump_table(void){
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_NULL(LookupBridgeCmd(CmdBlackHat_key));
}
void LookupBridgeCmd_example_calling_the_command(void){
    //=====[ Setup ]=====
    BiColorLedGreen(status_led);
    /* *BiColorLed_port = 0x00; // fake port with status_led pin green */
    /* ------------------------------- */
    /* =====[ Operate ]===== */
    /* Note the parentheses to make it a function call */
    LookupBridgeCmd(BridgeLedRed_key)(); // call the function returned by lookup
    /* ------------------------------- */
    //=====[ Test ]=====
    TEST_ASSERT_BIT_HIGH(status_led, *BiColorLed_port);
}
void LookupBridgeCmd_example_storing_the_returned_pointer(void){
    /* =====[ Setup ]===== */
    bridge_cmd_key cmd;
    /* =====[ Operate ]===== */
    cmd = BridgeLedRed_key;
    BridgeCmd* CmdFn = LookupBridgeCmd(cmd);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL(BridgeLedRed, CmdFn);
    /* =====[ Operate ]===== */
    cmd = CmdBlackHat_key;
    CmdFn = LookupBridgeCmd(cmd);
    /* =====[ Test ]===== */
    TEST_ASSERT_NULL(CmdFn);
}
/* API for a caller of LookupBridgeCmd to respond to all command keys with: */
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
    bridge_cmd_key cmd = BridgeCfgLis_key;
    /* =====[ Operate ]===== */
    /* StatusOk means the command is valid and was successfully carried out. */
    /* So LookupBridgeCmd returns a non-NULL function pointer. */
    TEST_ASSERT_NOT_NULL(LookupBridgeCmd(cmd));
    /* simBrd calls CmdFn() and execution enters CmdCfgLis() */
    /* Fake that CmdCfgLis() runs without any errors. */
    bool success = true;
    /* Get a value to unit test without going mock crazy. */
    uint8_t nbytes_sent = 0;
    /* =====[ Operate ]===== */
    /* The conditional check shows how UsbWriteStatusOk is used in CmdCfgLis. */
    if (!success) { ; /* Placeholder for code to send error code to UsbHost.*/ }
    else // received valid command and parameters
    {
        ; // Placeholder for code that does the command.
        nbytes_sent = UsbWriteStatusOk(cmd);
        ; // Placeholder for code that sends updated Lis cfg value to UsbHost.
    };
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL(2, nbytes_sent);
}
void UsbWriteStatusInvalid_sends_error_byte_and_echoes_invalid_command(void)
{
    /* =====[ Setup ]===== */
    bridge_cmd_key cmd = CmdBlackHat_key;
    /* =====[ Operate ]===== */
    /* StatusInvalid means the command is not valid. */
    /* So LookupBridgeCmd returns a NULL function pointer. */
    BridgeCmd* CmdFn = LookupBridgeCmd(cmd);
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
    bridge_cmd_key cmd = BridgeCfgLis_key;
    /* A bad args status means the command is valid, but not its parameters. */
    /* So LookupBridgeCmd returns a non-NULL function pointer. */
    TEST_ASSERT_NOT_NULL(LookupBridgeCmd(cmd));
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
    bridge_cmd_key cmd = BridgeCfgLis_key;
    /* TimedOut means the command is valid, but a timer expired */
    /* before receiving the expected number of bytes. */
    /* So LookupBridgeCmd returns a non-NULL function pointer. */
    TEST_ASSERT_NOT_NULL(LookupBridgeCmd(cmd));
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
        num_status_bytes_sent = UsbWriteStatusMissingArgs(BridgeCfgLis_key);
    }
    else {; /* number of bytes is correct, go check the bytes are valid */ }
    TEST_ASSERT_EQUAL(2,num_status_bytes_sent);
}
void UsbWriteStatusSpiBusError_sends_error_byte_and_slave_cmd(void)
{
    /* This just tests that two bytes are sent. */
    /* I cannot check the value of those two bytes. */
    /* =====[ Operate ]===== */
    uint8_t num_status_bytes_sent = 0;
    num_status_bytes_sent = UsbWriteStatusSpiBusError(SensorLed1Red_key);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_MESSAGE( 2, num_status_bytes_sent,
        "Expect `UsbWriteStatusSpiBusError` to send two bytes.");
}
void BytesComing_returns_16bit_word_from_struct_spi_NBytesToExpect(void)
{
    /* =====[ Setup ]===== */
    spi_BytesComing_s response_size;
    response_size.msb = 0x03;
    response_size.lsb = 0x10;
    /* =====[ Operate and Test ]===== */
    uint16_t expect = 0x0310;
    TEST_ASSERT_EQUAL_UINT16(expect, BytesComing(response_size));
}
void BytesComing_example_usage_to_catch_a_slave_error(void)
{
    TEST_FAIL_MESSAGE("Implement test.");
    /* /1* =====[ Setup ]===== *1/ */
    /* spi_BytesComing_s response_size; */
    /* response_size.msb = 0x00; */
    /* response_size.lsb = 0x01; */
    /* /1* =====[ Operate and Test ]===== *1/ */
    /* uint16_t expect = 0x0310; */
    /* if ( BytesComing(response_size) != 1 ) */
    /* { */
    /*     UsbWriteStatusSpiBusError(spi_Led1_Red_key); */
    /*     return; */
    /* } */

}

void LookupBridgeCmd_sad_example_using_UsbWriteStatus_API(void){
    /* =====[ Setup ]===== */
    bridge_cmd_key cmd = CmdBlackHat_key; // receive an invalid command
    /* =====[ Operate Example of Invalid Command (no test here) ]===== */
    BridgeCmd* CmdFn = LookupBridgeCmd(cmd);
    if (CmdFn == NULL) // sad
    {
        /* Send two bytes: error-code and cmd */
        UsbWriteStatusInvalid(cmd);
    }
    else CmdFn();
}
void LookupBridgeCmd_happy_example_using_UsbWriteStatus_API(void){
    /* =====[ Setup ]===== */
    bridge_cmd_key cmd = BridgeLedRed_key;
    /* =====[ Operate Example of Valid Command (no test here) ]===== */
    BridgeCmd* CmdFn = LookupBridgeCmd(cmd);
    if (CmdFn == NULL) UsbWriteStatusInvalid(cmd);
    else CmdFn();
    /* It is the CmdFn() responsibility to send UsbWriteStatusOk() at the end of a */
    /*     successful command (or some other code). */
    /* If a CmdFn() sends additional data, it must send UsbWriteStatusOk() prior to */
    /* sending the requested data. */
}
void CmdCfgLis_returns_StatusOk_and_echoes_back_the_4_cfg_bytes(void){
    /* =====[ Setup ]===== */
    bridge_cmd_key cmd = BridgeCfgLis_key;
    /* =====[ Operate ]===== */
    BridgeCmd* CmdFn = LookupBridgeCmd(cmd);
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
void CmdCfgLis_returns_StatusBadArgs_if_cfg_bytes_are_invalid(void)
{
    TEST_FAIL_MESSAGE("Implement test.");
}
void CmdCfgLis_1pushes_cfg_to_SpiSlave_2pulls_updated_cfg_3reports_StatusOk_updated_cfg(void)
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

/* =====[ Jump Table Sandbox ]===== */
/* Functions of type `spi_Cmd` take nothing and return nothing. */
void LookupSensorCmd_returns_Nth_fn_for_Nth_key(void){

    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL(SensorLed1Red, LookupSensorCmd(SensorLed1Red_key));
    TEST_ASSERT_EQUAL(SensorLed1Green, LookupSensorCmd(SensorLed1Green_key));
}
static sensor_cmd_key const spi_BlackHat_key = 255; // out-of-bounds: return NULL ptr
void LookupSensorCmd_returns_NULL_if_key_is_not_in_jump_table(void){
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_NULL(LookupSensorCmd(spi_BlackHat_key));
}
void LookupSensorCmd_example_calling_the_returned_command(void)
{
    TEST_IGNORE_MESSAGE("I do not understand this test.");
    /* See mocking in SpiSlaveSendBytes_waits_for_master_read_between_each_byte */
    /* Three callers in SpiSlaveSendBytes are mocked up in the test setup. */
    /* 1. Stub_SpiTransferIsDone: check if SPI interrupt flag is set. */
    /*     Fake the SPIF values returned. The test hangs if SPIF is never set! */
    bool SPIFs[] = {true, true, true};
    SpiTransferIsDone_StubbedReturnValue = SPIFs;

    /* 2. WriteSpiDataRegister: *Spi_spdr = byte_to_write; */
    /*     Spy by setting expectations on the values sent. */
    uint8_t Spi_spdr_write_log[sizeof(SPIFs)];
    WriteSpiDataRegister_WriteLog = Spi_spdr_write_log;

    /* 3. SpiSlaveSignalDataIsReady: disable/enable SPI to spike MISO low. */
    /*     Not important for testing. */
    /*     But it is mocked out, so I have to set an expectation that it is called. */

    /* =====[ Mock-up test scenario ]===== */
    /* Context: */
    /*     ---SpiSlave sends StatusOk byte after turning the LEDs red.--- */
    /* SpiSlaveWrite_StatusOK calls SpiSlaveSendBytes to send three bytes. */
    uint8_t const StatusOk[] = {
        0x00, 0x01, // nybtes [msb lsb]
        0x00        // data (0x00 means StatusOk)
    };
    uint8_t const * bytes = StatusOk; uint8_t nbytes = 3;
    /* Make sure the test fakes one SPIF value for each byte sent. */
    TEST_ASSERT_EQUAL(sizeof(SPIFs), nbytes);

    /* =====[ Record the expectations ]===== */
    while(nbytes-- != 0)
    {
        /* Set expectation for value written to the SPI data register. */
        Expect_WriteSpiDataRegister(*(bytes++));
        /* Set expectation that SpiSlave signals *Data-Ready* to the SpiMaster. */
        Expect_SpiSlaveSignalDataIsReady();
        /* Set expectation SpiSlave checks the SPIF. */
        Expect_SpiTransferIsDone();
    }

    /* =====[ Setup ]===== */
    /* Fake that DebugLed pins are low (LEDs are green). */
    *DebugLeds_port = 0x00; // debug_led1, debug_led2
    /* =====[ Operate ]===== */
    /* Note the parentheses to make it a function call */
    LookupSensorCmd(SensorLed1Red_key)(); // call the function returned by lookup
    /* ------------------------------- */
    //=====[ Test ]=====
    // --- 2019-09-30: testing the DebugLeds is impossible. Comment this out.
    /* TEST_ASSERT_BIT_HIGH(debug_led1, *DebugLeds_port); // HIGH is red */
    /* TEST_ASSERT_BIT_LOW(debug_led2, *DebugLeds_port); // LOW is green */
    // ---
    /* =====[ Spy on values sent from SpiSlave to SpiMaster ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void SpiSlaveWrite_StatusOk_sends_0x00_0x02_0x00_valid_cmd(void)
{
    TEST_IGNORE_MESSAGE("Move test to lib `Sensor`.");
    /* Use SetUp_SpiSlaveSendBytes */
    /* Three functions in SpiSlaveSendBytes are mocked out in SetUp. */
    /* 1. Stub_SpiTransferIsDone: check if SPI interrupt flag is set. */
    /*     Fake the SPIF values returned. The test hangs if SPIF is never set! */
    bool SPIFs[] = {true, true, true};
    SpiTransferIsDone_StubbedReturnValue = SPIFs;

    /* 2. WriteSpiDataRegister: *Spi_spdr = byte_to_write; */
    /*     Spy by setting expectations on the values sent. */
    uint8_t Spi_spdr_write_log[sizeof(SPIFs)];
    WriteSpiDataRegister_WriteLog = Spi_spdr_write_log;

    /* 3. SpiSlaveSignalDataIsReady: disable/enable SPI to spike MISO low. */
    /*     Not important for testing. */
    /*     But it is mocked out, so I have to set an expectation that it is called. */

    /* =====[ Mock-up test scenario ]===== */
    /* Context: */
    /*     ---SpiSlave sends StatusOk byte after turning the LEDs red.--- */
    /* SpiSlaveWrite_StatusOK calls SpiSlaveSendBytes to send three bytes. */
    sensor_cmd_key valid_cmd = 0x00;
    uint8_t const StatusOk[] = {
        0x00, 0x02, // nybtes [msb lsb]
        0x00, valid_cmd  // data (0x00 means StatusOk)
    };
    uint8_t const * bytes = StatusOk; uint8_t nbytes = 4;
    /* Make sure the test fakes one SPIF value for each byte sent. */
    TEST_ASSERT_EQUAL(sizeof(SPIFs), nbytes);

    /* =====[ Record the expectations ]===== */
    while(nbytes-- != 0)
    {
        /* Set expectation for value written to the SPI data register. */
        Expect_WriteSpiDataRegister(*(bytes++));
        /* Set expectation that SpiSlave signals *Data-Ready* to the SpiMaster. */
        Expect_SpiSlaveSignalDataIsReady();
        /* Set expectation SpiSlave checks the SPIF. */
        Expect_SpiTransferIsDone();
    }

    /* =====[ Operate ]===== */
    SpiSlaveWrite_StatusOk(valid_cmd);

    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

void SpiSlaveWrite_StatusInvalid_sends_0x00_0x02_0xFF_invalid_cmd_name(void)
{
    /* Use SetUp_SpiSlaveSendBytes */
    /* Three functions in SpiSlaveSendBytes are mocked out in SetUp. */
    /* 1. Stub_SpiTransferIsDone: check if SPI interrupt flag is set. */
    /*     Fake the SPIF values returned. The test hangs if SPIF is never set! */
    bool SPIFs[] = {true, true, true, true};
    SpiTransferIsDone_StubbedReturnValue = SPIFs;

    /* 2. WriteSpiDataRegister: *Spi_spdr = byte_to_write; */
    /*     Spy by setting expectations on the values sent. */
    uint8_t Spi_spdr_write_log[sizeof(SPIFs)];
    WriteSpiDataRegister_WriteLog = Spi_spdr_write_log;

    /* 3. SpiSlaveSignalDataIsReady: disable/enable SPI to spike MISO low. */
    /*     Not important for testing. */
    /*     But it is mocked out, so I have to set an expectation that it is called. */

    /* =====[ Mock-up test scenario ]===== */
    /* Context: */
    /*     ---SpiSlave sends StatusOk byte after turning the LEDs red.--- */
    /* SpiSlaveWrite_StatusOK calls SpiSlaveSendBytes to send three bytes. */
    uint8_t const StatusInvalid[] = {
        0x00, 0x02,             // nybtes [msb lsb]
        0xFF, spi_BlackHat_key  // data (0xFF means StatusInvalid)
    };
    uint8_t const * bytes = StatusInvalid; uint8_t nbytes = sizeof(StatusInvalid);
    TEST_ASSERT_EQUAL_MESSAGE(
        sizeof(SPIFs),
        nbytes,
        "Make sure the test fakes one SPIF value as true for each byte sent."
        );

    /* =====[ Record the expectations ]===== */
    while(nbytes-- != 0)
    {
        /* Set expectation for value written to the SPI data register. */
        Expect_WriteSpiDataRegister(*(bytes++));
        /* Set expectation that SpiSlave signals *Data-Ready* to the SpiMaster. */
        Expect_SpiSlaveSignalDataIsReady();
        /* Set expectation SpiSlave checks the SPIF. */
        Expect_SpiTransferIsDone();
    }

    /* =====[ Operate ]===== */
    SpiSlaveWrite_StatusInvalid(spi_BlackHat_key);

    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

void SpiMasterWriteN_NoInlineHelpers_sends_N_bytes_to_SpiSlave(void)
{
    /* =====[ Setup ]===== */
    /* Stub_SpiTransferIsDone: check if SPI interrupt flag is set. */
    /* Fake the SPIF values returned. The test hangs if SPIF is never set! */
    bool SPIFs[] = {false, false, true};
    uint8_t const nfalse = 2;
    SpiTransferIsDone_StubbedReturnValue = SPIFs;

    /* =====[ Set Expectations ]===== */
    Expect_SpiTransferIsDone(); // SPIF is clear
    Expect_SpiTransferIsDone(); // SPIF is still clear
    Expect_SpiTransferIsDone(); // SPIF is set!

    /* =====[ Fake the registers. ]===== */
    /* Fake the `Spi_port`. */
    /* Slave Select starts low to test */
    /* that `Spi_Ss` is high when `SpiMasterWriteN_NoInlineHelpers` is done. */
    *Spi_port = 0x00;

    /* 3. Fake the `Spi_spdr`. */
    /* Start with `Spi_spdr` != `cmd` to test that */
    /* `SpiMasterWriteN_NoInlineHelpers` loads `Spi_spdr` with `cmd`. */
    *Spi_spdr = 0xFF;

    /* =====[ Operate ]===== */
    /* Fake the UsbHost sent CmdLedRed. */
    /* Write the command to turn debug_led1 red on the SPI slave. */
    uint8_t const cmd_led1_red[] = {0x00};
    uint8_t nbytes = sizeof(cmd_led1_red);
    TEST_ASSERT_EQUAL_MESSAGE( sizeof(SPIFs)-nfalse, nbytes,
        "Make sure there is one true SPIF value for each byte sent."
        );
    SpiMasterWriteN_NoInlineHelpers(cmd_led1_red, nbytes);

    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE( Spi_Ss, *Spi_port,
        "Expect *Slave Select* is high when done."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE( *Spi_spdr, cmd_led1_red[0],
        "Expect `cmd_1ed1_red` was written to the *SPI data register*."
        );

    TEST_ASSERT_TRUE_MESSAGE(
    RanAsHoped(mock),           // If this is false,
    WhyDidItFail(mock)          // print this message.
    );
}

void SpiMasterWriteByte_sends_one_byte_to_SpiSlave(void)
{
    /* =====[ Setup ]===== */
    /* Fake the SPI interrupt flag in the SPI status register. */
    /* Start test with SPIF set so that `SpiMasterWriteN` sees */
    /* the transmission is done when it checks this flag. */
    SetBit(Spi_spsr, Spi_InterruptFlag);

    /* =====[ Fake the registers. ]===== */
    /* Fake the `Spi_port`. */
    /* Start test with Slave Select low to test that */
    /* `Spi_Ss` is high when `SpiMasterWriteByte` is done. */
    *Spi_port = 0x00;

    /* 3. Fake the `Spi_spdr`. */
    /* Start test with `Spi_spdr` != `cmd` to test that */
    /* `SpiMasterWriteByte` loads `Spi_spdr` with `cmd`. */
    uint8_t const black_hat_cmd = 0xFF;
    *Spi_spdr = black_hat_cmd;

    /* =====[ Operate ]===== */
    /* Simulate that the UsbHost sent CmdLedRed. */
    /* Write the command to turn led1 red on the SPI slave. */
    uint8_t const cmd_led1_red = 0x00;
    SpiMasterWriteByte(cmd_led1_red);

    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE( Spi_Ss, *Spi_port,
        "Expect *Slave Select* is high when done."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE( *Spi_spdr, cmd_led1_red,
        "Expect `cmd_led1_red` was written to the *SPI data register*."
        );
}

