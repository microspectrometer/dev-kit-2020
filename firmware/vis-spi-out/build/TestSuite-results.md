test/test_runner.c:46:GetSensorLED_receives_led_number:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0001.
WriteSpiMaster called with write_buffer[0] == 1
test/test_runner.c:47:GetSensorLED_replies_msg_status_error_if_led_does_not_exist:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0001.
WriteSpiMaster called with write_buffer[0] == 1
test/test_runner.c:48:GetSensorLED_sends_no_additional_bytes_if_msg_status_is_error:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
test/test_runner.c:49:GetSensorLED_replies_msg_status_ok_if_led_number_is_recognized:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
WriteSpiMaster called with write_buffer[1] == 2
test/test_runner.c:50:GetSensorLED_sends_led_status_byte_after_sending_msg_status_ok:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
WriteSpiMaster called with write_buffer[1] == 0
test/test_runner.c:51:GetSensorLED_replies_led_off_if_led_is_off:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
WriteSpiMaster called with write_buffer[1] == 1
test/test_runner.c:52:GetSensorLED_replies_led_green_if_led_is_green:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
WriteSpiMaster called with write_buffer[1] == 2
test/test_runner.c:53:GetSensorLED_replies_led_red_if_led_is_red:PASS

-----------------------
8 Tests 0 Failures 0 Ignored 
OK
