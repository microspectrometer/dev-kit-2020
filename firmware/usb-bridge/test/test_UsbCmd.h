#ifndef _TEST_USBCMD_H
#define _TEST_USBCMD_H

/* =====[ GetBridgeLED ]===== */
// ReadLedState() -> led_state
// returns OFF, GREEN, or RED
void ReadLedState_returns_OFF_if_LED_is_off(void);
void ReadLedState_returns_GREEN_if_LED_is_on_and_green(void);
void ReadLedState_returns_RED_if_LED_is_on_and_red(void);
// GetBridgeLED()
void GetBridgeLED_waits_for_byte_led_num(void);
void GetBridgeLED_reads_byte_led_num(void);
void GetBridgeLED_writes_two_bytes_to_USB_host(void);
void GetBridgeLED_writes_OK_and_LED_STATE_if_led_num_is_valid(void);
void GetBridgeLED_writes_ERROR_and_pads_second_byte_if_led_num_is_invalid(void);
// SetBridgeLED()
void SetBridgeLED_waits_for_byte_led_num(void);
void SetBridgeLED_reads_byte_led_num(void);
void SetBridgeLED_waits_for_byte_led_setting(void);
void SetBridgeLED_reads_byte_led_setting(void);
void SetBridgeLED_applies_led_setting_and_writes_OK_if_parameters_are_valid(void);
void SetBridgeLED_writes_ERROR_if_led_num_is_invalid(void);
void SetBridgeLED_writes_ERROR_if_led_setting_is_invalid(void);
// GetSensorLED()
void GetSensorLED_waits_for_byte_led_num(void);
void GetSensorLED_reads_byte_led_num(void);
void GetSensorLED_sends_command_to_sensor(void);
void GetSensorLED_sends_led_num_to_sensor(void);
void GetSensorLED_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void);
void GetSensorLED_waits_for_sensor_to_signal_STATUS_data_ready(void);
void GetSensorLED_reads_status_from_sensor(void);
void GetSensorLED_waits_for_sensor_to_signal_LED_SETTING_data_ready(void);
void GetSensorLED_reads_led_setting_from_sensor(void);
void GetSensorLED_writes_sensor_status(void);
void GetSensorLED_writes_sensor_led_setting(void);
// SetSensorLED()
void SetSensorLED_waits_for_byte_led_num(void);
void SetSensorLED_reads_byte_led_num(void);
void SetSensorLED_waits_for_byte_led_setting(void);
void SetSensorLED_reads_byte_led_setting(void);
void SetSensorLED_sends_command_to_sensor(void);
void SetSensorLED_sends_led_num_to_sensor(void);
void SetSensorLED_sends_led_setting_to_sensor(void);
void SetSensorLED_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void);
void SetSensorLED_waits_for_sensor_to_signal_STATUS_data_ready(void);
void SetSensorLED_reads_status_from_sensor(void);
void SetSensorLED_writes_sensor_status(void);
// SetSensorConfig()
void SetSensorConfig_waits_for_byte_binning(void);
void SetSensorConfig_reads_byte_binning(void);
void SetSensorConfig_waits_for_byte_gain(void);
void SetSensorConfig_reads_byte_gain(void);
void SetSensorConfig_waits_for_byte_row_bitmap(void);
void SetSensorConfig_reads_byte_row_bitmap(void);
void SetSensorConfig_sends_command_to_sensor(void);
void SetSensorConfig_sends_binning_to_sensor(void);
void SetSensorConfig_sends_gain_to_sensor(void);
void SetSensorConfig_sends_row_bitmap_to_sensor(void);
void SetSensorConfig_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void);
void SetSensorConfig_waits_for_sensor_to_signal_STATUS_data_ready(void);
void SetSensorConfig_reads_status_from_sensor(void);
void SetSensorConfig_writes_sensor_status(void);

#endif // _TEST_USBCMD_H
