#ifndef _TEST_VISCMD_H
#define _TEST_VISCMD_H

/* =====[ ReplyCommandInvalid ]===== */
void ReplyCommandInvalid_transmits_one_byte_over_SPI(void);
void ReplyCommandInvalid_sends_byte_INVALID_CMD(void);

/* =====[ SetSensorConfig ]===== */
void SetSensorConfig_receives_three_bytes_of_config_from_Bridge(void);
void SetSensorConfig_replies_msg_status_error_if_binning_is_invalid(void);

#endif // _TEST_VISCMD_H
