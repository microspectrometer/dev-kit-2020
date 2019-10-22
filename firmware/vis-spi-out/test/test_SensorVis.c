#include "test_SensorVis.h"
#include "SensorVis.h"
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

/* =====[ Develop a Queue for teh SPI FIFO Rx Buffer ]===== */
void test_struct_syntax(void)
{
    /* =====[ Setup ]===== */
    printf(
        "Array for the data in the SPI FIFO Queue is allocated in translation unit SensorVis:\n"
        "```c\n"
        "#include \"SensorVis.h\"\n"
        "```\n"
        "Create a SpiFifo without a typedef (don't do it this way):\n"
        "```c\n"
        "struct Queue_s Queue; // declare a Queue\n"
        "struct Queue_s *SpiFifo = &Queue; // declare a pointer to the Queue\n"
        "```\n"
        "Create a SpiFifo with a typedef (do it this way):\n"
        "```c\n"
        "Queue_s Queue;\n"
        "Queue_s * SpiFifo = &Queue;\n"
        "```\n"
        "Initialize the Queue by pointing head and tail at the first byte:\n"
        "```c\n"
        "SpiFifo->head = spi_rx_queue;\n"
        "SpiFifo->tail = spi_rx_queue;\n"
        "```\n"
        );
    // I can't run these anymore because I hid the definition in SensorVis.c
    /* printf( */
    /*     "`SpiFifo` stores up to *%d* bytes of incoming SPI data.\n", */
    /*     MaxLengthOfSpiRxQueue */
    /*     ); */
    /* printf( */
    /*     "The `SpiFifo` is a pointer to a `Queue_s` struct. " */
    /*     "The pointer consumes *%zu* bytes of memory.\n", */
    /*     sizeof(SpiFifo) */
    /*     ); */
    /* printf( */
    /*     "The struct `SpiFifo` points to consumes *%zu* bytes of memory.\n", */
    /*     sizeof(*SpiFifo) */
    /*     ); */
    /* *(SpiFifo->head) = 0xAB; */
    /* uint8_t SpiData = *(SpiFifo->tail); */
    /* printf( */
    /*     "Write SPI data to the Queue head:\n" */
    /*     "*(SpiFifo->head) = 0xAB;\n" */
    /*     "Read SPI data from the Queue tail:\n" */
    /*     "uint8_t SpiData = *(SpiFifo->tail); // SpiData: %d\n", */
    /*     SpiData */
    /*     ); */
}
void test_QueueInit_sets_buffer_length(void)
{
    /* =====[ Setup ]===== */
    uint16_t const max_length = 3;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    /* =====[ Operate ]===== */
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    /* =====[ Test ]===== */
    // Fill the Queue. When it is full, check it's length.
    uint16_t bytes_pushed = 0;
    while(bytes_pushed < max_length)
    {
        QueuePush(SpiFifo, 0);
        bytes_pushed++;
    }
    // Check correct test setup: assert bytes_pushed == max_length
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(
        max_length, bytes_pushed,
        "Failed to set this test up correctly. "
        "Must push exact number of bytes to fill the buffer."
        );
    // Is length of Queue == size of buffer passed in Init?
    TEST_ASSERT_EQUAL_UINT16(max_length, QueueLength(SpiFifo));
}
void test_QueueLength_is_0_after_QueueInit(void)
{
    /* =====[ Operate ]===== */
    QueueInit(SpiFifo, spi_rx_buffer, MaxLengthOfSpiRxQueue);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
}
void test_QueueLength_increments_after_a_push(void)
{
    /* =====[ Operate ]===== */
    QueueInit(SpiFifo, spi_rx_buffer, MaxLengthOfSpiRxQueue);
    QueuePush(SpiFifo, 0xAB);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(1, QueueLength(SpiFifo));
}
void test_QueuePush_writes_to_byte_pointed_to_by_head(void)
{
    /* =====[ Setup ]===== */
    QueueInit(SpiFifo, spi_rx_buffer, MaxLengthOfSpiRxQueue);
    /* =====[ Operate ]===== */
    QueuePush(SpiFifo, 0xAB);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0xAB, spi_rx_buffer[0]);
}
void test_QueuePush_increments_head(void)
{
    /* =====[ Setup ]===== */
    QueueInit(SpiFifo, spi_rx_buffer, MaxLengthOfSpiRxQueue);
    /* =====[ Operate ]===== */
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    TEST_ASSERT_EQUAL_UINT8(0xAB, spi_rx_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(0xCD, spi_rx_buffer[1]);
}
void test_QueueIsFull_returns_true_if_Queue_is_full(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue to give test control over max queue length
    uint16_t const max_length = 100;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the Queue
    uint16_t bytes_pushed = 0;
    while(bytes_pushed < max_length)
    {
        QueuePush(SpiFifo, 0);
        bytes_pushed++;
    }
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(max_length, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
}
void test_QueueIsFull_returns_false_if_Queue_is_not_full(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue to give test control over max queue length
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Do not fill the queue
    QueuePush(SpiFifo, 0xAB);
    // Assert the Length is less than the max value.
    TEST_ASSERT_EQUAL_UINT16(max_length-1, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(QueueIsFull(SpiFifo));
}
void test_QueueIsEmpty_returns_true_if_Queue_is_empty(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue to give test control over max queue length
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Assert the Queue Length is 0 (nothing pushed onto Queue yet)
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
}
void test_QueueIsEmpty_returns_false_if_Queue_is_not_empty(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue to give test control over max queue length
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    QueuePush(SpiFifo, 0xAB);
    // Assert the Queue Length is 1
    TEST_ASSERT_EQUAL_UINT16(1, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
}
void test_QueuePush_does_nothing_if_Queue_is_full(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue to give test control over max queue length
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the queue
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(max_length, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    // Try another push.
    QueuePush(SpiFifo, 0xEF);
    /* =====[ Test ]===== */
    // Assert no values were overwritten.
    TEST_ASSERT_EQUAL_UINT8(0xAB, fake_spi_rx_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(0xCD, fake_spi_rx_buffer[1]);
}
void test_QueueLength_does_not_increase_beyond_max_length(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue to give test control over max queue length
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the queue
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(max_length, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    // Try another push.
    QueuePush(SpiFifo, 0xEF);
    /* =====[ Test ]===== */
    // Assert the Length is still at its max value.
    TEST_ASSERT_EQUAL_UINT16(max_length, QueueLength(SpiFifo));
}
void test_QueueLength_decrements_after_a_pop(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the queue
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    // Assert queue length is 2
    TEST_ASSERT_EQUAL_UINT16(2, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    QueuePop(SpiFifo);
    // Assert queue length decrements after pop
    TEST_ASSERT_EQUAL_UINT16(1, QueueLength(SpiFifo));
}
void test_QueuePop_reads_byte_pointed_to_by_tail(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the queue
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    /* =====[ Operate ]===== */
    uint8_t byte_0 = QueuePop(SpiFifo);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0xAB, byte_0);
}
void test_QueuePop_increments_tail(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the queue
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    /* =====[ Operate ]===== */
    uint8_t byte_0 = QueuePop(SpiFifo);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0xAB, byte_0);
    /* =====[ Operate ]===== */
    uint8_t byte_1 = QueuePop(SpiFifo);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0xCD, byte_1);
}
void test_QueueLength_does_not_decrease_below_zero(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the queue
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    // Assert Queue length is 2
    TEST_ASSERT_EQUAL_UINT16(2, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    QueuePop(SpiFifo);
    QueuePop(SpiFifo);
    // Assert Queue length is 0
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
    QueuePop(SpiFifo);
    /* =====[ Test ]===== */
    // Assert Queue length is still 0
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
}
void test_QueuePop_returns_0_if_Queue_is_empty(void)
{
    /* =====[ Setup ]===== */
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Put fake old garbage non-zero values in the fake buffer
    fake_spi_rx_buffer[0] = 0xAB;
    fake_spi_rx_buffer[1] = 0xCD;
    TEST_ASSERT_EQUAL_UINT8(0xAB, fake_spi_rx_buffer[0]);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0, QueuePop(SpiFifo));
}
void test_QueuePop_does_not_increment_tail_if_Queue_is_empty(void)
{
    /* =====[ Setup ]===== */
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Put fake old garbage non-zero values in the fake buffer
    fake_spi_rx_buffer[0] = 0xAB;
    fake_spi_rx_buffer[1] = 0xCD;
    TEST_ASSERT_EQUAL_UINT8(0, QueuePop(SpiFifo));
    // Test is to check that this pop did not move tail.
    // Push one byte into the Queue.
    QueuePush(SpiFifo, 0x01);
    // Head is at byte 1.
    // But tail should still be at byte 0.
    /* =====[ Operate and Test ]===== */
    // If the value popped is the one just pushed, tail did not move.
    TEST_ASSERT_EQUAL_UINT8(0x01, QueuePop(SpiFifo));
    // Now tail should be at byte 1, but Queue is empty, so Pop should return 0.
    TEST_ASSERT_EQUAL_UINT8(0, QueuePop(SpiFifo));
}
void test_QueuePush_wraps_head_back_to_buffer_index_0(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue to give test control over max queue length
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the queue
    QueuePush(SpiFifo, 11);
    QueuePush(SpiFifo, 22);
    // Assert the Queue is full
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
    // Assert the head is pointing at the last byte index in the Queue.
    TEST_ASSERT_EQUAL_UINT16(max_length, QueueLength(SpiFifo));
    // Empty the queue
    QueuePop(SpiFifo);
    QueuePop(SpiFifo);
    // Assert the Queue is empty.
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    // Put more data on the Queue. `head` should wrap around.
    QueuePush(SpiFifo, 33);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_MESSAGE(
        33, fake_spi_rx_buffer[0],
        "Failed to overwrite byte 0! "
        "Head should wrap around when it reaches the end of the array."
        );
}
void test_QueuePop_wraps_tail_back_to_buffer_index_0(void)
{
    /* =====[ Setup ]===== */
    // Create a fake Spi Rx Queue to give test control over max queue length
    uint16_t const max_length = 2;
    volatile uint8_t fake_spi_rx_buffer[max_length+1];
    QueueInit(SpiFifo, fake_spi_rx_buffer, max_length);
    // Fill the queue
    QueuePush(SpiFifo, 11);
    QueuePush(SpiFifo, 22);
    // Load next byte of buffer with value Pop should never see
    fake_spi_rx_buffer[2] = 99;
    // Assert the Queue is full
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
    // Empty the queue
    QueuePop(SpiFifo);
    QueuePop(SpiFifo);
    // Assert the Queue is empty.
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
    // Put more data on the Queue. `head` should wrap around.
    QueuePush(SpiFifo, 33);
    // Check test is setup: assert byte 0 is 33.
    TEST_ASSERT_EQUAL_MESSAGE(
        33, fake_spi_rx_buffer[0],
        "Failed to overwrite byte 0! "
        "Head should wrap around when it reaches the end of the array."
        );
    /* =====[ Operate and Test ]===== */
    // Pop data from the Queue.
    // `tail` should wrap around to read the value just pushed.
    // If `tail` does not wrap, it will point to the value 99.
    TEST_ASSERT_EQUAL_UINT8(33, QueuePop(SpiFifo));
}

/* =====[ Mock ReadSpiMaster() for unit-testing GetSensorLED() ]===== */
// Define call recorded when func-under-test calls mocked function.
static RecordedCall * Record_ReadSpiMaster(uint8_t *arg1, uint16_t arg2)
{
    char const *call_name = "ReadSpiMaster";
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
// Define behavior of mocked function: ReadSpiMaster().
uint8_t *FakeByteArray_ForReadSpiMaster;
static uint16_t ReadSpiMaster_Mocked(uint8_t *read_buffer, uint16_t nbytes)
{
    RecordActualCall(mock, Record_ReadSpiMaster(read_buffer, nbytes));
    // Fake reading an array of bytes into the read_buffer.
    // The test is responsible to populate FakeByteArray_ForReadSpiMaster.
    uint16_t num_bytes_read = 0;
    while ( num_bytes_read++ < nbytes )
    {
        *(read_buffer++)  = *(FakeByteArray_ForReadSpiMaster++);
    }
    return nbytes;
}
// Define how to swap function definitions
static uint16_t (*ReadSpiMaster_Saved)(uint8_t *, uint16_t);
// how to restore real definition
static void Restore_ReadSpiMaster(void) { ReadSpiMaster = ReadSpiMaster_Saved; }
// how to swap real definition with mocked version
static void Mock_ReadSpiMaster(void)
{
    ReadSpiMaster_Saved = ReadSpiMaster;
    ReadSpiMaster = ReadSpiMaster_Mocked;
}

/* =====[ Mock WriteSpiMaster() for unit-testing GetSensorLED() ]===== */
// Define call recorded when func-under-test calls mocked function.
static RecordedCall * Record_WriteSpiMaster(uint8_t const *arg1, uint16_t arg2)
{
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
// Define behavior of mocked function: WriteSpiMaster().
// Global for test to spy on array input arg to WriteSpiMaster.
#define max_sizeof_write_buffer 1596
uint8_t SpyOn_WriteSpiMaster_arg1[max_sizeof_write_buffer];
static uint16_t WriteSpiMaster_Mocked(uint8_t const *write_buffer, uint16_t nbytes)
{
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
// Define how to swap function definitions
static uint16_t (*WriteSpiMaster_Saved)(uint8_t const *, uint16_t);
// how to restore real definition
static void Restore_WriteSpiMaster(void) { WriteSpiMaster = WriteSpiMaster_Saved; }
// how to swap real definition with mocked version
static void Mock_WriteSpiMaster(void)
{
    WriteSpiMaster_Saved = WriteSpiMaster;
    WriteSpiMaster = WriteSpiMaster_Mocked;
}

void SetUp_GetSensorLED(void)
{
    SetUp_Mock();
    Mock_ReadSpiMaster();
    Mock_WriteSpiMaster();
}
void TearDown_GetSensorLED(void)
{
    TearDown_Mock();
    Restore_ReadSpiMaster();
    Restore_WriteSpiMaster();
}
void GetSensorLED_sends_OK_to_Bridge_to_signal_ready_for_payload(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    PrintAllCalls(mock);
    uint8_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect 1st call is 'WriteSpiMaster'."
        );
    uint8_t arg_n = 1; uint8_t status = ok; uint8_t *p_status=&status;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArgPointsToValue(mock, call_n, arg_n, &p_status),
        "Expect WriteSpiMaster sends status byte OK."
        );
}
// Continue here
void GetSensorLED_reads_one_byte_of_payload(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "ReadSpiMaster"),
        "Expect call 2 is ReadSpiMaster."
        );
    arg_n = 2; uint16_t num_bytes = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &num_bytes),
        "Expect payload is 1 byte."
        );
}
void GetSensorLED_replies_with_three_bytes_if_led_is_non_existent(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx+100};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 2 is WriteSpiMaster."
        );
    arg_n = 2; uint16_t num_bytes = 3;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &num_bytes),
        "Expect reply is 3 bytes: 2 bytes of size plus 1 byte actual reply."
        );
}
void GetSensorLED_replies_msg_status_error_if_led_is_non_existent(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx+100};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    printf("WriteSpiMaster called with write_buffer[1] == %d\n", SpyOn_WriteSpiMaster_arg1[1]);
    printf("WriteSpiMaster called with write_buffer[2] == %d\n", SpyOn_WriteSpiMaster_arg1[2]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x00, SpyOn_WriteSpiMaster_arg1[0],
        "Expect MSB of repy_size == 0x00."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x01, SpyOn_WriteSpiMaster_arg1[1],
        "Expect LSB of repy_size == 0x01."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x01, SpyOn_WriteSpiMaster_arg1[2],
        "Expect msg_status byte is 0x01 (error) because LED is nonexistent."
        );
}
void GetSensorLED_replies_with_four_bytes_if_led_number_is_recognized(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Test ]===== */
    /* PrintAllCalls(mock); */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "WriteSpiMaster"),
        "Expect call 2 is WriteSpiMaster."
        );
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, &num_bytes),
        "Expect reply is 4 bytes: 2 bytes of size plus 2 bytes actual reply."
        );
}
void GetSensorLED_replies_msg_status_ok_if_led_number_is_recognized(void)
{
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Make sure there are four bytes to read before running test. ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &num_bytes));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[0] == %d\n", SpyOn_WriteSpiMaster_arg1[0]);
    printf("WriteSpiMaster called with write_buffer[1] == %d\n", SpyOn_WriteSpiMaster_arg1[1]);
    printf("WriteSpiMaster called with write_buffer[2] == %d\n", SpyOn_WriteSpiMaster_arg1[2]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x00, SpyOn_WriteSpiMaster_arg1[0],
        "Expect MSB of repy_size == 0x00."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x02, SpyOn_WriteSpiMaster_arg1[1],
        "Expect LSB of repy_size == 0x02."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        0x00, SpyOn_WriteSpiMaster_arg1[2],
        "Expect msg_status byte is 0x00 (ok) because LED is recognized."
        );
}
void GetSensorLED_replies_led_off_if_led_is_off(void)
{
    /* Inject LED state: off */
    BiColorLedOff(led_TxRx);
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Make sure there are four bytes to read before running test. ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &num_bytes));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[3] == %d\n", SpyOn_WriteSpiMaster_arg1[3]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_off, SpyOn_WriteSpiMaster_arg1[3],
        "Expect led_status byte is 0x00 (led_off) because test turns off LED TxRx."
        );
}
void GetSensorLED_replies_led_green_if_led_is_green(void)
{
    /* Inject LED state: off */
    BiColorLedOn(led_TxRx); BiColorLedGreen(led_TxRx);
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Make sure there are four bytes to read before running test. ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &num_bytes));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[3] == %d\n", SpyOn_WriteSpiMaster_arg1[3]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_green, SpyOn_WriteSpiMaster_arg1[3],
        "Expect led_status byte is 0x01 (led_green) because test turns LED TxRx green."
        );
}
void GetSensorLED_replies_led_red_if_led_is_red(void)
{
    /* Inject LED state: off */
    BiColorLedOn(led_TxRx); BiColorLedRed(led_TxRx);
    /* Inject one byte of payload for fake ReadSpiMaster. */
    uint8_t payload[] = {led_TxRx};
    FakeByteArray_ForReadSpiMaster = payload;
    /* =====[ Operate ]===== */
    GetSensorLED();
    /* =====[ Make sure there are four bytes to read before running test. ]===== */
    uint8_t call_n; uint8_t arg_n;
    call_n = 2;
    TEST_ASSERT_TRUE(AssertCall(mock, call_n, "WriteSpiMaster"));
    arg_n = 2; uint16_t num_bytes = 4;
    TEST_ASSERT_TRUE(AssertArg(mock, call_n, arg_n, &num_bytes));
    /* =====[ Test ]===== */
    /* WriteSpiMaster_Mocked spies on values in input arg `write_buffer` */
    printf("WriteSpiMaster called with write_buffer[3] == %d\n", SpyOn_WriteSpiMaster_arg1[3]);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        led_red, SpyOn_WriteSpiMaster_arg1[3],
        "Expect led_status byte is 0x02 (led_red) because test turns LED TxRx red."
        );
}


/* Pull useful stuff from here: */
/* =====[ Jump Table Sandbox ]===== */
/* Functions of type `spi_Cmd` take nothing and return nothing. */
void LookupSensorCmd_returns_Nth_fn_for_Nth_key(void){

    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL(GetSensorLED, LookupSensorCmd(GetSensorLED_key));
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
    /* Fake that led_TxRx pin is red. */
    *BiColorLed_port = 0xFF; // HIGH is red
    /* =====[ Operate ]===== */
    /* Note the parentheses to make it a function call */
    LookupSensorCmd(GetSensorLED_key)(); // call the function returned by lookup
    /* ------------------------------- */
    //=====[ Test ]=====
    // --- 2019-09-30: What am I trying to test here? Does the LED change?
    TEST_ASSERT_BIT_LOW(led_TxRx, *BiColorLed_port); // LOW is green
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

