#include "test_Bridge.h"
#include "Bridge.h"         // lib under test
/* ---Unit Test Framework--- */
#include "unity.h"          // unity macros: TEST_BLAH
#include <Mock.h>           // record call history: PrintAllCalls, AssertCalls, AssertArgs
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

#define npixels 784
uint8_t frame[npixels*2];

/** `status_led` is defined in `fake/BiColorLed-Hardware.h`. */
/* I cannot simply include that header because the test suite */
/* has multiple test translation units. */
/* If the Usb and Bicolor tests both include the */
/* BiColorLed-Hardware header, then `status_led` is defined */
/* multiple times: once in `test_Usb.o` and once in `test_BiColor.o`. */
extern uint8_t const status_led;

/* =====[ Mock UsbWrite() for unit-testing SerialWriteByte() ]===== */
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


void SetUp_SerialWriteByte_writes_one_byte_over_USB(void)
{
    SetUp_Mock();
    Mock_UsbWrite();
}
void TearDown_SerialWriteByte_writes_one_byte_over_USB(void)
{
    TearDown_Mock();
    Restore_UsbWrite();
}
void SerialWriteByte_writes_one_byte_over_USB(void)
{
    /* =====[ Operate ]===== */
    SerialWriteByte(ok);
    /* =====[ Test: assert UsbWrite ]===== */
    uint8_t call_n = 1;
    /* PrintAllCalls(mock); */
    TEST_ASSERT_TRUE(AssertCall(mock,call_n,"UsbWrite"));
    /* =====[ Test: assert one byte is written ]===== */
    uint8_t arg_n = 2; // arg2 is nbytes to write
    uint16_t nbytes = 1; // assert nbytes is 1
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &nbytes));
}
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
void LookupBridgeCmd_returns_NULL_if_key_is_not_found(void)
{
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL(NULL, LookupBridgeCmd(CmdBlackHat_key));
}

/* ---Create Mocks--- */
/* =====[ Mock WriteSensor() for unit-testing BridgeGetSensorLED() ]===== */
// Define call recorded when func-under-test calls mocked function.
static RecordedCall * Record_WriteSensor(uint8_t const *arg1, uint16_t arg2)
{
    char const *call_name = "WriteSensor";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t const **)record_of_arg1->pArg) = arg1;
    RecordedArg *record_of_arg2 = RecordedArg_new(SetupRecord_uint16_t);
    *((uint16_t *)record_of_arg2->pArg) = arg2;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    RecordArg(record_of_this_call, record_of_arg2);
    return record_of_this_call;
}
// Define behavior of mocked function: WriteSensor().
// Global for test to spy on array input arg to WriteSensor.
#define max_sizeof_write_buffer 10
uint8_t SpyOn_WriteSensor_arg1[max_sizeof_write_buffer];
static uint16_t WriteSensor_Mocked(uint8_t const *write_buffer, uint16_t nbytes)
{
    RecordActualCall(mock, Record_WriteSensor(write_buffer, nbytes));
    /* Spy on values passed to write_buffer by storing them in a global */
    uint8_t *spy_writer = SpyOn_WriteSensor_arg1;
    uint16_t num_bytes_sent = 0;
    while(num_bytes_sent < nbytes)
    {
        *(spy_writer++) = *(write_buffer++);
        num_bytes_sent++;
    }
    return num_bytes_sent;
}

