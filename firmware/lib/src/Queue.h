#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdint.h>     // uint8_t
#include <stdbool.h>    // bool, true, false
typedef struct Queue_s Queue_s;
// ---API---
volatile Queue_s * QueueInit(
    volatile uint8_t * buffer, // SPI FIFO Rx Buffer
    uint16_t const size_of_buffer_in_bytes // Max length of Spi Rx Queue
    );

#endif // _QUEUE_H
