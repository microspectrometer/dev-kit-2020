#ifndef _TEST_USB_H
#define _TEST_USB_H

/* void SetUp_NothingForUsb(void); void TearDown_NothingForUsb(void); */

void SetUp_UsbRead(void); void TearDown_UsbRead(void);
void UsbRead_returns_0_if_there_was_no_data_to_read(void);
void UsbRead_returns_N_if_there_were_N_bytes_to_read(void);
void UsbRead_turns_LED_red_if_there_was_no_data_to_read(void);
void UsbRead_copies_bytes_to_the_input_read_buffer(void);
/* ---------------------------------------------------------------------- */
/* =====[ WIP: robust UsbRead functionality started 2019-03-01 ]===== */
void UsbReadOneByte_copies_the_next_available_byte_to_the_input_read_buffer(void);
void UsbReadOneByte_returns_0_if_there_are_no_bytes_to_read(void);
void UsbReadOneByte_returns_1_if_there_is_at_least_one_byte_to_read(void);
void UsbReadOneByte_example_readings_several_bytes(void);
/* ---------------------------------------------------------------------- */
void SetUp_DetailsOf_UsbRead(void); void TearDown_DetailsOf_UsbRead(void);
void UsbRead_sad_path_is_implemented_like_this(void);
void UsbRead_happy_path_is_implemented_like_this(void);

void SetUp_UsbWrite(void); void TearDown_UsbWrite(void);
void UsbWrite_returns_the_number_of_bytes_sent(void);
void UsbWrite_calls_FtWrite_for_each_byte_to_send(void);
void UsbWrite_stops_sending_bytes_if_the_tx_buffer_is_full(void);
void UsbWrite_turns_LED_red_if_the_tx_buffer_was_already_full(void);
void UsbWrite_returns_0_if_the_tx_buffer_was_already_full(void);
void SetUp_DetailsOf_UsbWrite(void); void TearDown_DetailsOf_UsbWrite(void);
void UsbWrite_happy_path_is_implemented_like_this(void);
void UsbWrite_sad_path_is_implemented_like_this(void);

void SetUp_UsbHasDataToRead(void); void TearDown_UsbHasDataToRead(void);
void UsbHasDataToRead_returns_true_if_the_rx_buffer_has_data(void);
void UsbHasDataToRead_returns_false_if_the_rx_buffer_is_empty(void);

void SetUp_UsbHasRoomToWrite(void); void TearDown_UsbHasRoomToWrite(void);
void UsbHasRoomToWrite_returns_true_if_the_tx_buffer_is_not_full(void);
void UsbHasRoomToWrite_returns_false_if_the_tx_buffer_is_full(void);


#endif // _TEST_USB_H