// Define how to swap function definitions
static uint16_t (*WriteSensor_Saved)(uint8_t const *, uint16_t);
// how to restore real definition
static void Restore_WriteSensor(void) { WriteSensor = WriteSensor_Saved; }
// how to swap real definition with mocked version
static void Mock_WriteSensor(void)
{
    WriteSensor_Saved = WriteSensor;
    WriteSensor = WriteSensor_Mocked;
}
/* =====[ Mock ReadSensor() for unit-testing BridgeGetSensorLED() ]===== */
// Define call recorded when func-under-test calls mocked function.
static RecordedCall * Record_ReadSensor(uint8_t *arg1, uint16_t arg2)
{
    char const *call_name = "ReadSensor";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t **)record_of_arg1->pArg) = arg1;
    RecordedArg *record_of_arg2 = RecordedArg_new(SetupRecord_uint16_t);
    *((uint16_t *)record_of_arg2->pArg) = arg2;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    RecordArg(record_of_this_call, record_of_arg2);
    return record_of_this_call;
}
// Define behavior of mocked function: ReadSensor().
uint8_t *FakeByteArray_ForReadSensor;
static uint16_t ReadSensor_Mocked(uint8_t *read_buffer, uint16_t nbytes)
{
    RecordActualCall(mock, Record_ReadSensor(read_buffer, nbytes));
    // Fake reading an array of bytes into the read_buffer.
    // The test is responsible to populate FakeByteArray_ForReadSensor.
    uint16_t num_bytes_read = 0;
    while ( num_bytes_read++ < nbytes )
    {
        *(read_buffer++)  = *(FakeByteArray_ForReadSensor++);
    }
    return nbytes;
}
// Define how to swap function definitions
static uint16_t (*ReadSensor_Saved)(uint8_t *, uint16_t);
// how to restore real definition
static void Restore_ReadSensor(void) { ReadSensor = ReadSensor_Saved; }
// how to swap real definition with mocked version
static void Mock_ReadSensor(void)
{
    ReadSensor_Saved = ReadSensor;
    ReadSensor = ReadSensor_Mocked;
}
/* =====[ Mock SpiWriteByte ]===== */
static RecordedCall * Record_SpiWriteByte(uint8_t arg1)
{
    char const *call_name = "SpiWriteByte";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
static void SpiWriteByte_Mocked(uint8_t byte)
{
    RecordActualCall(mock, Record_SpiWriteByte(byte));
}
static void (*SpiWriteByte_Saved)(uint8_t);
static void Restore_SpiWriteByte(void)
{
    SpiWriteByte = SpiWriteByte_Saved;
}
static void Mock_SpiWriteByte(void)
{
    SpiWriteByte_Saved = SpiWriteByte;
    SpiWriteByte = SpiWriteByte_Mocked;
}

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

// Define call recorded when func-under-test calls mocked function.
static RecordedCall * Record_UsbReadBytes(uint8_t *arg1, uint16_t arg2)
{
    char const *call_name = "UsbReadBytes";
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

/* =====[ Replace UsbReadN with UsbReadbytes. ]===== */
// Define behavior of stubbed UsbReadBytes().
uint8_t * FakeByteArray_ForUsbReadBytes;
static uint16_t UsbReadBytes_Stubbed(uint8_t *read_buffer, uint16_t nbytes)
{
    // Fake reading an array of bytes into the read_buffer.
    // The test is responsible to populate FakeByteArray_ForUsbReadBytes.
    uint16_t byte_counter = 0;
    while ( byte_counter++ < nbytes )
    {
        *(read_buffer++)  = *(FakeByteArray_ForUsbReadBytes++);
    }
    return nbytes;
}
static uint16_t UsbReadBytes_Mocked(uint8_t *read_buffer, uint16_t nbytes)
{
    // Fake reading an array of bytes into the read_buffer.
    // The test is responsible to populate FakeByteArray_ForUsbReadN.
    RecordActualCall(mock, Record_UsbReadBytes(read_buffer, nbytes));
    return UsbReadBytes_Stubbed(read_buffer, nbytes);
}
// Define how to swap function definitions
static uint16_t (*UsbReadBytes_Saved)(uint8_t *, uint16_t);
// how to restore real UsbReadBytes()
static void Restore_UsbReadBytes(void) {UsbReadBytes=UsbReadBytes_Saved;}
// how to swap real UsbReadBytes() with mocked version
static void Mock_UsbReadBytes(void)
{
    UsbReadBytes_Saved = UsbReadBytes;
    UsbReadBytes = UsbReadBytes_Mocked;
}

/* =====[ Mock SerialWriteByte() for unit-testing GetBridgeLED() ]===== */
// Define call recorded when func-under-test calls mocked function.
static RecordedCall * Record_SerialWriteByte(uint8_t arg1)
{
    char const *call_name = "SerialWriteByte";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
// Define behavior of stubbed SerialWriteByte().
/* static void SerialWriteByte_Stubbed(status_byte status) { (void)status;} */
// Define behavior of mocked SerialWriteByte().
static void SerialWriteByte_Mocked(status_byte status)
{
    RecordActualCall(mock, Record_SerialWriteByte(status));
}
// Define how unit test sets an expectation for SerialWriteByte.
/* static void Expect_SerialWriteByte(status_byte status) { */
/*     RecordExpectedCall(mock, Record_SerialWriteByte(status)); */
/* } */
// Define how to swap function definitions
static void (*SerialWriteByte_Saved)(status_byte);
// how to restore real definition
static void Restore_SerialWriteByte(void) { SerialWriteByte=SerialWriteByte_Saved; }
// how to swap real definition with stubbed version
/* static void Stub_SerialWriteByte(void) { SerialWriteByte_Saved=SerialWriteByte; SerialWriteByte=SerialWriteByte_Stubbed;} */
// how to swap real definition with mocked version
static void Mock_SerialWriteByte(void)
{
    SerialWriteByte_Saved = SerialWriteByte;
    SerialWriteByte = SerialWriteByte_Mocked;
}


void SetUp_GetBridgeLED(void)
{
    SetUp_Mock();
    Mock_UsbReadBytes();
    Mock_ReadSensor();
    Mock_SerialWriteByte();
}
void TearDown_GetBridgeLED(void)
{
    TearDown_Mock();
    Restore_UsbReadBytes();
    Restore_ReadSensor();
    Restore_SerialWriteByte();
}
void GetBridgeLED_receives_led_number(void)
{
    /* =====[ Setup ]===== */
    /* Inject payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test: assert UsbReadBytes called to read 1 byte ]===== */
    /* PrintAllCalls(mock); // view entire call history */
    uint8_t call_n = 1; uint16_t arg_n = 2; uint16_t nbytes = 1;
    // test call 1 is UsbReadBytes
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadBytes"));
    // test UsbReadBytes is called with nbytes=1
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &nbytes));
}
void GetBridgeLED_reads_and_ignores_Sensor_reply_to_GetBridgeLED(void)
{
    /* =====[ Setup ]===== */
    /* Inject payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    // ---Check the Bridge reads the Sensor response to command GetBridgeLED---
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 2 is ReadSensor."
        );
    // ---Prove that the Bridge does not write this response to the USB host---
    // Assert two and only two subsequent calls to SerialWriteByte.
    // Assert calls to SerialWriteByte do not send the Sensor response.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 3 is SerialWriteByte."
        );
    uint8_t arg_n = 1; uint8_t arg_val = invalid_cmd;
    TEST_ASSERT_FALSE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_val),
        "Expect SerialWriteByte does not send the Sensor response (invalid_cmd)."
        );
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 4 is SerialWriteByte."
        );
    TEST_ASSERT_FALSE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_val),
        "Expect SerialWriteByte does not send the Sensor response (invalid_cmd)."
        );
    // Assert there are no more calls
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void GetBridgeLED_replies_msg_status_ok_if_led_number_is_recognized(void)
{
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t good_led_number = led_0;
    uint8_t payload[] = {good_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* Inject LED state */
    BiColorLedOn(status_led);
    BiColorLedGreen(status_led);
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; arg_value = ok;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value));
}
void GetBridgeLED_replies_msg_status_error_if_led_does_not_exist(void)
{
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t bad_led_number = led_0+99;
    uint8_t payload[] = {bad_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* Inject LED state */
    BiColorLedOn(status_led);
    BiColorLedGreen(status_led);
    /* uint8_t led_state = led_green; */
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; arg_value = error;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value));
}
void GetBridgeLED_sends_no_additional_bytes_if_msg_status_is_error(void)
{
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t bad_led_number = led_0+99;
    uint8_t payload[] = {bad_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* Inject LED state */
    BiColorLedOn(status_led);
    BiColorLedGreen(status_led);
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    // ---The Bridge does not send any more bytes after status byte "error"---
    // Check that the Bridge sends "error"
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; arg_value = error;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value));
    // Assert there are no more calls
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void GetBridgeLED_sends_led_status_byte_after_sending_msg_status_ok(void)
{
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t good_led_number = led_0;
    uint8_t payload[] = {good_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* Inject LED state */
    BiColorLedOn(status_led);
    BiColorLedGreen(status_led);
    uint8_t led_state = led_green;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    // ---The Bridge sends led_state after status byte "ok"---
    // Check that the Bridge sends "ok"
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    call_n = 3;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; arg_value = ok;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value));
    // Check that the Bridge sends led_state
    call_n = 4;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; arg_value = led_state;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value));
    // Assert there are no more calls
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void GetBridgeLED_replies_led_off_if_led_is_off(void)
{
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t good_led_number = led_0;
    uint8_t payload[] = {good_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* Inject LED state */
    BiColorLedOff(status_led); uint8_t led_state = led_off;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    call_n = 4;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; arg_value = led_state;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value));
}
void GetBridgeLED_replies_led_green_if_led_is_green(void)
{
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t good_led_number = led_0;
    uint8_t payload[] = {good_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* Inject LED state */
    BiColorLedOn(status_led);
    BiColorLedGreen(status_led); uint8_t led_state = led_green;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    call_n = 4;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; arg_value = led_state;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value));
}
void GetBridgeLED_replies_led_red_if_led_is_red(void)
{
    /* =====[ Setup ]===== */
    // Inject one byte of payload for fake UsbReadBytes.
    uint8_t good_led_number = led_0;
    uint8_t payload[] = {good_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* Inject LED state */
    BiColorLedOn(status_led);
    BiColorLedRed(status_led); uint8_t led_state = led_red;
    /* =====[ Operate ]===== */
    GetBridgeLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    call_n = 4;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    arg_n = 1; arg_value = led_state;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_value));
}

