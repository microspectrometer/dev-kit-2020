#include "test_Bridge.h"
#include "Bridge.h"         // lib under test
/* ---Unit Test Framework--- */
#include "unity.h"          // unity macros: TEST_BLAH
#include <Mock.h>           // RanAsHoped, WhyDidItFail (mocks record calls/args)
#include <ReadWriteBits.h>  // set/clear bits in fake hardware registers

/* ---Define a setup/teardown for writing call history--- */
static void SetUp_Mock(void) { mock = Mock_new(); }
static void TearDown_Mock(void) { Mock_destroy(mock); mock = NULL; }

/* Hardware fakes are included by test_runner.c. */
/* ---Project Lib Dependencies--- */
#include "BiColorLed.h"
#include "Spi.h"
/* ---Mock Out Libs That Read Hardware Registers--- */
#include "mock_Spi.h"
/* ---Include Libs with functions faked in tests--- */
#include "Usb.h"
/* ---Hardware registers for faking UsbRead values--- */
#include "Ft1248.h"

/** `status_led` is defined in `fake/BiColorLed-Hardware.h`. */
/* I cannot simply include that header because the test suite */
/* has multiple test translation units. */
/* If the Usb and Bicolor tests both include the */
/* BiColorLed-Hardware header, then `status_led` is defined */
/* multiple times: once in `test_Usb.o` and once in `test_BiColor.o`. */
extern uint8_t const status_led;

/* =====[ BridgeJumpTable - started 2019-10-02]===== */
/* Functions of type `BridgeCmd` take nothing and return nothing. */
/* Command arguments are handled by the command's function definition. */
    /* Example: */
    /* command `GetBridgeLED` is followed by a one-byte argument, */
    /* so `GetBridgeLED()` reads one more byte. */
void LookupBridgeCmd_takes_key_and_returns_fn_ptr(void){
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL(GetBridgeLED, LookupBridgeCmd(GetBridgeLED_key));
}
static bridge_cmd_key const CmdBlackHat_key = 255; // out-of-bounds: return NULL ptr
void LookupBridgeCmd_returns_NULL_if_key_is_not_found(void){
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL(NULL, LookupBridgeCmd(CmdBlackHat_key));
}
/* =====[ Mock UsbWrite() for unit-testing SendStatus() ]===== */
// Define what is recorded when the function under test calls fake.
static RecordedCall * Record_UsbWrite(uint8_t const *arg1, uint16_t arg2)
{
    char const *call_name = "UsbWrite";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t const **)record_of_arg1->pArg) = arg1;
    RecordedArg *record_of_arg2 = RecordedArg_new(SetupRecord_uint16_t);
    *((uint16_t *)record_of_arg2->pArg) = arg2;
    RecordArg(record_of_this_call, record_of_arg1);
    RecordArg(record_of_this_call, record_of_arg2);
    return record_of_this_call;
}
/* =====[ Define the fake UsbWrite() ]===== */
static uint16_t UsbWrite_Mocked(uint8_t const *write_buffer, uint16_t nbytes) {
    RecordActualCall(mock, Record_UsbWrite(write_buffer, nbytes));
    return nbytes; // Test fakes return value by choosing value passed as nybtes.
}
// Define how to swap function definitions
static uint16_t (*UsbWrite_Saved)(uint8_t const *, uint16_t);
// how to restore real UsbWrite()
static void Restore_UsbWrite(void) {UsbWrite = UsbWrite_Saved;}
// how to swap real UsbWrite() with mocked version
static void Mock_UsbWrite(void) {UsbWrite_Saved=UsbWrite;UsbWrite=UsbWrite_Mocked;}

/* =====[ Fake/Mock/Stub terminology ]===== */
// Fake:
    // anything that is not from the real function definition
    // examples:
    // fake return value
    // stubbed version of function
    // mocked version of function
// MyFunc_Stubbed:
    // placeholder for MyFunc, has no side effects
    // Stub_MyFunc(): MyFunc does not show up in the call history
