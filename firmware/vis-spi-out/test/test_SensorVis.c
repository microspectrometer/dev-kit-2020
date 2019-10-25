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
#define max_sizeof_write_buffer 1596
uint8_t SpyOn_WriteSpiMaster_arg1[max_sizeof_write_buffer];
static uint16_t WriteSpiMaster_Mocked(uint8_t const *write_buffer, uint16_t nbytes)
{ // Define behavior of mocked function: WriteSpiMaster().
    RecordActualCall(mock, Record_WriteSpiMaster(write_buffer, nbytes));
    /* Spy on values passed to write_buffer by storing them in a global */
    uint8_t *spy_writer = SpyOn_WriteSpiMaster_arg1;
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
    Mock_WriteSpiMaster();
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
void GetSensorLED_always_replies_with_two_bytes(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // GetSensorLED waits until there is a byte in the queue.

    /* =====[ First test for an invalid LED number ]===== */

    // Fake placing a byte in the queue.
    uint8_t const fake_bad_led_number = 99;
    QueuePush(SpiFifo, fake_bad_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));

    /* =====[ Operate ]===== */
    GetSensorLED();

    /* =====[ Test ]===== */
    printf("If LED number is not valid:\n");
    // Assert reply with two bytes:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t assert_val = 2;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &assert_val));
    // Note, the arg must match the type as well the value!

    /* =====[ Repeat test for a valid LED number. ]===== */

    // Fake placing a byte in the queue.
    uint8_t const fake_good_led_number = 1;
    QueuePush(SpiFifo, fake_good_led_number);
    // Assert that the Queue is not empty.
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));

    /* =====[ Operate ]===== */
    GetSensorLED();

    /* =====[ Test ]===== */
    printf("If LED number is valid:\n");
    // Assert reply with two bytes:
    call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    arg_n = 2; assert_val = 2;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &assert_val));
    // Note, the arg must match the type as well the value!

    // For next test: Assert byte 0 is error
    /* TEST_ASSERT_EQUAL_UINT8(error, */ 
}
void GetSensorLED_replies_msg_status_error_if_led_is_non_existent(void)
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

    // Assert reply with two bytes:
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    uint8_t arg_n = 2; uint16_t assert_val = 2;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &assert_val));

    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    /* printf("WriteSpiMaster called with write_buffer[1] == %d\n", SpyOn_WriteSpiMaster_arg1[1]); */
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x01, SpyOn_WriteSpiMaster_arg1[0],
        "Expect status ERROR (0x01)."
        );
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
    /* printf("WriteSpiMaster called with write_buffer[1] == %d\n", SpyOn_WriteSpiMaster_arg1[1]); */
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x00, SpyOn_WriteSpiMaster_arg1[0],
        "Expect OK (0x00)."
        );
    /* TEST_ASSERT_EQUAL_UINT8_MESSAGE( */
    /*     led_red, SpyOn_WriteSpiMaster_arg1[1], */
    /*     "Expect led_status is led_red." */
    /*     ); */
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



