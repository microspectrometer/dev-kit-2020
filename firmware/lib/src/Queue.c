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
volatile Queue_s * QueueInit(volatile uint8_t * pqmem, uint16_t const mem_size)
{
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
