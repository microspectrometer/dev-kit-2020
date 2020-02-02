#ifndef _TEST_USB_H
#define _TEST_USB_H

/* =====[ UsbRxbufferIsEmpty ]===== */
void UsbRxbufferIsEmpty_returns_true_if_pin_FT1248_MISO_is_HIGH(void);
void UsbRxbufferIsEmpty_returns_false_if_pin_FT1248_MISO_is_LOW(void);

/* =====[ UsbRxbufferIsFull ]===== */
void UsbRxbufferIsFull_returns_false_if_pin_MIOSIO0_is_HIGH(void);
void UsbRxbufferIsFull_returns_true_if_pin_MIOSIO0_is_LOW(void);

/* =====[ _FtDriveDatabus ]===== */
void FtDriveDatabus_drives_FtClock_HIGH_to_drive_data_onto_the_bus(void);

/* =====[ _FtWriteDatabus ]===== */
void FtWriteDatabus_makes_FtMiosio0_to_FtMiosio7_output_pins(void);
void FtWriteDatabus_outputs_data_on_FtMiosio0_to_FtMiosio7(void);

/* =====[ _FtSampleDatabus ]===== */
void FtSampleDatabus_drives_FtClock_LOW_to_sample_data_from_the_bus(void);

/* =====[ UsbRxbufferPop ]===== */
void UsbRxbufferPop_selects_the_FT221X(void);
void UsbRxbufferPop_drives_databus_with_read_command(void);
void UsbRxbufferPop_signals_FT221X_to_sample_the_databus(void);

#endif // _TEST_USB_H
