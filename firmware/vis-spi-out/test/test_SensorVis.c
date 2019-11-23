#include "test_SensorVis.h"
#include "SensorVis.h"   // lib under test
/* ---Unit Test Framework--- */
#include "unity.h"          // unity macros: TEST_BLAH
#include <Mock.h>           // record call history: PrintAllCalls, AssertCalls, AssertArgs
/* ---Define a setup/teardown for writing call history--- */
static void SetUp_Mock(void) { mock = Mock_new(); }
static void TearDown_Mock(void) { Mock_destroy(mock); mock = NULL; }

/* Hardware fakes are included by test_runner.c. */
/* ---Fake hardware dependencies defined in the test_runner.c translation unit--- */
extern uint8_t const led_TxRx;
extern uint8_t const led_Done;
/* ---Project Lib Dependencies--- */
#include "BiColorLed.h"
/* ---Mock Out Libs That Read Hardware Registers--- */
#include "mock_Spi.h"
/* ---Global Queue--- */
#include "Queue.h"          // SensorVis accesses the Spi Rx Buffer
#define max_length_of_queue 5 // bytes
// SpiFifo must be global in main application for linking against its extern
// declaration in SensorVis
// `extern` builds SensorVis.o to use same SpiFifo defined in  main application.
volatile Queue_s * SpiFifo;

// Photodiode Array Config must be global in main application for linking
// against its extern declaration in SensorVis
// `extern` builds SensorVis.o to use same data defined in main application.
uint8_t binning; uint8_t gain; uint8_t active_rows;
// Exposure Time must be global in main application for linking
// against its extern declaration in SensorVis
// `extern` builds SensorVis.o to use same data defined in main application.
uint16_t exposure_ticks;
// Frame of pixel data must be global in main application for linking
// against its extern declaration in SensorVis
// `extern` builds SensorVis.o to use same data defined in main application.
uint8_t frame[npixels*2];

/* ---Queue Plumbing and Examples--- */
void test_lib_SensorVis_can_use_lib_Queue_and_sees_SpiFifo(void)
{
    // This test passes if the build succeedes.
    // The build fails if this application does not make SpiFifo global.

    /* =====[ To Use the API ]===== */
    // app .c file:
        // #include SensorVis.h
        // #include "Queue.h"
        // volatile Queue_s * SpiFifo; // global pointer shared with SensorVis
    // main() in app .c file:
    // Allocate a global buffer:
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    // Tie the Queue pointer to the global Queue and its buffer.
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);

    /* =====[ Memory Setup ]===== */
    // Only one Queue instance can exist at a time.
    //
    // "Queue" lives in static memory and is defined by the Queue lib.
        // Struct "Queue" is defined in Queue.c.
        // The memory for "Queue" is only allocated by Queue.c.
    // The Queue puts data in a buffer.
        // The buffer is defined by the application.
    // "SpiFifo" points to the "Queue" struct.
        // "SpiFifo" lives in static memory and is defined by the application.
        // Any other .c files that use SpiFifo simply declare it as extern
}
void test_Queue_lib_examples_of_push_and_pop(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);

    /* =====[ Test ]===== */
    // Try out the API.

    // It's good form before a Push to make sure the Queue is *not full*.
    TEST_ASSERT_FALSE(QueueIsFull(SpiFifo));
    QueuePush(SpiFifo, 11);
    TEST_ASSERT_FALSE(QueueIsFull(SpiFifo));
    QueuePush(SpiFifo, 22);
    TEST_ASSERT_FALSE(QueueIsFull(SpiFifo));
    QueuePush(SpiFifo, 33);

    // It's good form before a Pop to make sure the Queue is *not empty*.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    uint8_t first_out = QueuePop(SpiFifo);

    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    uint8_t second_out = QueuePop(SpiFifo);

    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    uint8_t third_out = QueuePop(SpiFifo);

    // Show this is a FIFO.
    TEST_ASSERT_EQUAL_UINT8(11, first_out); // first in is first out?
    TEST_ASSERT_EQUAL_UINT8(22, second_out); // second in is second out?
    TEST_ASSERT_EQUAL_UINT8(33, third_out); // third in is third out?
}
void test_Queue_lib_example_of_push_wrap_around(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // NOTE: it's important for this test to start with an empty queue
    // *and* with head and tail pointed at byte 0 of the buffer.
    //
    // In general, empty the Queue by simply initializing it again.
    // This does not initialize the values in the buffer,
    // but it points head and tail to byte 0 of the buffer.
    // head==tail is the condition checked by QueueIsEmpty.

    // Fill the Queue with byte value 99. When it is full, check it's length.
    uint16_t bytes_pushed = 0;
    while(bytes_pushed < max_length_of_queue)
    {
        QueuePush(SpiFifo, 99);
        bytes_pushed++;
    }
    // Check correct test setup: assert bytes_pushed == max_length
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(
        max_length_of_queue, bytes_pushed,
        "Failed to set this test up correctly. "
        "Must push exact number of bytes to fill the buffer."
        );
    // Is length of Queue == max_length_of_queue?
    TEST_ASSERT_EQUAL_UINT16(max_length_of_queue, QueueLength(SpiFifo));

    // Pop off one byte so there is *room* to push one byte.
    QueuePop(SpiFifo);

    // Since the queue was emptied just before it was filled with 99s, we know
    // the next Push should wrap around to the beginning of the buffer.
    QueuePush(SpiFifo, 11);
    // Assert that byte 0 has the value just pushed (instead of 99).
    TEST_ASSERT_EQUAL_UINT8(11, spi_rx_buffer[0]);
}