void SetUp_SetBridgeLED(void)
{
    SetUp_Mock();
    Mock_UsbReadBytes();
    Mock_ReadSensor();
    Mock_SerialWriteByte();
}
void TearDown_SetBridgeLED(void)
{
    TearDown_Mock();
    Restore_UsbReadBytes();
    Restore_ReadSensor();
    Restore_SerialWriteByte();
}
void SetBridgeLED_reads_and_ignores_Sensor_reply_to_SetBridgeLED(void)
{
    /* =====[ Setup ]===== */
    /* Inject two bytes of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test ]===== */
    // ---Check the Bridge reads the Sensor response to command GetBridgeLED---
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 2 is ReadSensor."
        );
    // ---Prove that the Bridge does not write this response to the USB host---
    // Assert one and only one subsequent call to SerialWriteByte.
    // Assert call to SerialWriteByte does not send the Sensor response.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 3 is SerialWriteByte."
        );
    uint8_t arg_n = 1; uint8_t arg_val = invalid_cmd;
    TEST_ASSERT_FALSE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_val),
        "Expect SerialWriteByte does not send the Sensor response (invalid_cmd)."
        );
    // Assert there are no more calls
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}

void SetBridgeLED_reads_two_bytes_of_payload_from_usb_host(void)
{
    /* Inject two bytes of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test: assert UsbReadBytes called to read 2 bytes ]===== */
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadBytes"));
    uint8_t arg_n = 2; uint16_t nbytes = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes),
        "Expect UsbReadBytes reads two bytes."
        );
}
void SetBridgeLED_replies_with_one_byte(void)
{
    /* Inject two bytes of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test ]===== */
    // ---Only the last call sends a byte to the USB host---
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadBytes"));
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "ReadSensor"));
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    // Check there are no more calls
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void SetBridgeLED_replies_msg_status_ok_if_led_number_is_status_led(void)
{
    /* Inject two bytes of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test ]===== */
    // ---Check Bridge sends status byte "ok"---
    uint8_t msg_status = ok;
    uint8_t call_n=3; uint8_t arg_n=1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &msg_status));
}
void SetBridgeLED_replies_msg_status_error_if_led_number_is_not_recognized(void)
{
    /* Test case: host sends a nonexistent led number. */
    uint8_t const led_number = led_0+100;
    /* Inject two bytes of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_number, led_off};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test ]===== */
    // ---Check Bridge sends status byte "error"---
    uint8_t msg_status = error;
    uint8_t call_n=3; uint8_t arg_n=1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &msg_status));
}
void SetBridgeLED_turns_off_led_if_payload_is_led_off(void)
{
    /* =====[ Test case: status_led is on before command is sent ]===== */
    BiColorLedOn(status_led);
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsOn(status_led),
        "status_led must be on before test begins."
        );
    /* Inject two bytes of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0, led_off};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    SetBridgeLED();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsOn(status_led),
        "Expect SetBridgeLED turns off status_led."
        );
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
    /* Inject two bytes of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0, led_green};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
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
    /* Inject two bytes of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0, led_red};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
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

void SetUp_BridgeGetSensorLED(void)
{
    SetUp_Mock();
    Mock_UsbReadBytes();
    Mock_SerialWriteByte();
    Mock_SpiWriteByte();
    Mock_WriteSensor();
    Mock_ReadSensor();
    // Fake the Sensor signaling it does *not* have a byte to send
    SetBit(Spi_pin, Spi_DataReady); // DataReady HIGH signals data is *not* ready
    // Now the injected Sensor responses are *not* consumed in `if (SensorHasResponse())`
}
void TearDown_BridgeGetSensorLED(void)
{
    TearDown_Mock();
    Restore_UsbReadBytes();
    Restore_SerialWriteByte();
    Restore_SpiWriteByte();
    Restore_WriteSensor();
    Restore_ReadSensor();
}
void BridgeGetSensorLED_reads_one_byte_of_host_payload(void)
{
    /* =====[ Setup ]===== */
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0+99};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test: assert UsbReadBytes called to read 1 byte ]===== */
    /* PrintAllCalls(mock); // view entire call history */
    uint8_t call_n = 1; uint16_t arg_n = 2; uint16_t nbytes = 1;
    // test call 1 is UsbReadBytes
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadBytes"));
    // test UsbReadBytes is called with nbytes=1
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &nbytes));
}
void BridgeGetSensorLED_checks_for_invalid_command_error_from_Sensor(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    // Bridge call n=2 is ReadSensor if Sensor is trying to send invalid cmd error
    uint8_t call_n=2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 2 is ReadSensor if Sensor is trying to send invalid_cmd."
        );
}
void BridgeGetSensorLED_does_not_send_payload_if_Sensor_says_invalid_cmd(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Now the injected Sensor responses are consumed in `if (SensorHasResponse())`
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    // Bridge call n=3 is SerialWriteByte if Sensor reply is invalid cmd error
    uint8_t call_n=3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 3 is SerialWriteByte if Sensor reply is invalid_cmd (0x02)."
        );
    // Bridge does not do anything else after this SerialWriteByte.
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void BridgeGetSensorLED_passes_invalid_cmd_reply_back_to_host(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Now the injected Sensor responses are consumed in `if (SensorHasResponse())`
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    // Bridge call n=3 is SerialWriteByte if Sensor reply is invalid cmd error
    uint8_t call_n=3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 3 is SerialWriteByte if Sensor reply is invalid_cmd (0x02)."
        );
    /* =====[ Test ]===== */
    uint8_t arg_n = 1; uint8_t arg_val = invalid_cmd;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_val),
        "Expect Bridge passes invalid_cmd (0x02) back to USB host."
        );
}
void BridgeGetSensorLED_responds_ok_if_Sensor_does_not_say_invalid_cmd(void)
{
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t payload[] = {led_0};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n = 2; uint8_t arg_n = 1; uint8_t reply = ok;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &reply));
}
void BridgeGetSensorLED_writes_led_number_to_Sensor(void)
{
    /* =====[ Setup ]===== */
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t bad_led_number = led_0+99;
    uint8_t payload[] = {bad_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n; uint8_t arg_value;
    //---Write payload byte---
    call_n = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SpiWriteByte"),
        "Expect call number 4 is SpiWriteByte."
        )
    arg_n = 1; arg_value = bad_led_number;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_value),
        "Expect SpiWriteByte sends led number."
        );
}
void BridgeGetSensorLED_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t bad_led_number = led_0+99;
    uint8_t payload[] = {bad_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    // Bridge reads a byte from sensor
    uint8_t call_n = 4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 4 is ReadSensor."
        );
    // The Bridge sends the USB host the Sensor msg status.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 5 is SerialWriteByte."
        );
    // Check that Bridge sends the Sensor response (see Setup) to the USB host
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: error (0x01)."
        );
}
void BridgeGetSensorLED_reads_and_sends_led_status_byte_if_Sensor_status_is_ok(void)
{
    /* =====[ Setup ]===== */
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t good_led_number = led_0;
    uint8_t payload[] = {good_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {ok, 0x02};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* Inject LED state */
    BiColorLedOn(led_0);
    BiColorLedRed(led_0); // sensor_responses[1] == 0x02;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    // ---Check that Sensor responds OK---
    // Bridge reads a byte from sensor
    uint8_t call_n = 4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 4 is ReadSensor."
        );
    // The Bridge sends the USB host the Sensor msg status.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 5 is SerialWriteByte."
        );
    // Assert Bridge sends Sensor response "ok"
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: ok (0x00)."
        );
    // ---Test: Bridge reads another byte---
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "ReadSensor"),
        "Expect call 6 is ReadSensor."
        );
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 7 is SerialWriteByte."
        );
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[1]),
        "Expect Bridge sends USB host Sensor response: led_red (0x02)."
        );
}
void BridgeGetSensorLED_reads_no_more_bytes_if_Sensor_status_is_error(void)
{
    /* =====[ Setup ]===== */
    /* Inject one byte of payload for fake UsbReadBytes. */
    uint8_t bad_led_number = led_0+99;
    uint8_t payload[] = {bad_led_number};
    FakeByteArray_ForUsbReadBytes = payload;
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorLED();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    // ---Assert Sensor response is "error"---
    uint8_t call_n = 4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 4 is ReadSensor."
        );
    // The Bridge sends the USB host the Sensor msg status.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 5 is SerialWriteByte."
        );
    // Check that Bridge sends the Sensor response (see Setup) to the USB host
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: error (0x01)."
        );
    // ---Test: no more calls---
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}

