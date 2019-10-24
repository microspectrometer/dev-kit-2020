/** \file */
#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdint.h>     // uint8_t
#include <stdbool.h>    // bool, true, false

// typedef eliminates need to declare a Queue with `struct` keyword.
typedef struct Queue_s Queue_s; // Queue_s definition hidden in Queue.c.

/* =====[ Build ]===== */
// Linking against Queue.o provides access to global `Queue` allocated in static
// memory.
// Application is responsible for creating a pointer to a Queue_s and allocating
// a buffer (an array of bytes) for the Queue to store its queued values.
// The size of the buffer sets the maximum length of the queue.
// QueueInit connects the global `Queue` with the application's buffer and queue
// pointer.
//
/* =====[ API ]===== */
// Context for argument names:
// Queue is for the SPI FIFO Rx Buffer on the Sensor board (the SPI Slave)

volatile Queue_s * QueueInit(
    volatile uint8_t * buffer, // SPI FIFO Rx Buffer
    uint16_t const size_of_buffer_in_bytes // Max length of Spi Rx Queue
    );
uint16_t QueueLength(volatile Queue_s * pq);
void QueuePush(volatile Queue_s * SpiFifo, uint8_t data_to_push);
bool QueueIsFull(volatile Queue_s * SpiFifo);
bool QueueIsEmpty(volatile Queue_s * SpiFifo);
uint8_t QueuePop(volatile Queue_s * SpiFifo);

/* =====[ USAGE ]===== */
// If Client code accesses Queue from only one file:
    // // client.c (only file to acccess Queue)
    // #include "Queue.h"
    // volatile Queue_s * SpiFifo;
    // #define max_length_of_queue 5 // bytes
    // volatile uint8_t spi_rx_buffer[max_length_of_queue];
// If Client code accesses Queue in many files:
    // // move above code from client.c to client_lib.c
    // // any files (like client.c) that need to access SpiFifo:
    // extern volatile Queue_s * SpiFifo;
// In the file that defines SpiFifo and spi_rx_buffer:
    // // Tie Queue to buffer and pointer:
    // SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
// Then any file that can *see* the global Queue can make API calls:
    // if (!QueueIsFull(SpiFifo)) QueuePush(SpiFifo, byte_to_push);
    // if (!QueueIsEmpty(SpiFifo)) popped_byte = QueuePop(SpiFifo);
#endif // _QUEUE_H
