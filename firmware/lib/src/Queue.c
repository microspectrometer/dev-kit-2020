#include "Queue.h"

// Define a Queue struct for accessing the SPI FIFO Rx Buffer.
struct Queue_s {
    volatile uint8_t * buffer; // address of SPI FIFO Rx Buffer
    volatile uint8_t head; // index buffer at head for Push
    volatile uint8_t tail; // index buffer at tail for Pop
    volatile uint16_t length; // number of bytes waiting to be read
    volatile uint16_t max_length; // maximum number of bytes the queue can hold
};
// Struct definitions do not declare symbols or allocate memory.
/* volatile Queue_s Queue; // Define Queue and allocate static memory. */
    // The queue must be defined and allocated in the same file where the
    // Queue_s is defined.
    // Attempting to define a Queue in a different file, the compiler throws
    // error: `storage size of 'Queue' isn't known`.
// Client must declare Queue extern to reference the Queue allocated here:
    // extern volatile Queue_s Queue;
// See Queue.h for client usage

// Define Queue API.
/* void QueueInit(volatile Queue_s * pq, volatile uint8_t * pqmem, uint16_t const mem_size) */
volatile Queue_s Queue; // Define Queue and allocate static memory.

volatile Queue_s * QueueInit(volatile uint8_t * pqmem, uint16_t const mem_size)
{
    /** Initialize the global Queue:\n 
     * - use array `spi_rx_buffer` as the queue's buffer\n 
     * - `size_of_spi_rx_buffer_in_bytes` is the max length of the queue in bytes\n 
     * - empty the queue\n 
     * */
    // Create a pointer to the global Queue.
    volatile Queue_s * pq = &Queue;
    // Assign Queue to access the array
    pq->buffer = pqmem;
    // Store array size (this is the maximum length of the Queue)
    pq->max_length = mem_size;
    // Empty the Rx Buffer: by setting head/tail index to first byte
    pq->head = 0;
    pq->tail = 0;
    // queue length is 0
    pq->length = 0;
    return pq;
}
uint16_t QueueLength(volatile Queue_s * pq)
{ // Return length of Queue
    return pq->length;
}
void QueuePush(volatile Queue_s * pq, uint8_t data)
{ // Push data onto the Queue
    if (QueueIsFull(pq)) return;
    // wrap head to beginning of buffer when it reaches the end of the buffer
    if (pq->head >= pq->max_length) pq->head = 0;
    pq->buffer[pq->head++] = data;
    pq->length++;
}
uint8_t QueuePop(volatile Queue_s *pq)
{
    if (QueueIsEmpty(pq)) return 0;
    // wrap tail to beginning of buffer when it reaches the end of the buffer
    if (pq->tail >= pq->max_length) pq->tail = 0;
    pq->length--;
    return pq->buffer[pq->tail++];
}
bool QueueIsFull(volatile Queue_s * pq)
{ // Return true if Queue is full
    if (pq->length >= pq->max_length) return true;
    return false;
}
bool QueueIsEmpty(volatile Queue_s * pq)
{ // Return true if Queue is empty
    if (pq->length == 0) return true;
    return false;
}

