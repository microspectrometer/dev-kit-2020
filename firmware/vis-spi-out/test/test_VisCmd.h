#ifndef _TEST_VISCMD_H
#define _TEST_VISCMD_H

/* =====[ ReplyCommandInvalid ]===== */
void ReplyCommandInvalid_transmits_one_byte_over_SPI(void);
void ReplyCommandInvalid_sends_byte_INVALID_CMD(void);

/* =====[ SetSensorConfig ]===== */
void SetSensorConfig_receives_three_bytes_of_config_from_Bridge(void);
void SetSensorConfig_replies_ERROR_if_binning_is_invalid(void);
void SetSensorConfig_replies_ERROR_if_gain_is_invalid(void);
void SetSensorConfig_replies_ERROR_if_active_rows_is_invalid(void);
void SetSensorConfig_replies_OK_if_all_config_values_are_valid(void);
void SetSensorConfig_writes_config_to_LIS_770i_programmable_setup_register(void);

#endif // _TEST_VISCMD_H
