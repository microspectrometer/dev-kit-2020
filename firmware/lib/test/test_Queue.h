#ifndef _TEST_QUEUE_H
#define _TEST_QUEUE_H

// Definition and memory alloction of SPI FIFO Rx Buffer is in test_Queue.c
/* extern volatile uint8_t spi_rx_buffer[]; */

void QueueInit_returns_a_pointer_to_a_Queue_struct(void);
void QueueInit_memory_for_Queue_struct_is_allocated_in_Queue_object_file(void);
void QueueInit_assigns_input_buffer_as_Queue_buffer(void);
void QueueInit_size_input_is_the_maximum_Queue_length(void);
void QueueInit_initializes_Queue_with_length_0(void);

#endif // _TEST_QUEUE_H
