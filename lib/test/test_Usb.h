#ifndef _TEST_USB_H
#define _TEST_USB_H

/* void SetUp_NothingForUsb(void); void TearDown_NothingForUsb(void); */

void SetUp_UsbRead(void); void TearDown_UsbRead(void);
void UsbRead_returns_0_if_there_was_no_data_to_read(void);
void UsbRead_turns_LED_red_if_there_was_no_data_read(void);
void UsbRead_returns_N_if_there_were_N_bytes_to_read(void);
void UsbRead_should_read_until_buffer_is_empty(void);
void UsbRead_copies_bytes_to_the_input_read_buffer_address(void);

void SetUp_UsbWrite(void); void TearDown_UsbWrite(void);
void UsbWrite_returns_the_number_of_bytes_sent(void);
void UsbWrite_calls_FtWrite_for_each_byte_to_send(void);

#endif // _TEST_USB_H
