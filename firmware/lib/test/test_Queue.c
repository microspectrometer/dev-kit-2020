#include "unity.h"
#include "test_Queue.h"
#include "Queue.h"
// ---Queue tests are coupled---
// Testing one Queue method requires calling other Queue methods.
// Data in a queue_s is only accessible through the Queue methods
// because Queue is an abstract data type.
// ---Queue tests are empty---
// Empty tests are for generating function doc strings.
// Tests start as empty. If the test is useful, write it using the
// necessary Queue methods when they are availalbe.

// ---Queue memory---
// Allocate static memory for the queue object
volatile Queue_s * SpiFifo;
// Define maximum size (bytes) of the queue's buffer
#define max_length_of_queue 5 // bytes
// Memory for the buffer (spi_rx_buffer) is allocated on the stack
// (it is local to each test). This eliminates the need for
// buffer setup/teardown code.

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
{ TEST_PASS(); }
void QueueInit_size_input_is_the_maximum_Queue_length(void)
{ TEST_PASS(); }
void QueueInit_initializes_Queue_with_length_0(void)
{ TEST_PASS(); }