/* ---GetSensorLED--- */
/* =====[ Mock WriteSpiMaster() ]===== */
static RecordedCall * Record_WriteSpiMaster(uint8_t const *arg1, uint16_t arg2)
{ // Define call recorded when func-under-test calls mocked function.
    char const *call_name = "WriteSpiMaster";
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
// Global for test to spy on array input arg to WriteSpiMaster.
#define max_sizeof_write_buffer 1596+1 // plus 1 for status byte
uint8_t SpyOn_WriteSpiMaster_arg1[max_sizeof_write_buffer]; uint8_t *spy_writer;
static uint16_t WriteSpiMaster_Mocked(uint8_t const *write_buffer, uint16_t nbytes)
{ // Define behavior of mocked function: WriteSpiMaster().
    RecordActualCall(mock, Record_WriteSpiMaster(write_buffer, nbytes));
    /* Spy on values passed to write_buffer by storing them in a global */
    /* uint8_t *spy_writer = SpyOn_WriteSpiMaster_arg1; */
    uint16_t num_bytes_sent = 0;
    while(num_bytes_sent < nbytes)
    {
        *(spy_writer++) = *(write_buffer++);
        num_bytes_sent++;
    }
    return num_bytes_sent;
}
/* =====[ Define how to swap function definitions ]===== */
static uint16_t (*WriteSpiMaster_Saved)(uint8_t const *, uint16_t);
static void Restore_WriteSpiMaster(void)
{ // how to restore real definition
    WriteSpiMaster = WriteSpiMaster_Saved;
}
static void Mock_WriteSpiMaster(void)
{ // how to swap real definition with mocked version
    WriteSpiMaster_Saved = WriteSpiMaster;
    WriteSpiMaster = WriteSpiMaster_Mocked;
}

/* =====[ Mock GetFrame() ]===== */
static RecordedCall * Record_GetFrame(void)
{ // Define call recorded when func-under-test calls mocked function.
    char const *call_name = "GetFrame";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
static void GetFrame_Mocked(void)
{ // Define behavior of mocked function: GetFrame().
    RecordActualCall(mock, Record_GetFrame());
}
/* =====[ Define how to swap function definitions ]===== */
static void (*GetFrame_Saved)(void);
static void Restore_GetFrame(void)
{ // how to restore real definition
    GetFrame = GetFrame_Saved;
}
static void Mock_GetFrame(void)
{ // how to swap real definition with mocked version
    GetFrame_Saved = GetFrame;
    GetFrame = GetFrame_Mocked;
}

void SetUp_GetSensorLED(void)
{
    SetUp_Mock();
    Mock_WriteSpiMaster(); spy_writer = SpyOn_WriteSpiMaster_arg1;
}
void TearDown_GetSensorLED(void)
{
    TearDown_Mock();
    Restore_WriteSpiMaster();
}
void GetSensorLED_receives_led_number(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_led_number = 99;
    QueuePush(SpiFifo, fake_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    GetSensorLED();
    // QueueIsEmpty immediately returns false because the test Setup queued a byte.
    /* =====[ Test ]===== */
    // Check that GetSensorLED pops the fake_led_number from the queue.
    // Assert that the queue is now empty.
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
}
void GetSensorLED_replies_msg_status_error_if_led_does_not_exist(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_bad_led_number = 99;
    QueuePush(SpiFifo, fake_bad_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    GetSensorLED();
    // Assert reply with one byte:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t arg_val = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_val));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        error, SpyOn_WriteSpiMaster_arg1[0],
        "Expect status ERROR (0x01)."
        );
}
void GetSensorLED_sends_no_additional_bytes_if_msg_status_is_error(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_bad_led_number = 99;
    QueuePush(SpiFifo, fake_bad_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    GetSensorLED();
    // Assert reply with one byte:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t arg_val = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_val));
    /* =====[ Test ]===== */
    // Show human reading test results that status is "error"
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    // Assert no additional bytes sent back to Bridge
    TEST_ASSERT_EQUAL_UINT8(call_n, NumberOfActualCalls(mock));
}
void GetSensorLED_replies_msg_status_ok_if_led_number_is_recognized(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_good_led_number = led_TxRx;
    QueuePush(SpiFifo, fake_good_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    // Fake the led_status to check for.
    BiColorLedOn(led_TxRx);
    BiColorLedRed(led_TxRx);
    /* =====[ Operate ]===== */
    GetSensorLED();
    // Assert reply with two bytes:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t assert_val = 2;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &assert_val));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x00, SpyOn_WriteSpiMaster_arg1[0],
        "Expect OK (0x00)."
        );
}
void GetSensorLED_sends_led_status_byte_after_sending_msg_status_ok(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_good_led_number = led_TxRx;
    QueuePush(SpiFifo, fake_good_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    // Fake the led_status to check for.
    BiColorLedOn(led_TxRx);
    BiColorLedRed(led_TxRx);
    /* =====[ Operate ]===== */
    GetSensorLED();
    // Assert reply with two bytes:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t assert_val = 2;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &assert_val));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    printf("WriteSpiMaster called with write_buffer[1] == %d\n", SpyOn_WriteSpiMaster_arg1[1]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x00, SpyOn_WriteSpiMaster_arg1[0],
        "Expect OK (0x00)."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_red, SpyOn_WriteSpiMaster_arg1[1],
        "Expect led_status is led_red."
        );
}
void GetSensorLED_replies_led_off_if_led_is_off(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_good_led_number = led_TxRx;
    QueuePush(SpiFifo, fake_good_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    // Fake the led_status to check for.
    BiColorLedOff(led_TxRx);
    /* =====[ Operate ]===== */
    GetSensorLED();
    // Assert reply with two bytes:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t assert_val = 2; // bytes
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &assert_val));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    printf("WriteSpiMaster called with write_buffer[1] == %d\n", SpyOn_WriteSpiMaster_arg1[1]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        ok, SpyOn_WriteSpiMaster_arg1[0],
        "Expect OK (0x00)."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_off, SpyOn_WriteSpiMaster_arg1[1],
        "Expect led_status is led_off."
        );
}
void GetSensorLED_replies_led_green_if_led_is_green(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_good_led_number = led_TxRx;
    QueuePush(SpiFifo, fake_good_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    // Fake the led_status to check for.
    BiColorLedOn(led_TxRx);
    BiColorLedGreen(led_TxRx);
    /* =====[ Operate ]===== */
    GetSensorLED();
    // Assert reply with two bytes:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t assert_val = 2; // bytes
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &assert_val));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    printf("WriteSpiMaster called with write_buffer[1] == %d\n", SpyOn_WriteSpiMaster_arg1[1]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        ok, SpyOn_WriteSpiMaster_arg1[0],
        "Expect OK (0x00)."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_green, SpyOn_WriteSpiMaster_arg1[1],
        "Expect led_status is led_green."
        );
}
void GetSensorLED_replies_led_red_if_led_is_red(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_good_led_number = led_TxRx;
    QueuePush(SpiFifo, fake_good_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    // Fake the led_status to check for.
    BiColorLedOn(led_TxRx);
    BiColorLedRed(led_TxRx);
    /* =====[ Operate ]===== */
    GetSensorLED();
    // Assert reply with two bytes:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t assert_val = 2; // bytes
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &assert_val));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    printf("WriteSpiMaster called with write_buffer[1] == %d\n", SpyOn_WriteSpiMaster_arg1[1]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        ok, SpyOn_WriteSpiMaster_arg1[0],
        "Expect OK (0x00)."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_red, SpyOn_WriteSpiMaster_arg1[1],
        "Expect led_status is led_red."
        );
}

void SetUp_SetSensorLED(void)
{
    SetUp_Mock();
    Mock_WriteSpiMaster(); spy_writer = SpyOn_WriteSpiMaster_arg1;
}
void TearDown_SetSensorLED(void)
{
    TearDown_Mock();
    Restore_WriteSpiMaster();
}
void SetSensorLED_receives_led_number_and_led_state_from_Bridge(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_led_number = 11;
    QueuePush(SpiFifo, fake_led_number);
    uint8_t const fake_led_state = 22;
    QueuePush(SpiFifo, fake_led_state);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    SetSensorLED();
    // QueueIsEmpty immediately returns false because the test Setup queued a byte.
    /* =====[ Test ]===== */
    // Check that SetSensorLED pops the fake led_number and led_state from the queue.
    // Assert that the queue is now empty.
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
}
void SetSensorLED_replies_with_one_byte(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_led_number = 11;
    QueuePush(SpiFifo, fake_led_number);
    uint8_t const fake_led_state = 22;
    QueuePush(SpiFifo, fake_led_state);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    SetSensorLED();
    // QueueIsEmpty immediately returns false because the test Setup queued a byte.
    /* =====[ Test ]===== */
    // ---WriteSpiMaster sends one byte and is not called again---
    uint8_t call_n = 1; uint8_t arg_n = 2; uint16_t arg_val = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &arg_val));
    TEST_ASSERT_EQUAL_UINT8(call_n, NumberOfActualCalls(mock));
}
void SetSensorLED_replies_msg_status_error_if_led_number_is_not_valid(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_led_number = 11;
    QueuePush(SpiFifo, fake_led_number);
    uint8_t const fake_led_state = 22;
    QueuePush(SpiFifo, fake_led_state);
    // Assert that the Queue has two bytes
    TEST_ASSERT_EQUAL_UINT16(2, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    SetSensorLED();
    // QueueIsEmpty immediately returns false because the test Setup queued a byte.
    /* =====[ Test ]===== */
    // ---WriteSpiMaster sends status "error"---
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t status = error;
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        status, SpyOn_WriteSpiMaster_arg1[0],
        "Expect WriteSpiMaster sends ERROR (0x01)."
        );
}
void SetSensorLED_replies_msg_status_error_if_led_state_is_not_valid(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const good_led_number = led_1;
    QueuePush(SpiFifo, good_led_number);
    uint8_t const fake_led_state = 22;
    QueuePush(SpiFifo, fake_led_state);
    // Assert that the Queue has two bytes
    TEST_ASSERT_EQUAL_UINT16(2, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    SetSensorLED();
    // QueueIsEmpty immediately returns false because the test Setup queued a byte.
    /* =====[ Test ]===== */
    // ---WriteSpiMaster sends status "error"---
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t status = error;
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        status, SpyOn_WriteSpiMaster_arg1[0],
        "Expect WriteSpiMaster sends ERROR (0x01)."
        );
}
void SetSensorLED_replies_msg_status_ok_if_led_number_and_led_state_are_valid(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const good_led_number = led_1;
    QueuePush(SpiFifo, good_led_number);
    uint8_t const good_led_state = led_red;
    QueuePush(SpiFifo, good_led_state);
    // Assert that the Queue has two bytes
    TEST_ASSERT_EQUAL_UINT16(2, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    SetSensorLED();
    // QueueIsEmpty immediately returns false because the test Setup queued a byte.
    /* =====[ Test ]===== */
    // ---WriteSpiMaster sends status "ok"---
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t status = ok;
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        status, SpyOn_WriteSpiMaster_arg1[0],
        "Expect WriteSpiMaster sends OK (0x00)."
        );
}
void SetSensorLED_turns_off_led_if_payload_is_led_off(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const good_led_number = led_1;
    QueuePush(SpiFifo, good_led_number);
    uint8_t const good_led_state = led_off;
    QueuePush(SpiFifo, good_led_state);
    // Assert that the Queue has two bytes
    TEST_ASSERT_EQUAL_UINT16(2, QueueLength(SpiFifo));
    /* =====[ Test case: led is on before command is sent ]===== */
    BiColorLedOn(good_led_number);
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsOn(good_led_number),
        "LED must be on before test begins."
        );
    /* =====[ Operate ]===== */
    SetSensorLED();
    // SetSensorLED expects 2 bytes in the SpiFifo as folllows.
    // SetSensorLED does `while (QueueIsEmpty(SpiFifo));` 2 times.
    // Both times the loop immediately exits because test Setup queued 2 bytes.
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsOn(good_led_number),
        "Expect SetSensorLED turns off LED."
        );
}
void SetSensorLED_turns_led_on_and_green_if_payload_is_led_green(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const good_led_number = led_1;
    QueuePush(SpiFifo, good_led_number);
    uint8_t const good_led_state = led_green;
    QueuePush(SpiFifo, good_led_state);
    // Assert that the Queue has two bytes
    TEST_ASSERT_EQUAL_UINT16(2, QueueLength(SpiFifo));
    /* =====[ Test case: led is off before command is sent ]===== */
    BiColorLedOff(good_led_number);
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsOn(good_led_number),
        "LED must be off before test begins."
        );
    /* =====[ Operate ]===== */
    SetSensorLED();
    // SetSensorLED expects 2 bytes in the SpiFifo as folllows.
    // SetSensorLED does `while (QueueIsEmpty(SpiFifo));` 2 times.
    // Both times the loop immediately exits because test Setup queued 2 bytes.
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsOn(good_led_number),
        "Expect SetSensorLED turns on LED."
        );
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsRed(good_led_number),
        "Expect SetSensorLED turns LED green."
        );
}
void SetSensorLED_turns_led_on_and_red_if_payload_is_led_red(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const good_led_number = led_1;
    QueuePush(SpiFifo, good_led_number);
    uint8_t const good_led_state = led_red;
    QueuePush(SpiFifo, good_led_state);
    // Assert that the Queue has two bytes
    TEST_ASSERT_EQUAL_UINT16(2, QueueLength(SpiFifo));
    /* =====[ Test case: led is off before command is sent ]===== */
    BiColorLedOff(good_led_number);
    TEST_ASSERT_TRUE_MESSAGE(
        !BiColorLedIsOn(good_led_number),
        "LED must be off before test begins."
        );
    /* =====[ Operate ]===== */
    SetSensorLED();
    // SetSensorLED expects 2 bytes in the SpiFifo as folllows.
    // SetSensorLED does `while (QueueIsEmpty(SpiFifo));` 2 times.
    // Both times the loop immediately exits because test Setup queued 2 bytes.
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsOn(good_led_number),
        "Expect SetSensorLED turns on LED."
        );
    TEST_ASSERT_TRUE_MESSAGE(
        BiColorLedIsRed(good_led_number),
        "Expect SetSensorLED turns LED red."
        );
}

