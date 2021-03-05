#ifndef _TEST_USB_H
#define _TEST_USB_H

// ---Run_Usb_Private_Ft_tests---

/* =====[ _FtClockDatabus ]===== */
void FtClockDatabus_drives_FtClock_HIGH_if_direction_is_FtDrive(void);
void FtClockDatabus_drives_FtClock_LOW_if_direction_is_FtSample(void);

/* =====[ _FtReadDatabus ]===== */
void FtReadDatabus_copies_databus_pin_values_to_address_pbyte(void);

/* =====[ _FtWriteDatabus ]===== */
void FtWriteDatabus_outputs_byte_on_databus_pins(void);

/* =====[ _FtDatabusPinDirection ]===== */
void FtDatabusPinDirection_makes_databus_pins_outputs_if_direction_is_FtOut(void);
void FtDatabusPinDirection_makes_databus_pins_inputs_if_direction_is_FtIn(void);

/* =====[ FtSelectFT221X ]===== */
void FtSelectFT221X_drives_FtChipSelect_LOW(void);

/* =====[ FtUnselectFT221X ]===== */
void FtUnselectFT221X_drives_FtChipSelect_HIGH(void);

/* =====[ FtBusTurnaround ]===== */
void FtBusTurnaround_clocks_one_cycle_to_signal_data_drive_then_data_sample(void);

/* =====[ FtIsOk ]===== */
void FtIsOk_returns_true_if_FtMiso_is_LOW(void);
void FtIsOk_returns_false_if_FtMiso_is_HIGH(void);

/* =====[ FtRead ]===== */
void FtRead_clocks_one_byte_out_of_the_FT221X(void);
void FtRead_stores_the_byte_at_address_pbyte(void);

/* =====[ FtWrite ]===== */
void FtWrite_signals_to_drive_data_onto_the_databus(void);
void FtWrite_sets_microcontroller_databus_pins_as_outputs(void);
void FtWrite_outputs_byte_on_databus_pins(void);
void FtWrite_signals_FT221X_to_sample_the_databus(void);
void FtWrite_sets_microcontroller_databus_pins_as_inputs(void);

// ---Run_Usb_API_tests---

/* =====[ UsbRxbufferIsEmpty ]===== */
void UsbRxbufferIsEmpty_returns_true_if_pin_FT1248_MISO_is_HIGH(void);
void UsbRxbufferIsEmpty_returns_false_if_pin_FT1248_MISO_is_LOW(void);

/* =====[ UsbTxbufferIsFull ]===== */
void UsbTxbufferIsFull_returns_true_if_pin_MIOSIO0_is_HIGH(void);
void UsbTxbufferIsFull_returns_false_if_pin_MIOSIO0_is_LOW(void);

/* =====[ UsbReadByte ]===== */
void UsbReadByte_selects_the_FT221X(void);
void UsbReadByte_drives_databus_with_read_command(void);
void UsbReadByte_signals_FT221X_to_sample_the_databus(void);
void UsbReadByte_reads_the_byte_if_data_transfer_status_is_OK(void);
void UsbReadByte_unselects_the_FT221X(void);
void UsbReadByte_returns_either_FtOK_if_pbyte_has_the_read_data_or_FtError_if_Usb_receive_buffer_was_empty(void);

/* =====[ UsbWriteByte ]===== */

#endif // _TEST_USB_H
