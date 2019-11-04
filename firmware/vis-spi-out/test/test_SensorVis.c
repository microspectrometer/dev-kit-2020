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
void GetSensorConfig_sends_three_bytes_of_data_to_Bridge_after_sending_ok(void)
{
    /* =====[ Operate ]===== */
    GetSensorConfig();
    /* =====[ Test ]===== */
    // Assert: send 4 bytes to Bridge
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t nbytes_sent = 4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &nbytes_sent),
        "Expect WriteSpiMaster sends three data bytes."
        );
    // Assert: data bytes sent are {binning, gain, active_rows}
    arg_n = 1;
    printf("status byte: %#02x\n", SpyOn_WriteSpiMaster_arg1[0]);
    TEST_ASSERT_EQUAL_HEX8(ok, SpyOn_WriteSpiMaster_arg1[0]);
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

/* =====[ Mock ProgramPhotodiodeArray() ]===== */
static RecordedCall * Record_ProgramPhotodiodeArray(uint32_t arg1)
{ // Define call recorded when func-under-test calls mocked function.
    char const *call_name = "ProgramPhotodiodeArray";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint32_t);
    *((uint32_t *)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
static void ProgramPhotodiodeArray_Mocked(uint32_t config)
{ // Define behavior of mocked function: ProgramPhotodiodeArray().
    RecordActualCall(mock, Record_ProgramPhotodiodeArray(config));
}
/* =====[ Define how to swap function definitions ]===== */
static void (*ProgramPhotodiodeArray_Saved)(uint32_t);
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
void SetSensorConfig_converts_three_data_bytes_to_a_28_bit_config(void)
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
    uint8_t const row1 = 0; uint32_t const row1_mask = 0x00842108;
    uint8_t const row2 = 1; uint32_t const row2_mask = 0x01084210;
    uint8_t const row3 = 2; uint32_t const row3_mask = 0x02108420;
    uint8_t const row4 = 3; uint32_t const row4_mask = 0x04210840;
    uint8_t const row5 = 4; uint32_t const row5_mask = 0x08421080;
    uint8_t fake_active_rows = 0;
    fake_active_rows |= 1<<row1;
    QueuePush(SpiFifo, fake_active_rows);
    // Caclulate expected config
    uint32_t expected_config = 0x00000000;
    uint8_t bit = 0;
    if (binning_on == fake_binning) expected_config |= 1<<(bit++); // bit 0: bin on/off
    if      (gain25x == fake_gain) { bit++; expected_config |= 1<<(bit++); }
    else if (gain4x == fake_gain)  { expected_config |= 1<<(bit++); bit++; }
    else if (gain5x == fake_gain)  { expected_config |= 1<<(bit++); expected_config |= 1<<(bit++); }
    else { bit++; bit++; }
    if (fake_active_rows&(1<<row1)) expected_config |= row1_mask;
    if (fake_active_rows&(1<<row2)) expected_config |= row2_mask;
    if (fake_active_rows&(1<<row3)) expected_config |= row3_mask;
    if (fake_active_rows&(1<<row4)) expected_config |= row4_mask;
    if (fake_active_rows&(1<<row5)) expected_config |= row5_mask;
    /* =====[ Operate ]===== */
    SetSensorConfig();
    // SetSensorConfig reads from the Queue and writes to the config globals.
    TEST_ASSERT_EQUAL_HEX8(fake_binning, binning);
    TEST_ASSERT_EQUAL_HEX8(fake_gain, gain);
    TEST_ASSERT_EQUAL_HEX8(fake_active_rows, active_rows);
    /* =====[ Test ]===== */
    // Assert response is OK if all three bytes are valid.
    uint8_t call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "ProgramPhotodiodeArray"));
    printf("Expected config: %08x\n", expected_config);
    uint8_t arg_n = 1;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &expected_config));
}