void SetUp_GetSensorConfig(void)
{
    SetUp_Mock();
    Mock_WriteSpiMaster(); spy_writer = SpyOn_WriteSpiMaster_arg1;
    // Initialize Photodiode Array Config to match init in vis-spi-out.c main()
    binning = binning_on; // default to 392 pixels
    gain = gain1x; // default to 1x gain
    active_rows = all_rows_active; // default to using all 5 pixel rows
}
void TearDown_GetSensorConfig(void)
{
    TearDown_Mock();
    Restore_WriteSpiMaster();
}
void GetSensorConfig_sends_msg_status_ok_to_Bridge(void)
{
    /* =====[ Operate ]===== */
    GetSensorConfig();
    /* =====[ Test ]===== */
    // Assert: send one status byte to Bridge
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    /* printf("status byte: %#02x\n", SpyOn_WriteSpiMaster_arg1[0]); */
    TEST_ASSERT_EQUAL_HEX8(ok, SpyOn_WriteSpiMaster_arg1[0]);
}
void GetSensorConfig_sends_three_bytes_of_data_to_Bridge_after_sending_ok(void)
{
    /* =====[ Operate ]===== */
    GetSensorConfig();
    /* =====[ Test ]===== */
    // Assert: send three data bytes to Bridge
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t nbytes_sent = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes_sent),
        "Expect WriteSpiMaster sends three data bytes."
        );
    // Assert: data bytes sent are {binning, gain, active_rows}
    arg_n = 1;
    printf("data byte 1: %#02x\n", SpyOn_WriteSpiMaster_arg1[1]);
    TEST_ASSERT_EQUAL_HEX8(binning, SpyOn_WriteSpiMaster_arg1[1]);
    printf("data byte 2: %#02x\n", SpyOn_WriteSpiMaster_arg1[2]);
    TEST_ASSERT_EQUAL_HEX8(gain, SpyOn_WriteSpiMaster_arg1[2]);
    printf("data byte 3: %#02x\n", SpyOn_WriteSpiMaster_arg1[3]);
    TEST_ASSERT_EQUAL_HEX8(active_rows, SpyOn_WriteSpiMaster_arg1[3]);
}

