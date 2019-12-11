#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdint.h>     // uint8_t
#include <stdbool.h>    // bool, true, false
typedef struct Queue_s Queue_s;
// ---API---
volatile Queue_s * QueueInit(
    volatile uint8_t * buffer, // SPI FIFO Rx Buffer
    uint16_t const buffer_size_in_bytes // Max length of Spi Rx Queue
    );
uint16_t QueueLength(volatile Queue_s * pq);
void QueuePush(volatile Queue_s * SpiFifo, uint8_t data_to_push);
bool QueueIsFull(volatile Queue_s * SpiFifo);
bool QueueIsEmpty(volatile Queue_s * SpiFifo);
uint8_t QueuePop(volatile Queue_s * SpiFifo);

#endif // _QUEUE_H