/* TODO: */
/* void BridgeGetSensorLED_responds_error_if_it_timeouts_waiting_for_host_payload(void) */

void SetUp_BridgeSetSensorLED(void)
{
    SetUp_Mock();
    Mock_UsbReadBytes();
    Mock_SerialWriteByte();
    Mock_SpiWriteByte();
    Mock_WriteSensor();
    Mock_ReadSensor();
    // Fake the Sensor signaling it does *not* have a byte to send
    SetBit(Spi_pin, Spi_DataReady); // DataReady HIGH signals data is *not* ready
    // Now the injected Sensor responses are *not* consumed in `if (SensorHasResponse())`
}
void TearDown_BridgeSetSensorLED(void)
{
    TearDown_Mock();
    Restore_UsbReadBytes();
    Restore_SerialWriteByte();
    Restore_SpiWriteByte();
    Restore_WriteSensor();
    Restore_ReadSensor();
}
void BridgeSetSensorLED_reads_two_bytes_of_host_payload(void)
{
    /* =====[ Setup ]===== */
    // Inject two bytes of payload for fake UsbReadBytes.
    uint8_t led_number = led_1; uint8_t led_state = led_red;
    uint8_t payload[] = {led_number, led_state};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Inject previous LED state different from desired state
    /* Inject LED state */
    BiColorLedOn(led_number);
    BiColorLedGreen(led_number);
    /* =====[ Operate ]===== */
    BridgeSetSensorLED();
    /* =====[ Test: assert UsbReadBytes called to read 2 bytes ]===== */
    uint8_t call_n = 1; uint16_t arg_n = 2; uint16_t nbytes = 2;
    // test call 1 is UsbReadBytes
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadBytes"));
    // test UsbReadBytes is called with nbytes=2
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &nbytes));
}
void BridgeSetSensorLED_checks_for_invalid_command_error_from_Sensor(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    /* =====[ Operate ]===== */
    BridgeSetSensorLED();
    /* =====[ Test ]===== */
    // Bridge call n=2 is ReadSensor if Sensor is trying to send invalid cmd error
    uint8_t call_n=2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 2 is ReadSensor if Sensor is trying to send invalid_cmd."
        );
}
void BridgeSetSensorLED_does_not_send_payload_if_Sensor_says_invalid_cmd(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Now the injected Sensor responses are consumed in `if (SensorHasResponse())`
    /* =====[ Operate ]===== */
    BridgeSetSensorLED();
    /* =====[ Test ]===== */
    // Bridge call n=3 is SerialWriteByte if Sensor reply is invalid cmd error
    uint8_t call_n=3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 3 is SerialWriteByte if Sensor reply is invalid_cmd (0x02)."
        );
    // Bridge does not do anything else after this SerialWriteByte.
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void BridgeSetSensorLED_passes_invalid_cmd_reply_back_to_host(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Now the injected Sensor responses are consumed in `if (SensorHasResponse())`
    /* =====[ Operate ]===== */
    BridgeSetSensorLED();
    // Bridge call n=3 is SerialWriteByte if Sensor reply is invalid cmd error
    uint8_t call_n=3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 3 is SerialWriteByte if Sensor reply is invalid_cmd (0x02)."
        );
    /* =====[ Test ]===== */
    uint8_t arg_n = 1; uint8_t arg_val = invalid_cmd;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_val),
        "Expect Bridge passes invalid_cmd (0x02) back to USB host."
        );
}
void BridgeSetSensorLED_responds_ok_if_Sensor_does_not_say_invalid_cmd(void)
{
    /* =====[ Setup ]===== */
    // Inject two bytes of payload for fake UsbReadBytes.
    uint8_t led_number = led_1; uint8_t led_state = led_red;
    uint8_t payload[] = {led_number, led_state};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Inject previous LED state different from desired state
    /* Inject LED state */
    BiColorLedOn(led_number);
    BiColorLedGreen(led_number);
    /* =====[ Operate ]===== */
    BridgeSetSensorLED();
    /* =====[ Test: Bridge responds "ok" after receiving host payload ]===== */
    uint8_t call_n = 2; uint16_t arg_n = 1; uint8_t arg_val = ok;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_val));
}
void BridgeSetSensorLED_passes_two_bytes_of_payload_to_Sensor(void)
{
    /* =====[ Setup ]===== */
    // Inject two bytes of payload for fake UsbReadBytes.
    uint8_t led_number = led_1; uint8_t led_state = led_red;
    uint8_t payload[] = {led_number, led_state};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Inject previous LED state different from desired state
    /* Inject LED state */
    BiColorLedOn(led_number);
    BiColorLedGreen(led_number);
    /* =====[ Operate ]===== */
    BridgeSetSensorLED();
    /* =====[ Test: Bridge passes payload to Sensor ]===== */
    uint8_t call_n = 3; uint16_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SpiWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &payload[0]));
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SpiWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &payload[1]));
}
void BridgeSetSensorLED_reads_and_sends_one_byte_Sensor_reply_to_host(void)
{
    /* =====[ Setup ]===== */
    // Inject two bytes of payload for fake UsbReadBytes.
    uint8_t led_number = led_1; uint8_t led_state = led_red;
    uint8_t payload[] = {led_number, led_state};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Inject previous LED state different from desired state
    /* Inject LED state */
    BiColorLedOn(led_number);
    BiColorLedGreen(led_number);
    /* =====[ Operate ]===== */
    BridgeSetSensorLED();
    /* =====[ Test: Bridge reads Sensor reply ]===== */
    uint8_t call_n = 5; uint8_t arg_n = 2; uint16_t arg_val = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "ReadSensor"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_val));
    /* =====[ Test: Bridge passes Sensor reply to USB host ]===== */
    arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &sensor_responses[0]));
}

