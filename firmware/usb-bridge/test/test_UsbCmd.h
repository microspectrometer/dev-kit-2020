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

#endif // _TEST_USBCMD_H