void SetUp_GetExposure(void)
{
    SetUp_Mock();
    Mock_WriteSpiMaster(); spy_writer = SpyOn_WriteSpiMaster_arg1;
    // Initialize exposure time to match init in vis-spi-out.c main()
    exposure_ticks = 50; // multiply by 20us to get integration time in seconds
}
void TearDown_GetExposure(void)
{
    TearDown_Mock();
    Restore_WriteSpiMaster();
}

void GetExposure_sends_status_byte_ok(void)
{
    /* =====[ Operate ]===== */
    GetExposure();
    /* =====[ Test ]===== */
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 1 is WriteSpiMaster."
        );
    uint8_t status = ok;
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        status, SpyOn_WriteSpiMaster_arg1[0],
        "Expect first byte of reply is ok (0x00)."
        );
}
void GetExposure_sends_two_bytes_of_exposure_time_most_significant_byte_first(void)
{
    /* =====[ Operate ]===== */
    GetExposure();
    /* =====[ Test ]===== */
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 2 is WriteSpiMaster."
        );
    uint8_t exposure_msb = exposure_ticks >> 8;
    uint8_t exposure_lsb = exposure_ticks & 0xFF;
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        exposure_msb, SpyOn_WriteSpiMaster_arg1[1],
        "Expect first byte of data is MSByte of exposure time."
        );
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        exposure_lsb, SpyOn_WriteSpiMaster_arg1[2],
        "Expect second byte of data is LSByte of exposure time."
        );
}

void SetUp_SetExposure(void)
{
    SetUp_Mock();
    Mock_WriteSpiMaster(); spy_writer = SpyOn_WriteSpiMaster_arg1;
    // Initialize exposure time to match init in vis-spi-out.c main()
    exposure_ticks = 50; // multiply by 20us to get integration time in seconds
}
void TearDown_SetExposure(void)
{
    TearDown_Mock();
    Restore_WriteSpiMaster();
}
void SetExposure_receives_two_bytes_of_exposure_msb_first_from_Bridge(void)
{
    /* =====[ Setup ]===== */
    exposure_ticks = 66; // multiply by 20us to get integration time in seconds
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_exposure_msb = exposure_ticks >> 8;
    QueuePush(SpiFifo, fake_exposure_msb);
    uint8_t const fake_exposure_lsb = exposure_ticks & 0xFF;
    QueuePush(SpiFifo, fake_exposure_lsb);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    SetExposure();
    /* =====[ Test ]===== */
    // SetExposure pops the two bytes of fake exposure time from the queue.
    // Assert that the queue is now empty.
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
}
void SetExposure_replies_msg_status_ok(void)
{
    /* =====[ Setup ]===== */
    exposure_ticks = 66; // multiply by 20us to get integration time in seconds
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_exposure_msb = exposure_ticks >> 8;
    QueuePush(SpiFifo, fake_exposure_msb);
    uint8_t const fake_exposure_lsb = exposure_ticks & 0xFF;
    QueuePush(SpiFifo, fake_exposure_lsb);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    SetExposure();
    // Assert that the queue is now empty.
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
    /* =====[ Test ]===== */
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 1 is WriteSpiMaster."
        );
    uint8_t status = ok;
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        status, SpyOn_WriteSpiMaster_arg1[0],
        "Expect status byte is ok (0x00)."
        );
}
void SetExposure_converts_two_data_bytes_to_new_16_bit_exposure_ticks_value(void)
{
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(
        50, exposure_ticks,
        "Expect default exposure_ticks value is 50."
        );
    /* =====[ Setup ]===== */
    uint16_t new_exposure_ticks = 66;
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.
    // Fake placing a byte in the queue.
    uint8_t const fake_exposure_msb = new_exposure_ticks >> 8;
    QueuePush(SpiFifo, fake_exposure_msb);
    uint8_t const fake_exposure_lsb = new_exposure_ticks & 0xFF;
    QueuePush(SpiFifo, fake_exposure_lsb);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    SetExposure();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(
            new_exposure_ticks, exposure_ticks,
            "Expect exposure_ticks is written with new value, 66."
            );
}