void SetUp_BridgeSetSensorConfig(void)
{
    SetUp_Mock();
    Mock_UsbReadBytes();
    Mock_ReadSensor();
    Mock_SerialWriteByte();
    Mock_SpiWriteByte();
    // Fake the Sensor signaling it does *not* have a byte to send
    SetBit(Spi_pin, Spi_DataReady); // DataReady HIGH signals data is *not* ready
    // Now the injected Sensor responses are *not* consumed in `if (SensorHasResponse())`
}
void TearDown_BridgeSetSensorConfig(void)
{
    TearDown_Mock();
    Restore_UsbReadBytes();
    Restore_ReadSensor();
    Restore_SerialWriteByte();
    Restore_SpiWriteByte();
}
void BridgeSetSensorConfig_reads_three_bytes_of_host_payload(void)
{
    /* =====[ Setup ]===== */
    // Inject three bytes of payload for fake UsbReadBytes.
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    uint8_t payload[] = {binning, gain, active_rows};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Inject previous config different from desired config
    /* Inject config */
    // BiColorLedOn(led_number);
    // BiColorLedGreen(led_number);
    /* =====[ Operate ]===== */
    BridgeSetSensorConfig();
    /* =====[ Test: assert UsbReadBytes called to read 2 bytes ]===== */
    uint8_t call_n = 1; uint16_t arg_n = 2; uint16_t nbytes = 3;
    // test call 1 is UsbReadBytes
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "UsbReadBytes"));
    // test UsbReadBytes is called with nbytes=3
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &nbytes));
}
void BridgeSetSensorConfig_sends_msg_status_ok_after_reading_payload(void)
{
    /* =====[ Setup ]===== */
    // Inject three bytes of payload for fake UsbReadBytes.
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    uint8_t payload[] = {binning, gain, active_rows};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeSetSensorConfig();
    /* =====[ Test: Bridge responds "ok" after receiving host payload ]===== */
    uint8_t call_n = 2; uint16_t arg_n = 1; uint8_t arg_val = ok;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_val));
}
void BridgeSetSensorConfig_checks_for_invalid_command_error_from_Sensor(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    /* =====[ Operate ]===== */
    BridgeSetSensorConfig();
    /* =====[ Test ]===== */
    // Bridge call n=3 is ReadSensor if Sensor is trying to send invalid cmd error
    uint8_t call_n=3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 3 is ReadSensor if Sensor is trying to send invalid_cmd."
        );
}
void BridgeSetSensorConfig_does_not_send_payload_if_Sensor_says_invalid_cmd(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Now the injected Sensor responses are consumed in `if (SensorHasResponse())`
    /* =====[ Operate ]===== */
    BridgeSetSensorConfig();
    /* =====[ Test ]===== */
    // Bridge call n=4 is SerialWriteByte if Sensor reply is invalid cmd error
    uint8_t call_n=4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 4 is SerialWriteByte if Sensor reply is invalid_cmd (0x02)."
        );
    // Bridge does not do anything else after this SerialWriteByte.
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void BridgeSetSensorConfig_passes_invalid_cmd_reply_back_to_host(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Now the injected Sensor responses are consumed in `if (SensorHasResponse())`
    /* =====[ Operate ]===== */
    BridgeSetSensorConfig();
    // Bridge call n=4 is SerialWriteByte if Sensor reply is invalid cmd error
    uint8_t call_n=4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 4 is SerialWriteByte if Sensor reply is invalid_cmd (0x02)."
        );
    /* =====[ Test ]===== */
    uint8_t arg_n = 1; uint8_t arg_val = invalid_cmd;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_val),
        "Expect Bridge passes invalid_cmd (0x02) back to USB host."
        );
}
void BridgeSetSensorConfig_passes_three_bytes_of_payload_to_Sensor_if_it_does_not_say_invalid_cmd(void)
{
    /* =====[ Setup ]===== */
    // Inject three bytes of payload for fake UsbReadBytes.
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    uint8_t payload[] = {binning, gain, active_rows};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeSetSensorConfig();
    /* =====[ Test: Bridge passes payload to Sensor ]===== */
    uint8_t call_n = 3; uint16_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SpiWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &payload[0]));
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SpiWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &payload[1]));
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SpiWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &payload[2]));
}
void BridgeSetSensorConfig_reads_and_sends_one_byte_Sensor_reply_to_host(void)
{
    /* =====[ Setup ]===== */
    // Inject three bytes of payload for fake UsbReadBytes.
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    uint8_t payload[] = {binning, gain, active_rows};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeSetSensorConfig();
    /* =====[ Test: Bridge reads Sensor reply ]===== */
    uint8_t call_n = 6; uint8_t arg_n = 2; uint16_t arg_val = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "ReadSensor"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_val));
    /* =====[ Test: Bridge passes Sensor reply to USB host ]===== */
    arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &sensor_responses[0]));
}