// MyFunc_Mocked:
    // like a stub, but also writes to the unit test's call history
    // Mock_MyFunc(): MyFunc shows up in the call history
// To swap real and fake definitions, **func must be a func-ptr**!!
    // if func is not a func-ptr, compiler throws `lvalue` error when
    // you try to do {func-ptr}={func-name}

/* =====[ Mock UsbReadN() for unit-testing GetBridgeLED() ]===== */
// Define call recorded when func-under-test calls mocked function.
static RecordedCall * Record_UsbReadN(uint8_t *arg1, uint16_t arg2)
{
    char const *call_name = "UsbReadN";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t **)record_of_arg1->pArg) = arg1;
    // Crazy pointer syntax for arg1:
        // `pArg` is a pointer to the argument value to store, arg1.
        // Dereference `pArg` with `*` to assign arg1.
        // arg1 is a byte array (a pointer to a byte): uint8_t *.
        // Therefore cast `pArg` as a pointer to a byte array: uint8_t **.
    RecordedArg *record_of_arg2 = RecordedArg_new(SetupRecord_uint16_t);
    *((uint16_t *)record_of_arg2->pArg) = arg2;
    // Crazy pointer syntax for arg2:
        // arg2 is a 16-bit word.
        // So cast `pArg` as a pointer to a 16-bit word.
        // Dereference `pArg` with `*` to assign arg2.
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    RecordArg(record_of_this_call, record_of_arg2);
    return record_of_this_call;
}
// Define behavior of stubbed UsbReadN().
static uint16_t UsbReadN_Stubbed(uint8_t *read_buffer, uint16_t nbytes) {
    /* (void)read_buffer; (void)nbytes; // trick to avoid unused args warning */
    *read_buffer = *FtMiosio_pin; // Fake a read by copying register value to read_buffer.
    return nbytes; // Fake: nbytes is whatever the test says it is.
}
// Define behavior of mocked UsbReadN(). Mock is like stub, but also records itself.
static uint16_t UsbReadN_Mocked(uint8_t *read_buffer, uint16_t nbytes) {
    RecordActualCall(mock, Record_UsbReadN(read_buffer, nbytes));
    *read_buffer = *FtMiosio_pin; // Fake a read by copying register value to read_buffer.
    return nbytes; // Fake: nbytes is whatever the test says it is.
}
uint8_t * FakeByteArray_ForUsbReadN;
static uint16_t UsbReadN_ByteArray_Mocked(uint8_t *read_buffer, uint16_t nbytes) {
    // Fake reading an array of bytes into the read_buffer.
    // The test is responsible to populate FakeByteArray_ForUsbReadN.
    RecordActualCall(mock, Record_UsbReadN(read_buffer, nbytes));
    uint16_t byte_counter = 0;
    while ( byte_counter++ < nbytes )
    {
        *(read_buffer++)  = *(FakeByteArray_ForUsbReadN++);
    }
    return nbytes;
}
// Define how to swap function definitions
static uint16_t (*UsbReadN_Saved)(uint8_t *, uint16_t);
// how to restore real UsbReadN()
static void Restore_UsbReadN(void) {UsbReadN=UsbReadN_Saved;}
// how to swap real UsbReadN() with stubbed version
static void Stub_UsbReadN(void) {UsbReadN_Saved=UsbReadN;UsbReadN=UsbReadN_Stubbed;}
// how to swap real UsbReadN() with mocked version
static void Mock_UsbReadN(void) {UsbReadN_Saved=UsbReadN;UsbReadN=UsbReadN_Mocked;}
static void Mock_UsbReadN_ByteArray(void)
{
    UsbReadN_Saved=UsbReadN;
    UsbReadN=UsbReadN_ByteArray_Mocked;
}