/* =====[ Mock ProgramPhotodiodeArray() ]===== */
static RecordedCall * Record_ProgramPhotodiodeArray(uint8_t const *arg1)
{ // Define call recorded when func-under-test calls mocked function.
    char const *call_name = "ProgramPhotodiodeArray";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t const **)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
static void ProgramPhotodiodeArray_Mocked(uint8_t const *config)
{ // Define behavior of mocked function: ProgramPhotodiodeArray().
    RecordActualCall(mock, Record_ProgramPhotodiodeArray(config));
}
/* =====[ Define how to swap function definitions ]===== */
static void (*ProgramPhotodiodeArray_Saved)(uint8_t const *);
static void Restore_ProgramPhotodiodeArray(void)
{ // how to restore real definition
    ProgramPhotodiodeArray = ProgramPhotodiodeArray_Saved;
}
static void Mock_ProgramPhotodiodeArray(void)
{ // how to swap real definition with mocked version
    ProgramPhotodiodeArray_Saved = ProgramPhotodiodeArray;
    ProgramPhotodiodeArray = ProgramPhotodiodeArray_Mocked;
}
void SetUp_SetSensorConfig(void)
{
    SetUp_Mock();
    Mock_WriteSpiMaster(); spy_writer = SpyOn_WriteSpiMaster_arg1;
    Mock_ProgramPhotodiodeArray();
    // Initialize Photodiode Array Config to match init in vis-spi-out.c main()
    binning = binning_on; // default to 392 pixels
    gain = gain1x; // default to 1x gain
    active_rows = all_rows_active; // default to using all 5 pixel rows
}
void TearDown_SetSensorConfig(void)
{
    TearDown_Mock();
    Restore_WriteSpiMaster();
    Restore_ProgramPhotodiodeArray();
}
void SetSensorConfig_receives_three_bytes_of_config_from_Bridge(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // SetSensorConfig waits until there is a byte in the queue.
    // Fake placing three bytes in the queue.
    uint8_t const fake_binning = binning_off;
    QueuePush(SpiFifo, fake_binning);
    uint8_t const fake_gain = gain5x;
    QueuePush(SpiFifo, fake_gain);
    uint8_t const fake_active_rows = 0x11; // b00010001
    QueuePush(SpiFifo, fake_active_rows);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    // SetSensorConfig reads from the Queue and writes to the config globals.
    TEST_ASSERT_EQUAL_HEX8(fake_binning, binning);
    TEST_ASSERT_EQUAL_HEX8(fake_gain, gain);
    TEST_ASSERT_EQUAL_HEX8(fake_active_rows, active_rows);
}
void SetSensorConfig_replies_msg_status_error_if_binning_is_invalid(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // SetSensorConfig waits until there is a byte in the queue.
    // Fake placing three bytes in the queue.
    uint8_t const fake_bad_binning = binning_off+100;
    QueuePush(SpiFifo, fake_bad_binning);
    uint8_t const fake_gain = gain5x;
    QueuePush(SpiFifo, fake_gain);
    uint8_t const fake_active_rows = 0x11; // b00010001
    QueuePush(SpiFifo, fake_active_rows);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    // SetSensorConfig reads from the Queue and writes to the config globals.
    TEST_ASSERT_EQUAL_HEX8(fake_bad_binning, binning);
    TEST_ASSERT_EQUAL_HEX8(fake_gain, gain);
    TEST_ASSERT_EQUAL_HEX8(fake_active_rows, active_rows);
    /* =====[ Test ]===== */
    // ---WriteSpiMaster sends status "error"---
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    printf("WriteSpiMaster sends: %#04x\n", SpyOn_WriteSpiMaster_arg1[0]);
    TEST_ASSERT_EQUAL_HEX8(error, SpyOn_WriteSpiMaster_arg1[0]);
}
void SetSensorConfig_replies_msg_status_error_if_gain_is_invalid(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // SetSensorConfig waits until there is a byte in the queue.
    // Fake placing three bytes in the queue.
    uint8_t const fake_binning = binning_off;
    QueuePush(SpiFifo, fake_binning);
    uint8_t const fake_bad_gain = gain5x+100;
    QueuePush(SpiFifo, fake_bad_gain);
    uint8_t const fake_active_rows = 0x11; // b00010001
    QueuePush(SpiFifo, fake_active_rows);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    // SetSensorConfig reads from the Queue and writes to the config globals.
    TEST_ASSERT_EQUAL_HEX8(fake_binning, binning);
    TEST_ASSERT_EQUAL_HEX8(fake_bad_gain, gain);
    TEST_ASSERT_EQUAL_HEX8(fake_active_rows, active_rows);
    /* =====[ Test ]===== */
    // ---WriteSpiMaster sends status "error"---
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    printf("WriteSpiMaster sends: %#04x\n", SpyOn_WriteSpiMaster_arg1[0]);
    TEST_ASSERT_EQUAL_HEX8(error, SpyOn_WriteSpiMaster_arg1[0]);
}
void SetSensorConfig_replies_msg_status_error_if_active_rows_is_invalid(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // SetSensorConfig waits until there is a byte in the queue.
    // Fake placing three bytes in the queue.
    uint8_t const fake_binning = binning_off;
    QueuePush(SpiFifo, fake_binning);
    uint8_t const fake_gain = gain5x;
    QueuePush(SpiFifo, fake_gain);
    uint8_t const fake_bad_active_rows = 0xE1; // b11110001
    QueuePush(SpiFifo, fake_bad_active_rows);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    // SetSensorConfig reads from the Queue and writes to the config globals.
    TEST_ASSERT_EQUAL_HEX8(fake_binning, binning);
    TEST_ASSERT_EQUAL_HEX8(fake_gain, gain);
    TEST_ASSERT_EQUAL_HEX8(fake_bad_active_rows, active_rows);
    /* =====[ Test ]===== */
    // ---WriteSpiMaster sends status "error"---
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    printf("WriteSpiMaster sends: %#04x\n", SpyOn_WriteSpiMaster_arg1[0]);
    TEST_ASSERT_EQUAL_HEX8(error, SpyOn_WriteSpiMaster_arg1[0]);
}
void SetSensorConfig_replies_msg_status_ok_if_all_config_bytes_are_valid(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // SetSensorConfig waits until there is a byte in the queue.
    // Fake placing three VALID bytes in the queue.
    uint8_t const fake_binning = binning_off;
    QueuePush(SpiFifo, fake_binning);
    uint8_t const fake_gain = gain5x;
    QueuePush(SpiFifo, fake_gain);
    uint8_t const fake_active_rows = 0x11; // b00010001
    QueuePush(SpiFifo, fake_active_rows);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    // SetSensorConfig reads from the Queue and writes to the config globals.
    TEST_ASSERT_EQUAL_HEX8(fake_binning, binning);
    TEST_ASSERT_EQUAL_HEX8(fake_gain, gain);
    TEST_ASSERT_EQUAL_HEX8(fake_active_rows, active_rows);
    /* =====[ Test ]===== */
    // Assert response is OK if all three bytes are valid.
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    printf("WriteSpiMaster sends: 0x%02x\n", SpyOn_WriteSpiMaster_arg1[0]);
    TEST_ASSERT_EQUAL_HEX8(ok, SpyOn_WriteSpiMaster_arg1[0]);
}
void SetSensorConfig_programs_the_photodiode_array_with_the_config(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // SetSensorConfig waits until there is a byte in the queue.
    // Fake placing three VALID bytes in the queue.
    uint8_t const fake_binning = binning_on;
    QueuePush(SpiFifo, fake_binning);
    uint8_t const fake_gain = gain1x;
    QueuePush(SpiFifo, fake_gain);
    uint8_t fake_active_rows = 0;
    uint8_t const row1 = 0; fake_active_rows |= 1<<row1;
    uint8_t const row2 = 1; fake_active_rows |= 1<<row2;
    uint8_t const row3 = 2; fake_active_rows |= 1<<row3;
    uint8_t const row4 = 3; fake_active_rows |= 1<<row4;
    uint8_t const row5 = 4; fake_active_rows |= 1<<row5;
    QueuePush(SpiFifo, fake_active_rows);
    // Calculate expected config
    uint8_t expected_config[4];
    RepresentConfigAs4bytes(expected_config, fake_binning, fake_gain, fake_active_rows);
    // Assert config now contains the following LIS programming sequence.
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xF9, expected_config[0], "Expected config[0]");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, expected_config[1], "Expected config[1]");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0xFF, expected_config[2], "Expected config[2]");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x0F, expected_config[3], "Expected config[3]");
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    // SetSensorConfig reads from the Queue and writes to the config globals.
    TEST_ASSERT_EQUAL_HEX8(fake_binning, binning);
    TEST_ASSERT_EQUAL_HEX8(fake_gain, gain);
    TEST_ASSERT_EQUAL_HEX8(fake_active_rows, active_rows);
    // ProgramPhotodiodeArray takes the address of the four-byte config.
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "ProgramPhotodiodeArray"));
}