void SetUp_BridgeGetSensorConfig(void)
{
    SetUp_Mock();
    Mock_ReadSensor();
    Mock_SerialWriteByte();
    // Fake the Sensor signaling it does *not* have a byte to send
    SetBit(Spi_pin, Spi_DataReady); // DataReady HIGH signals data is *not* ready
}
void TearDown_BridgeGetSensorConfig(void)
{
    TearDown_Mock();
    Restore_ReadSensor();
    Restore_SerialWriteByte();
}
void BridgeGetSensorConfig_replies_ok_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorConfig();
    // Bridge sends OK to USB Host
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 1 is SerialWriteByte."
        );
    uint8_t arg_n = 1; uint8_t arg_val = ok;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_val),
        "Expect Bridge sends OK (0x00)."
        );
}
void BridgeGetSensorConfig_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorConfig();
    /* =====[ Test ]===== */
    // Bridge reads a byte from sensor
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 2 is ReadSensor."
        );
    // The Bridge sends the USB host the Sensor msg status.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 2 is SerialWriteByte."
        );
    // Check that Bridge sends the Sensor response (see Setup) to the USB host
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: error (0x01)."
        );
}
void BridgeGetSensorConfig_reads_no_more_bytes_if_Sensor_status_is_error(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorConfig();
    // ---Assert Sensor response is "error"---
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 1 is ReadSensor."
        );
    // The Bridge sends the USB host the Sensor msg status.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 2 is SerialWriteByte."
        );
    // Check that Bridge sends the Sensor response (see Setup) to the USB host
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: error (0x01)."
        );
    /* =====[ Test ]===== */
    // ---Test: no more calls---
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void BridgeGetSensorConfig_if_status_is_ok_then_read_config_data_from_Sensor_and_send_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    uint8_t sensor_responses[] = {ok, binning, gain, active_rows};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetSensorConfig();
    // ---Assert Sensor response is "ok"---
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 1 is ReadSensor."
        );
    // The Bridge sends the USB host the Sensor msg status.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call 2 is SerialWriteByte."
        );
    // Check that Bridge sends the Sensor response (see Setup) to the USB host
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: ok (0x00)."
        );
    /* =====[ Test ]===== */
    // Bridge gets three bytes of data from Sensor
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "ReadSensor"),
        "Expect call 3 is ReadSensor."
        );
    arg_n = 2; uint16_t nbytes = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes),
        "Expect Bridge reads 3 bytes of Sensor data."
        );
    // Bridge passes three bytes of Sensor data to USB host
    arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[1]),
        "Expect Bridge sends USB host Sensor Config byte 1."
        );
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[2]),
        "Expect Bridge sends USB host Sensor Config byte 2."
        );
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[3]),
        "Expect Bridge sends USB host Sensor Config byte 3."
        );
    // Assert there are no more calls
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}

void SetUp_BridgeGetExposure(void)
{
    SetUp_Mock();
    Mock_SerialWriteByte();
    Mock_ReadSensor();
}
void TearDown_BridgeGetExposure(void)
{
    TearDown_Mock();
    Restore_SerialWriteByte();
    Restore_ReadSensor();
}
void BridgeGetExposure_replies_ok_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    //  Inject Sensor responses.
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetExposure();
    /* =====[ Test ]===== */
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 1 is SerialWriteByte."
        );
    uint8_t arg_n = 1; uint8_t status = ok;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &status),
        "Expect Bridge sends USB host status byte ok (0x00)."
        );
}
void BridgeGetExposure_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    //  Inject Sensor responses.
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetExposure();
    /* =====[ Test ]===== */
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 2 is ReadSensor."
        );
    uint8_t arg_n = 2; uint16_t nbytes = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes),
        "Expect Bridge reads one byte of status from Sensor."
        );
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect call after ReadSensor is SerialWriteByte."
        );
    arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge reads one byte of status from Sensor."
        );
}
void BridgeGetExposure_reads_no_more_bytes_if_Sensor_status_is_error(void)
{
    /* =====[ Setup ]===== */
    //  Inject Sensor responses.
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetExposure();
    /* =====[ Test ]===== */
    uint8_t call_n = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect last call is SerialWriteByte."
        );
    uint8_t arg_n = 1; uint8_t status = ok;
    TEST_ASSERT_FALSE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &status),
        "Expect Bridge does not receive status OK (0x00)."
        );
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
// on hold
void BridgeGetExposure_reads_two_bytes_of_exposure_time_from_Sensor_MSB_first_and_sends_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    //  Inject Sensor responses.
    uint16_t exposure_ticks = 50; // multiply by 20us to get integration time in seconds
    uint8_t exposure_msb = exposure_ticks >> 8;
    uint8_t exposure_lsb = exposure_ticks & 0xFF;
    uint8_t sensor_responses[] = {ok, exposure_msb, exposure_lsb};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeGetExposure();
    /* =====[ Test ]===== */
    uint8_t call_n = 4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 4 is ReadSensor."
        );
    uint8_t arg_n = 2; uint16_t nbytes = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes),
        "Expect Bridge reads two bytes of exposure time data MSB first."
        );
    arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &sensor_responses[1]));
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &sensor_responses[2]));
}