/* =====[ Mock SendStatus() for unit-testing GetBridgeLED() ]===== */
// Define call recorded when func-under-test calls mocked function.
static RecordedCall * Record_SendStatus(uint8_t arg1)
{
    char const *call_name = "SendStatus";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
// Define behavior of stubbed SendStatus().
/* static void SendStatus_Stubbed(status_byte status) { (void)status;} */
// Define behavior of mocked SendStatus().
static void SendStatus_Mocked(status_byte status) {
    RecordActualCall(mock, Record_SendStatus(status));
}
// Define how unit test sets an expectation for SendStatus.
/* static void Expect_SendStatus(status_byte status) { */
/*     RecordExpectedCall(mock, Record_SendStatus(status)); */
/* } */
// Define how to swap function definitions
static void (*SendStatus_Saved)(status_byte);
// how to restore real definition
static void Restore_SendStatus(void) { SendStatus=SendStatus_Saved; }
// how to swap real definition with stubbed version
/* static void Stub_SendStatus(void) { SendStatus_Saved=SendStatus; SendStatus=SendStatus_Stubbed;} */
// how to swap real definition with mocked version
static void Mock_SendStatus(void) { SendStatus_Saved=SendStatus; SendStatus=SendStatus_Mocked;}


void SetUp_GetBridgeLED(void)
{
    SetUp_Mock();
    Mock_UsbReadN();
    Mock_SendStatus();
}
void TearDown_GetBridgeLED(void)
{
    TearDown_Mock();
    Restore_UsbReadN();
    Restore_SendStatus();
}
void GetBridgeLED_reads_one_byte_of_payload(void)
{
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test: assert UsbReadN called to read 1 byte ]===== */
    /* PrintAllCalls(mock); // view entire call history */
    // call sig of mocked call: uint16_t UsbReadN(uint8_t *read_buffer, uint16_t nbytes)
    uint8_t call_n = 1; uint16_t arg_n = 2; uint16_t nbytes = 1;
    // test call 1 is UsbReadN
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadN"));
    // test UsbReadN is called with nbytes=1
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &nbytes));
}
void GetBridgeLED_replies_with_two_bytes_if_led_number_is_recognized(void)
{
    /* Inject query for status_led so that led number is recognized. */
    uint8_t const led_number = led_0; *FtMiosio_pin = led_number;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* PrintAllCalls(mock); */
    /* =====[ Test ]===== */
    // Assert there are only three calls total.
    TEST_ASSERT_EQUAL_UINT8(3, NumberOfActualCalls(mock));
    // And assert that only the last two calls send bytes.
    uint8_t call_n;
    call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadN"));
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
    call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
}
void GetBridgeLED_replies_with_one_byte_if_led_number_is_not_recognized(void)
{
    /* Inject query for status_led so that led number is not recognized. */
    uint8_t const led_number = led_0+100; *FtMiosio_pin = led_number;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* PrintAllCalls(mock); */
    /* =====[ Test ]===== */
    // Assert there are only two calls total.
    TEST_ASSERT_EQUAL_UINT8(2, NumberOfActualCalls(mock));
    // And assert that only the last call sends a byte.
    uint8_t call_n;
    call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadN"));
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
}
void GetBridgeLED_replies_msg_status_ok_if_host_queries_status_led(void)
{
    /* Inject query for status_led */
    uint8_t const led_number = led_0; *FtMiosio_pin = led_number;
    /* Test case: msg_status is ok */
    status_byte msg_status = ok;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t *)&msg_status));
}
void GetBridgeLED_replies_msg_status_error_if_host_queries_nonexistent_led(void)
{
    /* Inject query for nonexistent led */
    uint8_t const led_number = led_0+100; *FtMiosio_pin = led_number;
    /* Test case: msg_status is error because led number is not recognized. */
    status_byte msg_status = error;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t *)&msg_status));
}
void GetBridgeLED_replies_with_msg_status_byte_and_led_status_byte(void)
{
    /* =====[ Test Case: BridgeLED is green ]===== */
    status_byte msg_status = ok; status_byte led_status = led_green;
    /* Inject value in fake hardware registers to set led state for test: */
    /*  - (BiColorLed_ddr,status_led) set: on, clear: off */
    /*  - (BiColorLed_port,status_led) set: red, clear: green */
    SetBit(BiColorLed_ddr,status_led); // led is on
    ClearBit(BiColorLed_port,status_led); // led is green
    /* Inject value in fake hardware for UsbRead to place in read_buffer[0]: */
     /* - inject 0x00 to indicate payload from host queries status_led */
     /* - any other value is an error because status_led is the only Bridge LED */
    uint8_t const led_number = led_0; *FtMiosio_pin = led_number;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock,call_n,"SendStatus"));
    TEST_ASSERT_TRUE(AssertArg(mock,call_n,arg_n,(uint8_t*)&msg_status));
    call_n = 3; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock,call_n,"SendStatus"));
    TEST_ASSERT_TRUE(AssertArg(mock,call_n,arg_n,(uint8_t*)&led_status));
}
void GetBridgeLED_replies_led_off_if_status_led_is_off(void)
{
    /* =====[ Inject led_state ]===== */
    BiColorLedOff(status_led);
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 3; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
    status_byte led_status = led_off;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t*)&led_status));
}
void GetBridgeLED_replies_led_green_if_status_led_is_green(void)
{
    /* =====[ Inject led_state ]===== */
    BiColorLedOn(status_led); BiColorLedGreen(status_led);
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 3; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
    status_byte led_status = led_green;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t*)&led_status));
}
void GetBridgeLED_replies_led_red_if_status_led_is_red(void)
{
    /* =====[ Inject led_state ]===== */
    BiColorLedOn(status_led); BiColorLedRed(status_led);
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 3; arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
    status_byte led_status = led_red;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t*)&led_status));
}

