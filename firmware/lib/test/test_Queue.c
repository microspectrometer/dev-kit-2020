#include "unity.h"          // unity macros: TEST_BLAH
#include "test_Queue.h" // tests visible to test_runner
#include "Queue.h" // lib under test

/* =====[ Allocate memory for the Spi Rx Queue variables ]===== */
// Define a global pointer to the Queue
volatile Queue_s * SpiFifo;
// Define the maximum number of bytes the SPI FIFO Rx Buffer can hold.
#define MaxLengthOfQueue 5 // bytes

/* =====[ Develop a Queue for the SPI FIFO Rx Buffer ]===== */
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
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    /* =====[ Operate ]===== */
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    /* =====[ Test ]===== */
    // Fill the Queue. When it is full, check it's length.
    uint16_t bytes_pushed = 0;
    while(bytes_pushed < MaxLengthOfQueue)
    {
        QueuePush(SpiFifo, 0);
        bytes_pushed++;
    }
    // Check correct test setup: assert bytes_pushed == MaxLengthOfQueue
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(
        MaxLengthOfQueue, bytes_pushed,
        "Failed to set this test up correctly. "
        "Must push exact number of bytes to fill the buffer."
        );
    // Is length of Queue == size of buffer passed in Init?
    TEST_ASSERT_EQUAL_UINT16(MaxLengthOfQueue, QueueLength(SpiFifo));
}
void test_QueueLength_is_0_after_QueueInit(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    /* =====[ Operate ]===== */
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
}
void test_QueueLength_increments_after_a_push(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    /* =====[ Operate ]===== */
    QueuePush(SpiFifo, 0xAB);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(1, QueueLength(SpiFifo));
}
void test_QueuePush_writes_to_byte_pointed_to_by_head(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    /* =====[ Operate ]===== */
    QueuePush(SpiFifo, 0xAB);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0xAB, spi_rx_buffer[0]);
}
void test_QueuePush_increments_head(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    /* =====[ Operate ]===== */
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    TEST_ASSERT_EQUAL_UINT8(0xAB, spi_rx_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(0xCD, spi_rx_buffer[1]);
}
void test_QueueIsFull_returns_true_if_Queue_is_full(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    // Fill the Queue
    uint16_t bytes_pushed = 0;
    while(bytes_pushed < MaxLengthOfQueue)
    {
        QueuePush(SpiFifo, 0);
        bytes_pushed++;
    }
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(MaxLengthOfQueue, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
}
void test_QueueIsFull_returns_false_if_Queue_is_not_full(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    uint16_t const underfilled = MaxLengthOfQueue-1;
    uint8_t bytes_pushed=0;
    while(bytes_pushed < underfilled)
    {
        QueuePush(SpiFifo, 0xAB);
        bytes_pushed++;
    }
    // Assert the Length is less than the max value.
    TEST_ASSERT_EQUAL_UINT16(underfilled, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(QueueIsFull(SpiFifo));
}
void test_QueueIsEmpty_returns_true_if_Queue_is_empty(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    // Assert the Queue Length is 0 (nothing pushed onto Queue yet)
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
}
void test_QueueIsEmpty_returns_false_if_Queue_is_not_empty(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    QueuePush(SpiFifo, 0xAB);
    // Assert the Queue Length is 1
    TEST_ASSERT_EQUAL_UINT16(1, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
}
void test_QueuePush_does_nothing_if_Queue_is_full(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    // Fill the queue
    uint8_t bytes_pushed=0;
    while(bytes_pushed < MaxLengthOfQueue)
    {
        QueuePush(SpiFifo, 0xCD);
        bytes_pushed++;
    }
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(MaxLengthOfQueue, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    // Try another push.
    QueuePush(SpiFifo, 0xEF);
    /* =====[ Test ]===== */
    // Assert no values were overwritten.
    bytes_pushed=0;
    while(bytes_pushed < MaxLengthOfQueue)
    {
        TEST_ASSERT_EQUAL_UINT8(0xCD, spi_rx_buffer[0]);
        bytes_pushed++;
    }
}
void test_QueueLength_does_not_increase_beyond_max_length(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    // Fill the queue
    uint8_t bytes_pushed=0;
    while(bytes_pushed < MaxLengthOfQueue)
    {
        QueuePush(SpiFifo, 0xCD);
        bytes_pushed++;
    }
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(MaxLengthOfQueue, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    // Try another push.
    QueuePush(SpiFifo, 0xEF);
    /* =====[ Test ]===== */
    // Assert the Length is still at its max value.
    TEST_ASSERT_EQUAL_UINT16(MaxLengthOfQueue, QueueLength(SpiFifo));
}
void test_QueueLength_decrements_after_a_pop(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
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
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
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
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
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
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
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
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    // Put fake old garbage non-zero values in the fake buffer
    spi_rx_buffer[0] = 0xAB;
    spi_rx_buffer[1] = 0xCD;
    TEST_ASSERT_EQUAL_UINT8(0xAB, spi_rx_buffer[0]);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0, QueuePop(SpiFifo));
}
void test_QueuePop_does_not_increment_tail_if_Queue_is_empty(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    // Put fake old garbage non-zero values in the fake buffer
    spi_rx_buffer[0] = 0xAB;
    spi_rx_buffer[1] = 0xCD;
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
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue];
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    // Fill the queue
    uint8_t bytes_pushed=0;
    while(bytes_pushed < MaxLengthOfQueue)
    {
        QueuePush(SpiFifo, 0xCD);
        bytes_pushed++;
    }
    // Assert the Queue is full
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
    // Assert the head is pointing at the last byte index in the Queue.
    TEST_ASSERT_EQUAL_UINT16(MaxLengthOfQueue, QueueLength(SpiFifo));
    // Empty the queue
    uint8_t bytes_popped=0;
    while(bytes_popped < MaxLengthOfQueue)
    {
        QueuePop(SpiFifo);
        bytes_popped++;
    }
    // Assert the Queue is empty.
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
    /* =====[ Operate ]===== */
    // Put more data on the Queue. `head` should wrap around.
    QueuePush(SpiFifo, 33);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_MESSAGE(
        33, spi_rx_buffer[0],
        "Failed to overwrite byte 0! "
        "Head should wrap around when it reaches the end of the array."
        );
}
void test_QueuePop_wraps_tail_back_to_buffer_index_0(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[MaxLengthOfQueue+1]; // add one extra byte
    SpiFifo = QueueInit(spi_rx_buffer, MaxLengthOfQueue);
    uint8_t bytes_pushed=0;
    while(bytes_pushed < MaxLengthOfQueue)
    {
        QueuePush(SpiFifo, 0x11);
        bytes_pushed++;
    }
    // Assert the Queue is full
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
    // Load next byte of buffer with value Pop should never see
    spi_rx_buffer[MaxLengthOfQueue] = 99;
    // Empty the queue
    uint8_t bytes_popped=0;
    while(bytes_popped < MaxLengthOfQueue)
    {
        QueuePop(SpiFifo);
        bytes_popped++;
    }
    // Assert the Queue is empty.
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
    // Put more data on the Queue. `head` should wrap around.
    QueuePush(SpiFifo, 33);
    // Check test is setup: assert byte 0 is 33.
    TEST_ASSERT_EQUAL_MESSAGE(
        33, spi_rx_buffer[0],
        "Failed to overwrite byte 0! "
        "Head should wrap around when it reaches the end of the array."
        );
    /* =====[ Operate and Test ]===== */
    // Pop data from the Queue.
    // `tail` should wrap around to read the value just pushed.
    // If `tail` does not wrap, it will point to the value 99.
    TEST_ASSERT_EQUAL_UINT8(33, QueuePop(SpiFifo));
}