void SetUp_BridgeSetExposure(void)
{
    SetUp_Mock();
    // Fake the Sensor signaling it does *not* have a byte to send
    SetBit(Spi_pin, Spi_DataReady); // DataReady HIGH signals data is *not* ready
    // Now the injected Sensor responses are *not* consumed in `if (SensorHasResponse())`
    Mock_UsbReadBytes();
    Mock_SerialWriteByte();
    Mock_ReadSensor();
    Mock_SpiWriteByte();
}
void TearDown_BridgeSetExposure(void)
{
    TearDown_Mock();
    Restore_UsbReadBytes();
    Restore_SerialWriteByte();
    Restore_ReadSensor();
    Restore_SpiWriteByte();
}
void BridgeSetExposure_reads_two_bytes_of_payload_from_usb_host(void)
{
    /* =====[ Setup ]===== */
    // Inject two bytes of payload for fake UsbReadBytes.
    uint16_t exposure_ticks = 50; // multiply by 20us to get integration time in seconds
    uint8_t exposure_msb = exposure_ticks >> 8;
    uint8_t exposure_lsb = exposure_ticks & 0xFF;
    uint8_t payload[] = {exposure_msb, exposure_lsb};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    /* =====[ Operate ]===== */
    BridgeSetExposure();
    /* =====[ Test ]===== */
    // BridgeSetExposure reads two bytes from the host.
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "UsbReadBytes"),
        "Expect call 1 is UsbReadBytes."
        );
    uint8_t arg_n = 2; uint16_t nbytes = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes),
        "Expect Bridge reads two bytes from USB host."
        );
}
void BridgeSetExposure_sends_msg_status_ok_to_usb_host(void)
{
    /* =====[ Setup ]===== */
    // Inject two bytes of payload for fake UsbReadBytes.
    uint16_t exposure_ticks = 50; // multiply by 20us to get integration time in seconds
    uint8_t exposure_msb = exposure_ticks >> 8;
    uint8_t exposure_lsb = exposure_ticks & 0xFF;
    uint8_t payload[] = {exposure_msb, exposure_lsb};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    /* =====[ Operate ]===== */
    BridgeSetExposure();
    /* =====[ Test ]===== */
    // BridgeSetExposure reads two bytes from the host.
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 2 is SerialWriteByte."
        );
    uint8_t arg_n = 1; uint8_t bridge_status = ok;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &bridge_status),
        "Expect Bridge sends status byte ok (0x00) to USB host."
        );
}
void BridgeSetExposure_checks_for_invalid_command_error_from_Sensor(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    /* =====[ Operate ]===== */
    BridgeSetExposure();
    /* =====[ Test ]===== */
    // Bridge call n=3 is ReadSensor if Sensor is trying to send invalid cmd error
    uint8_t call_n=3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 3 is ReadSensor if Sensor is trying to send invalid_cmd."
        );
}
void BridgeSetExposure_does_not_send_payload_if_Sensor_says_invalid_cmd(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Now the injected Sensor responses are consumed in `if (SensorHasResponse())`
    /* =====[ Operate ]===== */
    BridgeSetExposure();
    /* =====[ Test ]===== */
    // Bridge call n=4 is SerialWriteByte if Sensor reply is invalid cmd error
    uint8_t call_n=4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 4 is SerialWriteByte if Sensor reply is invalid_cmd (0x02)."
        );
    // Bridge does not do anything else after this SerialWriteByte.
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void BridgeSetExposure_passes_invalid_cmd_reply_back_to_host(void)
{
    /* =====[ Setup ]===== */
    // If Sensor thinks command is invalid, it responds immediately
    // Inject Sensor Response: invalid command error
    uint8_t sensor_responses[] = {invalid_cmd};
    FakeByteArray_ForReadSensor = sensor_responses;
    // Fake the Sensor signaling it has a byte to send
    ClearBit(Spi_pin, Spi_DataReady); // DataReady LOW signals ready
    // Now the injected Sensor responses are consumed in `if (SensorHasResponse())`
    /* =====[ Operate ]===== */
    BridgeSetExposure();
    // Bridge call n=4 is SerialWriteByte if Sensor reply is invalid cmd error
    uint8_t call_n=4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 4 is SerialWriteByte if Sensor reply is invalid_cmd (0x02)."
        );
    /* =====[ Test ]===== */
    uint8_t arg_n = 1; uint8_t arg_val = invalid_cmd;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &arg_val),
        "Expect Bridge passes invalid_cmd (0x02) back to USB host."
        );
}
void BridgeSetExposure_passes_two_bytes_of_payload_to_Sensor_if_it_does_not_say_invalid_cmd(void)
{
    /* =====[ Setup ]===== */
    // Inject two bytes of payload for fake UsbReadBytes.
    uint16_t exposure_ticks = 50; // multiply by 20us to get integration time in seconds
    uint8_t exposure_msb = exposure_ticks >> 8;
    uint8_t exposure_lsb = exposure_ticks & 0xFF;
    uint8_t payload[] = {exposure_msb, exposure_lsb};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeSetExposure();
    /* =====[ Test: Bridge passes payload to Sensor ]===== */
    uint8_t call_n = 3; uint16_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "SpiWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &payload[0]));
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SpiWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &payload[1]));
}
void BridgeSetExposure_reads_and_sends_one_byte_Sensor_reply_to_host(void)
{
    /* =====[ Setup ]===== */
    // Inject two bytes of payload for fake UsbReadBytes.
    uint16_t exposure_ticks = 50; // multiply by 20us to get integration time in seconds
    uint8_t exposure_msb = exposure_ticks >> 8;
    uint8_t exposure_lsb = exposure_ticks & 0xFF;
    uint8_t payload[] = {exposure_msb, exposure_lsb};
    FakeByteArray_ForUsbReadBytes = payload;
    // Inject Sensor responses.
    uint8_t sensor_responses[] = {ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeSetExposure();
    /* =====[ Test: Bridge reads Sensor reply ]===== */
    uint8_t call_n = 5; uint8_t arg_n = 2; uint16_t arg_val = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "ReadSensor"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_val));
    /* =====[ Test: Bridge passes Sensor reply to USB host ]===== */
    arg_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &sensor_responses[0]));
}