void SetUp_SetBridgeLED(void)
{
    SetUp_Mock();
    Mock_UsbReadN_ByteArray();
    Mock_SendStatus();
}
void TearDown_SetBridgeLED(void)
{
    TearDown_Mock();
    Restore_UsbReadN();
    Restore_SendStatus();
}
void SetBridgeLED_reads_two_bytes_of_payload(void)
{
    /* Inject two bytes of payload for fake UsbReadN. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadN = payload;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test: assert UsbReadN called to read 1 byte ]===== */
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadN"));
    uint8_t arg_n = 2; uint16_t nbytes = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes),
        "Expect UsbReadN reads two bytes."
        );
}
void SetBridgeLED_replies_with_one_byte(void)
{
    /* Inject status_led number as payload from host */
    /* uint8_t const led_number = status_led; *FtMiosio_pin = led_number; */
    /* Inject two bytes of payload for fake UsbReadN. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadN = payload;

    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test: assert only two calls are made ]===== */
    TEST_ASSERT_EQUAL_UINT8(2,NumberOfActualCalls(mock));
    /* =====[ Test: assert only last call sends a byte ]===== */
    uint8_t call_n;
    call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadN"));
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
}
void SetBridgeLED_replies_msg_status_ok_if_led_number_is_status_led(void)
{
    /* Inject status_led number as payload from host */
    /* uint8_t const led_number = status_led; *FtMiosio_pin = led_number; */
    /* Inject two bytes of payload for fake UsbReadN. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadN = payload;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test: assert Bridge sends msg_status ok ]===== */
    status_byte msg_status = ok;
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
    arg_n = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t *)&msg_status));
}
void SetBridgeLED_replies_msg_status_error_if_led_number_is_not_recognized(void)
{
    /* Test case: host sends a nonexistent led number. */
    uint8_t const led_number = led_0+100;
    /* Inject two bytes of payload for fake UsbReadN. */
    uint8_t payload[] = {led_number, led_off};
    FakeByteArray_ForUsbReadN = payload;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test: assert Bridge sends msg_status error ]===== */
    status_byte msg_status = error;
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SendStatus"));
    arg_n = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, (uint8_t *)&msg_status));
}
void SetBridgeLED_turns_off_led_if_payload_is_led_off(void)
{
    /* =====[ Test case: status_led is on before command is sent ]===== */
    BiColorLedOn(status_led);
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsOn(status_led),
        "status_led must be on before test begins."
        );
    /* Inject two bytes of payload for fake UsbReadN. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadN = payload;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsOn(status_led),
        "Expect SetBridgeLED turns off status_led."
        )
}
void SetBridgeLED_turns_led_on_and_green_if_payload_is_led_green(void)
{
    /* =====[ Test case: status_led is off before command is sent ]===== */
    BiColorLedRed(status_led);
    BiColorLedOff(status_led);
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsOn(status_led),
        "status_led must be off and primed for red before test begins."
        );
    /* Inject two bytes of payload for fake UsbReadN. */
    uint8_t payload[] = {led_0, led_green};
    FakeByteArray_ForUsbReadN = payload;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsOn(status_led),
        "Expect SetBridgeLED turns on status_led."
        )
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsRed(status_led),
        "Expect SetBridgeLED turns status_led green."
        )
}
void SetBridgeLED_turns_led_on_and_red_if_payload_is_led_red(void)
{
    /* =====[ Test case: status_led is off before command is sent ]===== */
    BiColorLedGreen(status_led);
    BiColorLedOff(status_led);
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsOn(status_led),
        "status_led must be off and primed for green before test begins."
        );
    /* Inject two bytes of payload for fake UsbReadN. */
    uint8_t payload[] = {led_0, led_red};
    FakeByteArray_ForUsbReadN = payload;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsOn(status_led),
        "Expect SetBridgeLED turns on status_led."
        )
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsRed(status_led),
        "Expect SetBridgeLED turns status_led red."
        )
}

