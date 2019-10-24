#ifndef _TEST_QUEUE_H
#define _TEST_QUEUE_H

// Definition and memory alloction of SPI FIFO Rx Buffer is in test_Queue.c
extern volatile uint8_t spi_rx_buffer[];


/* void test_struct_syntax(void); */
void test_QueueLength_is_0_after_QueueInit(void);
void test_QueueLength_increments_after_a_push(void);
void test_QueuePush_writes_to_byte_pointed_to_by_head(void);
void test_QueuePush_increments_head(void);
void test_QueuePush_does_nothing_if_Queue_is_full(void);
void test_QueueIsFull_returns_true_if_Queue_is_full(void);
void test_QueueIsFull_returns_false_if_Queue_is_not_full(void);
void test_QueueLength_decrements_after_a_pop(void);
void test_QueueLength_does_not_increase_beyond_max_length(void);
void test_QueuePop_reads_byte_pointed_to_by_tail(void);
void test_QueuePop_increments_tail(void);
void test_QueueLength_does_not_decrease_below_zero(void);
void test_QueuePop_returns_0_if_Queue_is_empty(void);
void test_QueuePop_does_not_increment_tail_if_Queue_is_empty(void);
void test_QueueIsEmpty_returns_true_if_Queue_is_empty(void);
void test_QueueIsEmpty_returns_false_if_Queue_is_not_empty(void);
void test_QueuePush_wraps_head_back_to_buffer_index_0(void);
void test_QueueInit_sets_buffer_length(void);
void test_QueuePop_wraps_tail_back_to_buffer_index_0(void);

#endif // _TEST_QUEUE_H
