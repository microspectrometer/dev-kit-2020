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

#endif // _TEST_USBCMD_H
