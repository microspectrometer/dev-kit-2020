#ifndef _TEST_QUEUE_H
#define _TEST_QUEUE_H

// Definition and memory alloction of SPI FIFO Rx Buffer is in test_Queue.c
/* extern volatile uint8_t spi_rx_buffer[]; */

void QueueInit_returns_a_pointer_to_a_Queue_struct(void);
void QueueInit_memory_for_Queue_struct_is_allocated_in_Queue_object_file(void);
void QueueInit_assigns_input_buffer_as_Queue_buffer(void);
void QueueInit_size_input_is_the_maximum_Queue_length(void);
void QueueInit_initializes_Queue_with_length_0(void);
void QueueLength_increments_after_a_push(void);
void QueueLength_does_not_increase_beyond_max_length(void);
void QueueLength_decrements_after_a_pop(void);
void QueueLength_does_not_decrease_below_zero(void);
void QueuePush_writes_byte_to_Queue_buffer(void);
void QueuePush_writes_next_byte_to_address_after_previous_write(void);
void QueuePush_does_not_write_byte_if_Queue_is_full(void);
void QueuePush_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_full(void);
void QueuePop_removes_oldest_byte_from_Queue(void);
void QueuePop_returns_oldest_byte(void);
void QueuePop_returns_0_if_Queue_is_empty(void);
void QueuePop_does_not_remove_any_bytes_if_Queue_is_empty(void);
void QueuePop_hits_end_of_buffer_and_wraps_around_if_Queue_is_not_empty(void);
void QueueIsFull_returns_true_if_Queue_is_full(void);
void QueueIsFull_returns_false_if_Queue_is_not_full(void);
void QueueIsEmpty_returns_true_if_Queue_is_empty(void);
void QueueIsEmpty_returns_false_if_Queue_is_not_empty(void);

#endif // _TEST_QUEUE_H