void SetUp_BridgeCaptureFrame(void)
{
    SetUp_Mock();
    Mock_SerialWriteByte();
    Mock_ReadSensor();
}
void TearDown_BridgeCaptureFrame(void)
{
    TearDown_Mock();
    Restore_SerialWriteByte();
    Restore_ReadSensor();
}
void BridgeCaptureFrame_sends_msg_status_ok_to_usb_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeCaptureFrame();
    /* =====[ Test ]===== */
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 1 is SerialWriteByte"
        );
    uint8_t arg_n = 1; uint8_t bridge_status = ok;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &bridge_status),
        "Expect Bridge sends OK (0x00)."
        );
}
void BridgeCaptureFrame_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeCaptureFrame();
    /* =====[ Test ]===== */
    // Bridge reads a byte from sensor
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSensor"),
        "Expect call 2 is ReadSensor."
        );
    // The Bridge sends the USB host the Sensor msg status.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "SerialWriteByte"),
        "Expect next call is SerialWriteByte."
        );
    // Check that Bridge sends the Sensor response (see Setup) to the USB host
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: error (0x01)."
        );
}
void BridgeCaptureFrame_reads_no_more_bytes_if_Sensor_status_is_error(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint8_t sensor_responses[] = {error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeCaptureFrame();
    uint8_t call_n = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 3 is SerialWriteByte."
        );
    // Check that Bridge sends the Sensor response (see Setup) to the USB host
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: error (0x01)."
        );
    /* =====[ Test ]===== */
    // Assert there are no more calls.
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void BridgeCaptureFrame_reads_npixels_in_frame_and_sends_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    /* uint8_t sensor_responses[] = {ok, 0x01, 0x88}; */
    uint16_t npixels_in_frame;
    uint8_t binning = binning_on;
    if (binning == binning_on) npixels_in_frame = npixels>>1;
    else npixels_in_frame = npixels;
    // Split 16-bit npixels into two bytes
    uint8_t npixels_msb = npixels_in_frame >> 8;
    uint8_t npixels_lsb = npixels_in_frame & 0xFF;
    // Fake the first three bytes of response
    uint8_t sensor_responses[] = {ok, npixels_msb, npixels_lsb};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeCaptureFrame();
    /* =====[ Test ]===== */
    // Assert Sensor status is OK by looking at response sent by Bridge
    uint8_t call_n = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 3 is SerialWriteByte."
        );
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[0]),
        "Expect Bridge sends USB host Sensor response: ok (0x00)."
        );
    // Assert Bridge reads two bytes containing the number of pixels
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "ReadSensor"),
        "Expect call 4 is ReadSensor."
        );
    arg_n = 2; uint16_t nbytes = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes),
        "Expect Bridge reads two bytes."
        );
    // Bridge sends the two bytes to the USB host
    arg_n = 1;
    // First byte is msb
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &sensor_responses[1]));
    // Second byte is lsb
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &sensor_responses[2]));
}
void BridgeCaptureFrame_reads_another_status_byte_from_Sensor_and_sends_to_USB_host(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint16_t npixels_in_frame;
    uint8_t binning = binning_on;
    if (binning == binning_on) npixels_in_frame = npixels>>1;
    else npixels_in_frame = npixels;
    // Split 16-bit npixels into two bytes
    uint8_t npixels_msb = npixels_in_frame >> 8;
    uint8_t npixels_lsb = npixels_in_frame & 0xFF;
    // Fake the first three bytes of response
    uint8_t sensor_responses[] = {ok, npixels_msb, npixels_lsb, error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeCaptureFrame();
    /* =====[ Test ]===== */
    // Assert Sensor status is ERROR by looking at response sent by Bridge
    uint8_t call_n = 8;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 8 is SerialWriteByte."
        );
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[3]),
        "Expect Bridge sends USB host Sensor response: error (0x01)."
        );
}
void BridgeCaptureFrame_does_not_read_frame_if_Sensor_status_is_error(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint16_t npixels_in_frame;
    uint8_t binning = binning_on;
    if (binning == binning_on) npixels_in_frame = npixels>>1;
    else npixels_in_frame = npixels;
    // Split 16-bit npixels into two bytes
    uint8_t npixels_msb = npixels_in_frame >> 8;
    uint8_t npixels_lsb = npixels_in_frame & 0xFF;
    // Fake the first three bytes of response
    uint8_t sensor_responses[] = {ok, npixels_msb, npixels_lsb, error};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeCaptureFrame();
    // Assert Sensor status is ERROR by looking at response sent by Bridge
    uint8_t call_n = 8;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 8 is SerialWriteByte."
        );
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[3]),
        "Expect Bridge sends USB host Sensor response: error (0x01)."
        );
    /* =====[ Test ]===== */
    // Assert there are no more calls
    TEST_ASSERT_EQUAL_UINT8(call_n,NumberOfActualCalls(mock));
}
void BridgeCaptureFrame_reads_and_sends_frame_if_Sensor_status_is_ok(void)
{
    /* =====[ Setup ]===== */
    /* Inject Sensor responses. */
    uint16_t npixels_in_frame;
    uint8_t binning = binning_on;
    if (binning == binning_on) npixels_in_frame = npixels>>1;
    else npixels_in_frame = npixels;
    // Split 16-bit npixels into two bytes
    uint8_t npixels_msb = npixels_in_frame >> 8;
    uint8_t npixels_lsb = npixels_in_frame & 0xFF;
    // Fake the first three bytes of response
    uint8_t sensor_responses[] = {ok, npixels_msb, npixels_lsb, ok};
    FakeByteArray_ForReadSensor = sensor_responses;
    /* =====[ Operate ]===== */
    BridgeCaptureFrame();
    // Assert Sensor status is OK by looking at response sent by Bridge
    uint8_t call_n = 8;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SerialWriteByte"),
        "Expect call 8 is SerialWriteByte."
        );
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &sensor_responses[3]),
        "Expect Bridge sends USB host Sensor response: ok (0x00)."
        );
    /* =====[ Test ]===== */
    // Assert ReadSensor reads the expected number of bytes in the frame
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, ++call_n, "ReadSensor"),
        "Expect call 9 is ReadSensor."
        );
    // Calculate nbytes_in_frame
    uint16_t nbytes_in_frame = npixels_in_frame*2;
    arg_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes_in_frame),
        "Expect Bridge reads 784 bytes of frame data from the Sensor."
        );
    // Expect another 784 calls to SerialWriteByte
    uint16_t expected_ncalls = (uint16_t)call_n + nbytes_in_frame;
    TEST_ASSERT_EQUAL_UINT16(expected_ncalls,NumberOfActualCalls(mock));
    TEST_ASSERT_TRUE(AssertCall(mock, ++call_n, "SerialWriteByte"));
    TEST_ASSERT_TRUE(AssertCall(mock, expected_ncalls, "SerialWriteByte"));
}
