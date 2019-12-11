/** \file */
#include "Queue.h"
// Define a Queue struct for accessing the SPI FIFO Rx Buffer.
struct Queue_s {
    volatile uint8_t * buffer; // address of SPI FIFO Rx Buffer
    volatile uint8_t head; // index buffer at head for Push
    volatile uint8_t tail; // index buffer at tail for Pop
    volatile uint16_t length; // number of bytes waiting to be read
    volatile uint16_t max_length; // maximum number of bytes the queue can hold
};
// Define Queue and allocate static memory.
volatile Queue_s Queue;
// ---API---
volatile Queue_s * QueueInit(volatile uint8_t * buffer, uint16_t const buffer_size_in_bytes)
{ //! Return a pointer to the global Queue
    /** QueueInit behavior:\n 
      * - returns a pointer to a Queue struct\n 
      * - memory for Queue struct is allocated in Queue object file\n 
      * - assigns input buffer as Queue buffer\n 
      * - size input is the maximum Queue length\n 
      * - initializes Queue with length 0\n 
      * */
    // Create a pointer to the global Queue.
    volatile Queue_s * pq = &Queue;
    // Assign Queue to access the array
    pq->buffer = buffer;
    // Store array size (this is the maximum length of the Queue)
    pq->max_length = buffer_size_in_bytes;
    // Empty the Rx Buffer: by setting head/tail index to first byte
    pq->head = 0;
    pq->tail = 0;
    // queue length is 0
    pq->length = 0;
    return pq;
}
uint16_t QueueLength(volatile Queue_s * pq)
{ //! Return length of Queue
    /** QueueLength behavior:\n 
      * - increments after a push\n 
      * - does not increase beyond max length\n 
      * - decrements after a pop\n 
      * - does not decrease below zero\n 
      * */

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

