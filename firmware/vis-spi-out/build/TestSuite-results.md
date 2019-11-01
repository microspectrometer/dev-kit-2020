test/test_runner.c:35:test_lib_SensorVis_can_use_lib_Queue_and_sees_SpiFifo:PASS
test/test_runner.c:36:test_Queue_lib_examples_of_push_and_pop:PASS
test/test_runner.c:37:test_Queue_lib_example_of_push_wrap_around:PASS
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
19 Tests 0 Failures 0 Ignored 
OK