// tdd RepresentConfigAs4bytes
void RepresentConfigAs4bytes_writes_config_byte0_bit0_set_if_binning_is_on(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(0,config[0]);
}
void RepresentConfigAs4bytes_writes_config_byte0_bit0_clear_if_binning_is_off(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(0,config[0]);
}
void RepresentConfigAs4bytes_writes_config_byte0_bits1to2_clear_if_gain_is_1x(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(1,config[0]);
    TEST_ASSERT_BIT_LOW(2,config[0]);
}
void RepresentConfigAs4bytes_writes_config_byte0_bit1_clear_bit2_set_if_gain_is_2p5x(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain25x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(1,config[0]);
    TEST_ASSERT_BIT_HIGH(2,config[0]);
}
void RepresentConfigAs4bytes_writes_config_byte0_bit1_set_bit2_clear_if_gain_is_4x(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain4x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(1,config[0]);
    TEST_ASSERT_BIT_LOW(2,config[0]);
}
void RepresentConfigAs4bytes_writes_config_byte0_bits1to2_set_if_gain_is_5x(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain5x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(1,config[0]);
    TEST_ASSERT_BIT_HIGH(2,config[0]);
}
void RepresentConfigAs4bytes_writes_config_as_little_endian_so_prog_bit27_is_byte_3_bit3_not_byte0_bit_3(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        4, config[3],
        "Value of byte 3 bits 4 to 7 do not matter. "
        "Expected bit 4 of byte 3 to be low because RepresentConfigAs4bytes "
        "starts with {0x00, 0x00, 0x00,.0x00} and only affects the first 28 "
        "bits starting from byte 0."
        );
}
void RepresentConfigAs4bytes_writes_config_bits3to27_set_if_all_rows_are_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    /* printf("config byte 0: %#x\n", config[0]); */
    // last five bits of byte 0
    TEST_ASSERT_BIT_HIGH(3,config[0]);
    TEST_ASSERT_BIT_HIGH(4,config[0]);
    TEST_ASSERT_BIT_HIGH(5,config[0]);
    TEST_ASSERT_BIT_HIGH(6,config[0]);
    TEST_ASSERT_BIT_HIGH(7,config[0]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, config[1]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, config[2]);

    // byte 3
    TEST_ASSERT_BIT_HIGH(0,config[3]);
    TEST_ASSERT_BIT_HIGH(1,config[3]);
    TEST_ASSERT_BIT_HIGH(2,config[3]);
    TEST_ASSERT_BIT_HIGH(3,config[3]);
    // Remaining bits in byte 3 are not used, so there is nothing else to test.
}

