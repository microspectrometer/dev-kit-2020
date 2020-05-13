#ifndef _TEST_VISCMD_H
#define _TEST_VISCMD_H

/* =====[ ReplyCommandInvalid ]===== */
void ReplyCommandInvalid_transmits_one_byte_over_SPI(void);
void ReplyCommandInvalid_sends_byte_INVALID_CMD(void);

/* =====[ SetSensorConfig ]===== */
void SetSensorConfig_receives_three_bytes_of_config_from_Bridge(void);
void SetSensorConfig_does_not_update_config_globals_if_config_is_invalid(void);
void SetSensorConfig_replies_ERROR_if_binning_is_invalid(void);
void SetSensorConfig_replies_ERROR_if_gain_is_invalid(void);
void SetSensorConfig_replies_ERROR_if_active_rows_is_invalid(void);
void SetSensorConfig_does_not_program_LIS_770i_if_config_is_invalid(void);
void SetSensorConfig_writes_valid_config_to_LIS_770i_programmable_setup_register(void);
void SetSensorConfig_replies_OK_if_all_config_values_are_valid(void);
void SetSensorConfig_the_OK_is_sent_after_LIS_is_programmed_with_new_config(void);
// GetSensorLED()
void GetSensorLED_waits_for_byte_led_num(void);
void GetSensorLED_reads_byte_led_num(void);
void GetSensorLED_sends_OK_and_LED_SETTING_if_led_num_is_valid(void);
void GetSensorLED_sends_ERROR_and_pads_second_byte_if_led_num_is_invalid(void);
// SetSensorLED()
void SetSensorLED_waits_for_byte_led_num(void);
void SetSensorLED_reads_byte_led_num(void);
void SetSensorLED_waits_for_byte_led_setting(void);
void SetSensorLED_reads_byte_led_setting(void);
void SetSensorLED_sends_ERROR_if_led_num_is_invalid(void);
void SetSensorLED_sends_ERROR_if_led_setting_is_invalid(void);
void SetSensorLED_applies_LED_setting_if_valid(void);
void SetSensorLED_sends_OK_if_num_and_setting_are_valid(void);
// LedNumIsValid()
void LedNumIsValid_returns_TRUE_if_led_num_is_0(void);
void LedNumIsValid_returns_TRUE_if_led_num_is_1(void);
void LedNumIsValid_returns_FALSE_if_led_num_is_not_0_or_1(void);
// LedSettingIsValid()
void LedSettingIsValid_returns_TRUE_if_led_setting_is_OFF(void);
void LedSettingIsValid_returns_TRUE_if_led_setting_is_GREEN(void);
void LedSettingIsValid_returns_TRUE_if_led_setting_is_RED(void);
void LedSettingIsValid_returns_FALSE_if_led_setting_is_not_0_1_or_2(void);

#endif // _TEST_VISCMD_H
