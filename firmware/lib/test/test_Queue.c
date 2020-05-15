/** \file
  * *Queue tests are coupled*\n 
  * Testing one **Queue** method requires calling other **Queue**
  * methods.
  * Lib **Queue** design intent is that data in a `Queue_s` is
  * only accessible through the **Queue** methods.
  * The `Queue_s` is intended to be an abstract data type, but it
  * is exposed in `Queue.h` to make `inline` possible in the
  * application's translation unit.
  *
  * *Empty tests are for generating function doc strings*\n 
  * - Empty tests contain one line: `TEST_PASS();`\n 
  * - Tests start as empty.\n 
  * - If the test is useful, write it using the necessary
  *   **Queue** methods when they are available.
  * */
#include "unity.h"
#include "test_Queue.h"
#include "Queue.h"

/* ------------ */
/* Queue memory */
/* ------------ */
// Tests allocate static memory for the queue object.
volatile Queue_s * SpiFifo;
// Define maximum size (bytes) of the queue's buffer
#define max_length_of_queue 5
/* --------------------------------------------------- */
/* FIFO buffer memory is allocated local to each test. */
/* --------------------------------------------------- */
// Memory for the buffer (`spi_rx_buffer`) is allocated on the
// stack (`spi_rx_buffer` is a local variable in each test).
// This eliminates the need for buffer setup/teardown code.