void RepresentConfigAs4bytes_writes_config_b3b8b13b18b23_set_if_row1_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row1 = 0;
    uint8_t active_rows = (1<<row1);
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(3,config[0]);
    TEST_ASSERT_BIT_LOW(4,config[0]);
    TEST_ASSERT_BIT_LOW(5,config[0]);
    TEST_ASSERT_BIT_LOW(6,config[0]);
    TEST_ASSERT_BIT_LOW(7,config[0]);
    TEST_ASSERT_EQUAL_HEX8(0x21, config[1]);
    TEST_ASSERT_EQUAL_HEX8(0x84, config[2]);

    // byte 3
    TEST_ASSERT_EQUAL_HEX8(0x00, config[3]);
    /* TEST_ASSERT_BIT_HIGH(0,config[3]); */
    /* TEST_ASSERT_BIT_HIGH(1,config[3]); */
    /* TEST_ASSERT_BIT_HIGH(2,config[3]); */
    /* TEST_ASSERT_BIT_HIGH(3,config[3]); */
    // Remaining bits in byte 3 are not used, so there is nothing else to test.
}
void RepresentConfigAs4bytes_writes_config_b4b9b14b19b24_set_if_row2_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row2 = 1;
    uint8_t active_rows = (1<<row2);
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(3,config[0]);
    TEST_ASSERT_BIT_HIGH(4,config[0]);
    TEST_ASSERT_BIT_LOW(5,config[0]);
    TEST_ASSERT_BIT_LOW(6,config[0]);
    TEST_ASSERT_BIT_LOW(7,config[0]);
    TEST_ASSERT_EQUAL_HEX8(0x42, config[1]);
    TEST_ASSERT_EQUAL_HEX8(0x08, config[2]);
    TEST_ASSERT_EQUAL_HEX8(0x01, config[3]);
}
void RepresentConfigAs4bytes_writes_config_b5b10b15b20b25_set_if_row3_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row3 = 2;
    uint8_t active_rows = (1<<row3);
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(3,config[0]);
    TEST_ASSERT_BIT_LOW(4,config[0]);
    TEST_ASSERT_BIT_HIGH(5,config[0]);
    TEST_ASSERT_BIT_LOW(6,config[0]);
    TEST_ASSERT_BIT_LOW(7,config[0]);
    TEST_ASSERT_EQUAL_HEX8(0x84, config[1]);
    TEST_ASSERT_EQUAL_HEX8(0x10, config[2]);
    TEST_ASSERT_EQUAL_HEX8(0x02, config[3]);
}
void RepresentConfigAs4bytes_writes_config_b6b11b16b21b26_set_if_row4_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row4 = 3;
    uint8_t active_rows = (1<<row4);
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(3,config[0]);
    TEST_ASSERT_BIT_LOW(4,config[0]);
    TEST_ASSERT_BIT_LOW(5,config[0]);
    TEST_ASSERT_BIT_HIGH(6,config[0]);
    TEST_ASSERT_BIT_LOW(7,config[0]);
    TEST_ASSERT_EQUAL_HEX8(0x08, config[1]);
    TEST_ASSERT_EQUAL_HEX8(0x21, config[2]);
    TEST_ASSERT_EQUAL_HEX8(0x04, config[3]);
}
void RepresentConfigAs4bytes_writes_config_b7b12b17b22b27_set_if_row5_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row5 = 4;
    uint8_t active_rows = (1<<row5);
    /* =====[ Operate ]===== */
    uint8_t config[4];
    RepresentConfigAs4bytes(config, binning, gain, active_rows);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(3,config[0]);
    TEST_ASSERT_BIT_LOW(4,config[0]);
    TEST_ASSERT_BIT_LOW(5,config[0]);
    TEST_ASSERT_BIT_LOW(6,config[0]);
    TEST_ASSERT_BIT_HIGH(7,config[0]);
    TEST_ASSERT_EQUAL_HEX8(0x10, config[1]);
    TEST_ASSERT_EQUAL_HEX8(0x42, config[2]);
    TEST_ASSERT_EQUAL_HEX8(0x08, config[3]);
}

// tdd LIS programming sequence
void bit_number_is_correct_on_each_iteration_of_while_loop(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    (void) binning; (void) gain; (void) active_rows;
    /* uint32_t config = RepresentConfigAs28bits(binning, gain, active_rows); */
    uint32_t config = 0;
    /* =====[ Operate and Test ]===== */
    uint8_t bit=0;
    while (bit < 28)
    {
        if (config & (1<<bit))
        {
            /* SetBit(Lis_port1, Lis_Rst); */
            printf("set bit %d\n", bit);
        }
        else
        {
            /* ClearBit(Lis_port1, Lis_Rst); */
            printf("clear bit %d\n", bit);
        }
        bit++;
        // Wait for Lis_Rst value to clock in before loading the next bit.
        /* LisWaitForClkRiseEdge(); // bit is read on rising edge */
        /* LisWaitForClkFallEdge(); // hold bit until falling edge */
    }
    TEST_FAIL_MESSAGE("Implement test.");
}

// tdd NumPixelsInFrame
void npixels_is_a_macro_constant_equal_to_784(void)
{
    // macro `npixels` is defined in SensorVis.h
    // this test file includes SensorVis.h
    // the application includes SensorVis.h
    TEST_ASSERT_EQUAL_UINT16(784,npixels);
}
void NumPixelsInFrame_returns_784_if_binning_is_off(void)
{
    /* =====[ Setup ]===== */
    binning = binning_off;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(npixels, NumPixelsInFrame());
}
void NumPixelsInFrame_returns_392_if_binning_is_on(void)
{
    /* =====[ Setup ]===== */
    binning = binning_on;
    // Assert my math is correct:
    TEST_ASSERT_EQUAL_UINT16(392, npixels>>1);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_UINT16((npixels>>1), NumPixelsInFrame());
}

// tdd WordToTwoByteArray
void WordToTwoByteArray_stores_16bit_word_msb_first_in_input_array(void)
{
    /* =====[ Setup ]===== */
    uint8_t array[2]; uint16_t word = 0xabcd;
    /* =====[ Operate ]===== */
    WordToTwoByteArray(word, array);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(0xab, array[0]);
    TEST_ASSERT_EQUAL_HEX8(0xcd, array[1]);
}

