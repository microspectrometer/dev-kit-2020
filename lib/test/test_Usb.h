#ifndef _TEST_USB_H
#define _TEST_USB_H

void SetUp_UsbRead(void); void TearDown_UsbRead(void);
void UsbRead_returns_false_if_there_was_no_data_read(void);
void UsbRead_returns_true_if_there_is_data_to_read(void);
void UsbRead_should_read_until_buffer_is_empty(void);

/* void SetUp_NothingForUsb(void); void TearDown_NothingForUsb(void); */

#endif // _TEST_USB_H
