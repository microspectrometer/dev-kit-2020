Call 1 is named UsbReadN? Call 1 is UsbReadN.
Value passed to call 1, arg 2? (uint16_t)0x0002.
test/test_runner.c:140:SetBridgeLED_reads_two_bytes_of_payload:PASS
Call 1 is named UsbReadN? Call 1 is UsbReadN.
Call 2 is named SendStatus? Call 2 is SendStatus.
test/test_runner.c:141:SetBridgeLED_replies_with_one_byte:PASS
Call 2 is named SendStatus? Call 2 is SendStatus.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:142:SetBridgeLED_replies_msg_status_ok_if_led_number_is_status_led:PASS
Call 2 is named SendStatus? Call 2 is SendStatus.
Value passed to call 2, arg 1? (uint8_t)0x01.
test/test_runner.c:143:SetBridgeLED_replies_msg_status_error_if_led_number_is_not_recognized:PASS
test/test_runner.c:144:SetBridgeLED_turns_off_led_if_payload_is_led_off:PASS
test/test_runner.c:145:SetBridgeLED_turns_led_on_and_green_if_payload_is_led_green:PASS
test/test_runner.c:146:SetBridgeLED_turns_led_on_and_red_if_payload_is_led_red:PASS

-----------------------
7 Tests 0 Failures 0 Ignored 
OK