// tdd GetFrame: hmm... cannot stub without adding function call overhead
void GetFrame_exposes_the_photodiode_array(void)
{
    // If I make a function pointer to stub LisWaitForReadoutToStart(), it adds
    // function call overhead like this:
    //
    // lds	r30, 0x0110	; 0x800110 <LisWaitForReadoutToStart>
    // lds	r31, 0x0111	; 0x800111 <LisWaitForReadoutToStart+0x1>
    // icall
    //
    // The function itself is still optimized. Here is the function the call
    // jumps to:
    //
    // breq	.-16     	; 0x4da <LisWaitForReadoutToStart_Implementation>
    // sbic	0x09, 7	; 9
    // rjmp	.-4      	; 0x4ea <LisWaitForReadoutToStart_Implementation+0x10>
    // ret
    //
    // Is it possible to test this without making a function pointer?
    //
    /* =====[ Operate ]===== */
    /* GetFrame(); */
    // need to stub LisWaitForReadoutToStart, otherwise it hangs waiting for SYNC
    // high followed by SYNC low
    /* =====[ Test ]===== */
    TEST_FAIL_MESSAGE("Implement test.");
}

// tdd LisReadout
void LisReadout_reads_npixels(void)
{
    /* =====[ Setup ]===== */
    binning = binning_on;
    uint16_t npixels_in_frame = NumPixelsInFrame();
    printf("npixels_in_frame: %d\n", npixels_in_frame);
    /* =====[ Operate ]===== */
    LisReadout(); // just calling this to check plumbing
    // copy and paste body of function below
    uint16_t pixel_count = 0; // track number of pixels read
    uint16_t pixels_read;
    while (pixel_count++ < npixels_in_frame) pixels_read = pixel_count;
    // printf("pixels_read: %d\n",pixels_read);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(npixels_in_frame, pixels_read);
    // All the rest of the code is optimized.
    // The only questionable piece is sending two bytes then reading two bytes.
    // [ ] Try reading one byte after sending one byte.
}

void SetUp_CaptureFrame(void)
{
    SetUp_Mock();
    Mock_WriteSpiMaster(); spy_writer = SpyOn_WriteSpiMaster_arg1;
    // Initialize Photodiode Array Config to match init in vis-spi-out.c main()
    binning = binning_on; // default to 392 pixels
    Mock_GetFrame();
}
void TearDown_CaptureFrame(void)
{
    TearDown_Mock();
    Restore_WriteSpiMaster();
    Restore_GetFrame();
}
void CaptureFrame_sends_status_byte_ok(void)
{
    /* =====[ Operate ]===== */
    CaptureFrame();
    /* =====[ Test ]===== */
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 1 is WriteSpiMaster."
        );
    uint8_t status = ok;
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        status, SpyOn_WriteSpiMaster_arg1[0],
        "Expect first byte of reply is ok (0x00)."
        );
}
void CaptureFrame_collects_a_frame_of_pixel_data(void)
{
    /* =====[ Operate ]===== */
    CaptureFrame();
    /* =====[ Test ]===== */
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "GetFrame"),
        "Expect call 2 is GetFrame."
        );
}
void CaptureFrame_sends_two_bytes_msb_first_with_number_of_pixels_in_frame(void)
{
    /* =====[ Setup ]===== */
    // Set binning to define number of pixels in frame
    binning = binning_on; // default to 392 pixels
    /* binning = binning_off; // this test also works for 784 pixels */
    // Calculate msb and lsb of npixels_in_frame
    uint8_t msb = NumPixelsInFrame() >> 8;
    uint8_t lsb = NumPixelsInFrame() & 0xFF;
    /* =====[ Operate ]===== */
    CaptureFrame();
    /* =====[ Test ]===== */
    // Assert send (call to WriteSpiMaster) two bytes (arg2 is value 2)
    uint8_t call_n = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 3 is WriteSpiMaster."
        );
    printf(
        "Number of pixels is %d = %#04x.\n",
        NumPixelsInFrame(), NumPixelsInFrame()
        );
    uint8_t arg_n = 2; uint16_t nbytes_sent = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes_sent),
        "Expect 2 bytes are sent."
        );
    // Assert first byte is MSB of npixels_in_frame, second byte is LSB
    printf("Expected msb is %#04x\n", msb);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        msb, SpyOn_WriteSpiMaster_arg1[1],
        "Expect first byte is most significant."
        );
    printf("Expected lsb is %#04x\n", lsb);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        lsb, SpyOn_WriteSpiMaster_arg1[2],
        "Expect second byte is least significant."
        );
}
void CaptureFrame_sends_another_status_byte_ok(void)
{
    /* =====[ Operate ]===== */
    CaptureFrame();
    /* =====[ Test ]===== */
    uint8_t call_n = 4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 4 is WriteSpiMaster."
        );
    uint8_t status = ok;
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(
        status, SpyOn_WriteSpiMaster_arg1[3],
        "Expect fourth byte of reply is ok (0x00)."
        );
}
void CaptureFrame_sends_the_frame_as_two_bytes_per_pixel_msb_first(void)
{
    /* =====[ Setup ]===== */
    binning = binning_on; // default to 392 pixels
    TEST_ASSERT_EQUAL_UINT16(392, NumPixelsInFrame());
    uint8_t *pframe = frame;
    *pframe++ = 0xab;
    *pframe++ = 0xcd;
    // Check that frame is fake-loaded with data msb first.
    pframe = frame;
    TEST_ASSERT_EQUAL_UINT8(0xab, *pframe++);
    TEST_ASSERT_EQUAL_UINT8(0xcd, *pframe++);
    /* =====[ Operate ]===== */
    CaptureFrame();
    /* =====[ Test ]===== */
    uint8_t call_n = 5;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 5 is WriteSpiMaster."
        );
    uint16_t nbytes_sent = 2*NumPixelsInFrame();
    printf(
        "Number of pixels is %d = %#04x. Number of bytes in frame is %d = %#04x.\n",
        NumPixelsInFrame(), NumPixelsInFrame(), nbytes_sent, nbytes_sent
        );
    uint8_t arg_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes_sent),
        "Expect number of bytes sent is 2 x number of pixels."
        );
    // Check that bytes are sent msb first.
    TEST_ASSERT_EQUAL_UINT8(0xab, SpyOn_WriteSpiMaster_arg1[4]);
    TEST_ASSERT_EQUAL_UINT8(0xcd, SpyOn_WriteSpiMaster_arg1[5]);
}

