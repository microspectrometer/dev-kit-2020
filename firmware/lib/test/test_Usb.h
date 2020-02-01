#ifndef _TEST_USB_H
#define _TEST_USB_H

/* =====[ UsbRxbufferIsEmpty ]===== */
void UsbRxbufferIsEmpty_returns_true_if_pin_FT1248_MISO_is_HIGH(void);
void UsbRxbufferIsEmpty_returns_false_if_pin_FT1248_MISO_is_LOW(void);

/* =====[ UsbRxbufferIsFull ]===== */
void UsbRxbufferIsFull_returns_false_if_pin_MIOSIO0_is_HIGH(void);
void UsbRxbufferIsFull_returns_true_if_pin_MIOSIO0_is_LOW(void);

#endif // _TEST_USB_H