void SetUp_SendStatus_writes_one_byte_over_USB(void)
{
    SetUp_Mock();
    Mock_UsbWrite();
}
void TearDown_SendStatus_writes_one_byte_over_USB(void)
{
    TearDown_Mock();
    Restore_UsbWrite();
}
void SendStatus_writes_one_byte_over_USB(void)
{
    /* =====[ Operate ]===== */
    SendStatus(ok);
    /* =====[ Test: assert UsbWrite ]===== */
    uint8_t call_n = 1;
    /* PrintAllCalls(mock); */
    TEST_ASSERT_TRUE(AssertCall(mock,call_n,"UsbWrite"));
    /* =====[ Test: assert one byte is written ]===== */
    uint8_t arg_n = 2; // arg2 is nbytes to write
    uint16_t nbytes = 1; // assert nbytes is 1
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &nbytes));
}
void SetUp_Stub_UsbReadN_with_value_in_read_buffer(void)
{
    SetUp_Mock();
    Stub_UsbReadN();
}
void TearDown_Stub_UsbReadN_with_value_in_read_buffer(void)
{
    TearDown_Mock();
    Restore_UsbReadN();
}
void Stub_UsbReadN_with_value_in_read_buffer(void)
{ // This test demonstrates how to fake the value read over USB.
    /* =====[ Inject a fake value into the read_buffer ]===== */
    // Write the fake value to the low-level hardware register
    // accessed during the UsbRead: register `FtMiosio_pin`.
    uint8_t const led_number = led_0+100; *FtMiosio_pin = led_number;
    /* =====[ Operate: Read one byte, store result in read_buffer. ]===== */
    uint8_t const num_bytes_payload = 1;
    uint8_t read_buffer[num_bytes_payload];
    UsbReadN(read_buffer, num_bytes_payload); // stub reads fake pin values
    /* =====[ Test: this unit test injects its fake value in read_buffer[0] ]===== */
    TEST_ASSERT_EQUAL_UINT8(led_number, read_buffer[0]);
}

/* =====[ Jump Table Sandbox ]===== */
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
    /* TEST_IGNORE_MESSAGE("I do not understand this test."); */
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
    /* Fake that status_led pin is red. */
    *BiColorLed_port = 0xFF; // HIGH is red
    /* =====[ Operate ]===== */
    /* Note the parentheses to make it a function call */
    LookupSensorCmd(SensorLed1Red_key)(); // call the function returned by lookup
    /* ------------------------------- */
    //=====[ Test ]=====
    // --- 2019-09-30: What am I trying to test here? Does the LED change?
    TEST_ASSERT_BIT_LOW(status_led, *BiColorLed_port); // LOW is green
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

