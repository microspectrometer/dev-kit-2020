#ifndef _TEST_VISCMD_H
#define _TEST_VISCMD_H

/* =====[ LisReadout ]===== */
void LisReadout_waits_for_Lis_Sync_to_go_HIGH_then_go_LOW(void);
void LisReadout_reads_one_pixel_on_each_rising_edge_of_Lis_Clk(void);
void LisReadout_LOOP_wait_for_the_rising_edge_of_Lis_Clk(void);
void LisReadout_LOOP_start_the_ADC_conversion(void);
void LisReadout_LOOP_wait_for_45_cycles_of_10MHz_clock(void);
void LisReadout_LOOP_start_ADC_readout(void);
void LisReadout_LOOP_wait_for_most_significant_byte_ADC_readout(void);
void LisReadout_LOOP_save_MSB_to_frame_buffer(void);
void LisReadout_LOOP_wait_for_least_significant_byte_ADC_readout(void);
void LisReadout_LOOP_save_LSB_to_frame_buffer(void);

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
// GetSensorConfig()
void GetSensorConfig_sends_OK(void);
void GetSensorConfig_sends_binning(void);
void GetSensorConfig_sends_gain(void);
void GetSensorConfig_sends_row_bitmap(void);
// GetExposure()
void GetExposure_sends_OK(void);
void GetExposure_sends_MSB_of_exposure_ticks(void);
void GetExposure_sends_LSB_of_exposure_ticks(void);
// SetExposure()
void SetExposure_waits_for_byte_exposure_MSB(void);
void SetExposure_reads_byte_exposure_MSB(void);
void SetExposure_waits_for_byte_exposure_LSB(void);
void SetExposure_reads_byte_exposure_LSB(void);
void SetExposure_updates_global_exposure_ticks(void);
void SetExposure_sends_OK(void);
// CaptureFrame()
void CaptureFrame_sends_OK(void);
void CaptureFrame_checks_binning_to_determine_number_of_pixels_in_frame(void);
void CaptureFrame_sends_num_pixels_MSB(void);
void CaptureFrame_sends_num_pixels_LSB(void);
void CaptureFrame_exposes_the_pixels(void);
void CaptureFrame_does_readout_of_num_pixels_into_the_frame_buffer(void);
void CaptureFrame_sends_the_pixel_readings_stored_in_the_frame_buffer(void);
// GetPeak(start_pixel, stop_pixel) -> peak_counts
void GetPeak_is_visible(void);
// AutoExpose(start_pixel, stop_pixel)
void AutoExpose_is_visible(void);

#endif // _TEST_VISCMD_H
