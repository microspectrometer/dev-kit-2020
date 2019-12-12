/** \file */
#include "Queue.h"
// Define a Queue struct for accessing the SPI FIFO Rx Buffer.
//! Queue uses a byte array as a circular buffer.
/** A *queue* is a byte array with data (head, tail) and methods
 * (push, pop) that access linear memory like circular memory. The
 * memory is finite (the *queue* can fill up) but the user cannot
 * "fall off" the end of the byte array.\n\n 
 * Picture the byte array as a closed loop like the numbers on a
 * clock: the first byte is the next byte after the last byte. And
 * the *head* and *tail* are clock-hands pointing at the array
 * elements. The bytes *between* head and tail are the bytes in the
 * queue. Other bytes in the buffer are not in the queue, meaning
 * they cannot be accessed by a *pop*, though the values remain
 * stored in the buffer until overwritten by a *push*.\n\n 
 * A queue is a FIFO
 * (first-in first-out) buffer: *pop* removes the oldest byte in
 * the queue.
 * */
struct Queue_s {
    /** *buffer* is the address of the queue's FIFO buffer\n 
      *  - buffer is an array of bytes\n 
      *  - buffer size is set by user\n 
      *  - buffer memory is allocated by user\n 
      * */
    volatile uint8_t * buffer;
    /** *head* advances after a byte is pushed onto the queue\n 
      * - *push* indexes buffer at head
      * */
    volatile uint8_t head; // index buffer at head for Push
    /** *tail* advances after a byte is popped from the queue\n 
      * - *pop* indexes buffer at tail
      * */
    volatile uint8_t tail;
    /** *length* is the number of unread bytes in the queue */
    volatile uint16_t length;
    /** *max_length* is the maximum number of bytes the queue can hold\n 
     * - max_length is set by the user
     * */
    volatile uint16_t max_length;
};
// Define Queue and allocate static memory.
volatile Queue_s Queue;
// ---API---
volatile Queue_s * QueueInit(
    volatile uint8_t * buffer,
    uint16_t const buffer_size_in_bytes
    )
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
void QueuePush(
    volatile Queue_s * pq,
    uint8_t data
    )
{ //! Push data onto the Queue
    /** QueuePush behavior:\n 
      * - writes byte to Queue buffer\n 
      * - writes next byte to address after previous write\n 
      * - does not write byte if Queue is full\n 
      * - hits end of buffer and wraps around if Queue is not full\n 
      * */
    if (QueueIsFull(pq)) return;
    // wrap head to beginning of buffer when it reaches the end of the buffer
    if (pq->head >= pq->max_length) pq->head = 0;
    pq->buffer[pq->head++] = data;
    pq->length++;
}
uint8_t QueuePop(volatile Queue_s *pq)
{ //! Pop data off of the Queue
    /** QueuePop behavior:\n 
      * - reads oldest byte in Queue\n 
      * - removes oldest byte from Queue\n 
      * */
    if (QueueIsEmpty(pq)) return 0;
    // wrap tail to beginning of buffer when it reaches the end of the buffer
    if (pq->tail >= pq->max_length) pq->tail = 0;
    pq->length--;
    // Return the byte. Remove byte from Queue by advancing "tail".
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

