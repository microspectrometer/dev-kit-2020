test/test_runner.c:60:GetSensorLED_receives_led_number:PASS
If LED number is not valid:
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
If LED number is valid:
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
test/test_runner.c:61:GetSensorLED_always_replies_with_two_bytes:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 1
test/test_runner.c:62:GetSensorLED_replies_msg_status_error_if_led_is_non_existent:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
test/test_runner.c:63:GetSensorLED_replies_msg_status_ok_if_led_number_is_recognized:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
WriteSpiMaster called with write_buffer[1] == 0
test/test_runner.c:64:GetSensorLED_replies_led_off_if_led_is_off:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
WriteSpiMaster called with write_buffer[1] == 1
test/test_runner.c:65:GetSensorLED_replies_led_green_if_led_is_green:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0002.
WriteSpiMaster called with write_buffer[0] == 0
WriteSpiMaster called with write_buffer[1] == 2
test/test_runner.c:66:GetSensorLED_replies_led_red_if_led_is_red:PASS

-----------------------
7 Tests 0 Failures 0 Ignored 
OK
