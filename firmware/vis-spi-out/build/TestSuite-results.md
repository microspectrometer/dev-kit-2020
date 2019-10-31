test/test_runner.c:61:SetSensorLED_receives_led_number_and_led_state_from_Bridge:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:62:SetSensorLED_replies_with_one_byte:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value at address passed to call 1, arg 1? (uint8_t *)0x01.
test/test_runner.c:63:SetSensorLED_replies_msg_status_error_if_led_number_is_not_valid:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value at address passed to call 1, arg 1? (uint8_t *)0x01.
test/test_runner.c:64:SetSensorLED_replies_msg_status_error_if_led_state_is_not_valid:PASS
Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
Value at address passed to call 1, arg 1? (uint8_t *)0000.
test/test_runner.c:65:SetSensorLED_replies_msg_status_ok_if_led_number_and_led_state_are_valid:PASS
test/test_runner.c:66:SetSensorLED_turns_off_led_if_payload_is_led_off:PASS
test/test_runner.c:67:SetSensorLED_turns_led_on_and_green_if_payload_is_led_green:PASS
test/test_runner.c:68:SetSensorLED_turns_led_on_and_red_if_payload_is_led_red:PASS

-----------------------
8 Tests 0 Failures 0 Ignored 
OK
