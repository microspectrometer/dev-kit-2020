#ifndef _TEST_USB_H
#define _TEST_USB_H

/* void SetUp_NothingForUsb(void); void TearDown_NothingForUsb(void); */

void SetUp_UsbRead(void); void TearDown_UsbRead(void);
void UsbRead_returns_0_if_there_was_no_data_to_read(void);
void UsbRead_returns_N_if_there_were_N_bytes_to_read(void);
void UsbReadN_reads_N_bytes_and_returns_num_bytes_read(void);
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

/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
void LookupCmd_returns_Nth_fn_for_Nth_key(void);
void LookupCmd_returns_NULL_if_key_is_not_in_jump_table(void);
void LookupCmd_example_calling_the_command(void);
void LookupCmd_example_storing_the_returned_pointer(void);
void UsbWriteStatusOk_tells_UsbHost_command_was_success(void);
void UsbWriteStatusInvalid_sends_error_byte_and_echoes_invalid_command(void);
void UsbWriteStatusBadArgs_sends_error_byte_and_echoes_invalid_command(void);
void UsbWriteStatusMissingArgs_sends_error_byte_and_echoes_invalid_command(void);
void LookupCmd_sad_example_using_UsbWriteStatus_API(void);
void LookupCmd_happy_example_using_UsbWriteStatus_API(void);
void CmdCfgLis_returns_StatusOk_and_echoes_back_the_4_cfg_bytes(void);
void CmdCfgLis_returns_StatusMismatch_if_cfg_bytes_are_invalid(void);
void CmdCfgLis_sends_cfg_to_mBrd_and_reads_back_new_cfg_before_reporting_StatusOk(void);
void CfgBytesAreValid_checks_against_all_255_valid_configs(void);
void CfgTurnsOffAllPixels_returns_true_if_cfg_turns_off_all_pixels(void);
void CfgTurnsOffAllPixels_ignores_the_3LSB_and_4MSB_of_cfg(void);
void CfgTurnsOffAllPixels_returns_false_if_any_pixels_are_on(void);
void CfgTurnsRowPartiallyOn_returns_true_if_cfg_turns_on_part_of_row1(void);
void XOR_returns_true_if_a_is_true_and_b_is_false(void);
void CfgTurnsRowPartiallyOn_returns_false_if_cfg_turns_on_all_of_row1(void);
void CfgTurnsRowPartiallyOn_returns_true_if_cfg_turns_on_part_of_row5(void);
void CfgTurnsRowPartiallyOn_returns_true_if_row_number_is_out_bounds(void);

#endif // _TEST_USB_H
