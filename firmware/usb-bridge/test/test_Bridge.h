#ifndef _TEST_BRIDGE_H
#define _TEST_BRIDGE_H

/* =====[ WIP: Clean Command Parsing with jump tables started 2019-03-01 ]===== */
void LookupBridgeCmd_example_calling_the_command(void);
void LookupBridgeCmd_example_storing_the_returned_pointer(void);
void UsbWriteStatusOk_tells_UsbHost_command_was_success(void);
void UsbWriteStatusInvalid_sends_error_byte_and_echoes_invalid_command(void);
void UsbWriteStatusBadArgs_sends_error_byte_and_echoes_invalid_command(void);
void UsbWriteStatusMissingArgs_sends_error_byte_and_echoes_invalid_command(void);
void UsbWriteStatusSpiBusError_sends_error_byte_and_slave_cmd(void);
void LookupBridgeCmd_sad_example_using_UsbWriteStatus_API(void);
void LookupBridgeCmd_happy_example_using_UsbWriteStatus_API(void);
void CmdCfgLis_returns_StatusOk_and_echoes_back_the_4_cfg_bytes(void);
void CmdCfgLis_returns_StatusBadArgs_if_cfg_bytes_are_invalid(void);
void CmdCfgLis_1pushes_cfg_to_SpiSlave_2pulls_updated_cfg_3reports_StatusOk_updated_cfg(void);
void CfgBytesAreValid_checks_against_all_255_valid_configs(void);
void CfgTurnsOffAllPixels_returns_true_if_cfg_turns_off_all_pixels(void);
void CfgTurnsOffAllPixels_ignores_the_3LSB_and_4MSB_of_cfg(void);
void CfgTurnsOffAllPixels_returns_false_if_any_pixels_are_on(void);
void CfgTurnsRowPartiallyOn_returns_true_if_cfg_turns_on_part_of_row1(void);
void XOR_returns_true_if_a_is_true_and_b_is_false(void);
void CfgTurnsRowPartiallyOn_returns_false_if_cfg_turns_on_all_of_row1(void);
void CfgTurnsRowPartiallyOn_returns_true_if_cfg_turns_on_part_of_row5(void);
void CfgTurnsRowPartiallyOn_returns_true_if_row_number_is_out_bounds(void);
void BytesComing_returns_16bit_word_from_struct_spi_NBytesToExpect(void);

/* =====[ API - started 2019-10-03 ]===== */
void SetUp_GetBridgeLED(void); void TearDown_GetBridgeLED(void);
void GetBridgeLED_reads_one_byte_of_payload(void);
void GetBridgeLED_replies_msg_status_error_if_host_queries_nonexistent_led(void);
void GetBridgeLED_replies_with_one_byte_if_led_number_is_not_recognized(void);
void GetBridgeLED_replies_msg_status_ok_if_host_queries_status_led(void);
void GetBridgeLED_replies_with_two_bytes_if_led_number_is_recognized(void);
void GetBridgeLED_replies_with_msg_status_byte_and_led_status_byte(void);
void GetBridgeLED_replies_led_off_if_status_led_is_off(void);
void GetBridgeLED_replies_led_green_if_status_led_is_green(void);
void GetBridgeLED_replies_led_red_if_status_led_is_red(void);
void SetUp_SetBridgeLED(void); void TearDown_SetBridgeLED(void);
/* =====[ ApiSupport - started 2019-10-04 ]===== */
void SetUp_SendStatus_writes_one_byte_over_USB(void);
void TearDown_SendStatus_writes_one_byte_over_USB(void);
void SendStatus_writes_one_byte_over_USB(void);

void SetUp_Stub_UsbReadN_with_value_in_read_buffer(void);
void TearDown_Stub_UsbReadN_with_value_in_read_buffer(void);
void Stub_UsbReadN_with_value_in_read_buffer(void);
/* =====[ BridgeJumpTable - started 2019-10-02]===== */
void LookupBridgeCmd_takes_key_and_returns_fn_ptr(void);
void LookupBridgeCmd_returns_NULL_if_key_is_not_found(void);
void SetBridgeLED_reads_two_bytes_of_payload(void);
void SetBridgeLED_replies_with_one_byte(void);
void SetBridgeLED_replies_msg_status_ok_if_led_number_is_status_led(void);
void SetBridgeLED_replies_msg_status_error_if_led_number_is_not_recognized(void);
void SetBridgeLED_turns_off_led_if_payload_is_led_off(void);

#endif // _TEST_BRIDGE_H