void QueueInit_returns_a_pointer_to_a_Queue_struct(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    /* =====[ Operate and Test ]===== */
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    TEST_PASS();
}
void QueueInit_memory_for_Queue_struct_is_allocated_in_Queue_object_file(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    /* =====[ Operate and Test ]===== */
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    /* printf( */
    /*     "QueueInit accesses memory allocated " */
    /*     "for Queue object in Queue.o\n" */
    /*     ); */
    TEST_PASS();
}
void QueueInit_assigns_input_buffer_as_Queue_buffer(void)
{
    TEST_PASS();
}
void QueueInit_size_input_is_the_maximum_Queue_length(void)
{
    TEST_PASS();
}
void QueueInit_initializes_Queue_with_length_0(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    /* =====[ Operate ]===== */
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
}
void QueueLength_increments_after_a_push(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    /* =====[ Operate ]===== */
    QueuePush(SpiFifo, 0xAB);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(1, QueueLength(SpiFifo));
}
void QueueLength_does_not_increase_beyond_max_length(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Fill the queue
    uint8_t bytes_pushed=0;
    while(bytes_pushed < max_length_of_queue)
    {
        QueuePush(SpiFifo, 0xCD);
        bytes_pushed++;
    }
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(max_length_of_queue, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    // Try another push.
    QueuePush(SpiFifo, 0xEF);
    /* =====[ Test ]===== */
    // Assert the Length is still at its max value.
    TEST_ASSERT_EQUAL_UINT16(max_length_of_queue, QueueLength(SpiFifo));
}
void QueueLength_decrements_after_a_pop(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
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
void QueueLength_does_not_decrease_below_zero(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
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
void QueuePush_writes_byte_to_Queue_buffer(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    /* =====[ Operate ]===== */
    QueuePush(SpiFifo, 0xAB);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0xAB, spi_rx_buffer[0]);
}
void QueuePush_writes_next_byte_to_address_after_previous_write(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    /* =====[ Operate ]===== */
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    TEST_ASSERT_EQUAL_UINT8(0xAB, spi_rx_buffer[0]);
    TEST_ASSERT_EQUAL_UINT8(0xCD, spi_rx_buffer[1]);
}
void QueuePush_does_not_write_byte_if_Queue_is_full(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Fill the queue
    uint8_t bytes_pushed=0;
    while(bytes_pushed < max_length_of_queue)
    {
        QueuePush(SpiFifo, 0xCD);
        bytes_pushed++;
    }
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(max_length_of_queue, QueueLength(SpiFifo));
    /* =====[ Operate ]===== */
    // Try another push.
    QueuePush(SpiFifo, 0xEF);
    /* =====[ Test ]===== */
    // Assert no values were overwritten.
    bytes_pushed=0;
    while(bytes_pushed < max_length_of_queue)
    {
        TEST_ASSERT_EQUAL_UINT8(0xCD, spi_rx_buffer[0]);
        bytes_pushed++;
    }
}
void QueuePush_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_full(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Fill the queue
    uint8_t bytes_pushed=0;
    while(bytes_pushed < max_length_of_queue)
    {
        QueuePush(SpiFifo, 0xCD);
        bytes_pushed++;
    }
    // Assert the Queue is full
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
    // Assert the head is pointing at the last byte index in the Queue.
    TEST_ASSERT_EQUAL_UINT16(max_length_of_queue, QueueLength(SpiFifo));
    // Empty the queue
    uint8_t bytes_popped=0;
    while(bytes_popped < max_length_of_queue)
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
void QueuePop_removes_oldest_byte_from_Queue(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Fill the queue
    QueuePush(SpiFifo, 0xAB); // oldest byte in Queue
    QueuePush(SpiFifo, 0xCD); // next-oldest byte in Queue
    /* =====[ Operate ]===== */
    uint8_t byte_0 = QueuePop(SpiFifo);
    /* =====[ Test ]===== */
    // Assert the oldest byte is read.
    TEST_ASSERT_EQUAL_UINT8(0xAB, byte_0);
    /* =====[ Operate ]===== */
    uint8_t byte_1 = QueuePop(SpiFifo);
    /* =====[ Test ]===== */
    // Assert the next-oldest byte is read.
    // This implies the oldest byte was removed on the previous pop.
    TEST_ASSERT_EQUAL_UINT8(0xCD, byte_1);
}
void QueuePop_returns_oldest_byte(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Fill the queue
    QueuePush(SpiFifo, 0xAB);
    QueuePush(SpiFifo, 0xCD);
    /* =====[ Operate ]===== */
    uint8_t byte_0 = QueuePop(SpiFifo);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0xAB, byte_0);
}
void QueuePop_does_not_remove_any_bytes_if_Queue_is_empty(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Put fake old garbage non-zero values in the fake buffer
    spi_rx_buffer[0] = 0xAB;
    spi_rx_buffer[1] = 0xCD;
    TEST_ASSERT_EQUAL_UINT8(0, QueuePop(SpiFifo));
    // Removing a byte means advancing tail.
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
void QueuePop_returns_0_if_Queue_is_empty(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Put fake old garbage non-zero values in the fake buffer
    spi_rx_buffer[0] = 0xAB;
    spi_rx_buffer[1] = 0xCD;
    TEST_ASSERT_EQUAL_UINT8(0xAB, spi_rx_buffer[0]);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(0, QueuePop(SpiFifo));
}
void QueuePop_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_empty(void)
{
    /* =====[ Setup ]===== */
    // Allocate buffer with one extra byte.
    volatile uint8_t spi_rx_buffer[max_length_of_queue+1];
    // Extra byte is to place a value that Pop should never see.
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    uint8_t bytes_pushed=0;
    while(bytes_pushed < max_length_of_queue)
    {
        QueuePush(SpiFifo, 0x11);
        bytes_pushed++;
    }
    // Assert the Queue is full
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
    // Load next byte of buffer with value Pop should never see
    spi_rx_buffer[max_length_of_queue] = 99;
    // Empty the queue
    uint8_t bytes_popped=0;
    while(bytes_popped < max_length_of_queue)
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
void QueueIsFull_returns_true_if_Queue_is_full(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Fill the Queue
    uint16_t bytes_pushed = 0;
    while(bytes_pushed < max_length_of_queue)
    {
        QueuePush(SpiFifo, 0);
        bytes_pushed++;
    }
    // Assert the Length is at its max value.
    TEST_ASSERT_EQUAL_UINT16(max_length_of_queue, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(QueueIsFull(SpiFifo));
}
void QueueIsFull_returns_false_if_Queue_is_not_full(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    uint16_t const underfilled = max_length_of_queue-1;
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
void QueueIsEmpty_returns_true_if_Queue_is_empty(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Assert the Queue Length is 0 (nothing pushed onto Queue yet)
    TEST_ASSERT_EQUAL_UINT16(0, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(QueueIsEmpty(SpiFifo));
}
void QueueIsEmpty_returns_false_if_Queue_is_not_empty(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, 0xAB);
    // Assert the Queue Length is 1
    TEST_ASSERT_EQUAL_UINT16(1, QueueLength(SpiFifo));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(QueueIsEmpty(SpiFifo));
}

