Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:31:GetBridgeLED_reads_one_byte_of_payload:PASS
Call 2 is named SendStatus? Call 2 is SendStatus.
Value passed to call 2, arg 1? (uint8_t)0x01.
test/test_runner.c:32:GetBridgeLED_replies_msg_status_error_if_host_queries_nonexistent_led:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Call 2 is named SendStatus? Call 2 is SendStatus.
test/test_runner.c:33:GetBridgeLED_replies_with_one_byte_if_led_number_is_not_recognized:PASS
Call 2 is named SendStatus? Call 2 is SendStatus.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:34:GetBridgeLED_replies_msg_status_ok_if_host_queries_status_led:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Call 2 is named SendStatus? Call 2 is SendStatus.
Call 3 is named SendStatus? Call 3 is SendStatus.
test/test_runner.c:35:GetBridgeLED_replies_with_two_bytes_if_led_number_is_recognized:PASS
Call 2 is named SendStatus? Call 2 is SendStatus.
Value passed to call 2, arg 1? (uint8_t)0000.
Call 3 is named SendStatus? Call 3 is SendStatus.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:36:GetBridgeLED_replies_with_msg_status_byte_and_led_status_byte:PASS
Call 3 is named SendStatus? Call 3 is SendStatus.
Value passed to call 3, arg 1? (uint8_t)0000.
test/test_runner.c:37:GetBridgeLED_replies_led_off_if_status_led_is_off:PASS
Call 3 is named SendStatus? Call 3 is SendStatus.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:38:GetBridgeLED_replies_led_green_if_status_led_is_green:PASS
Call 3 is named SendStatus? Call 3 is SendStatus.
Value passed to call 3, arg 1? (uint8_t)0x02.
test/test_runner.c:39:GetBridgeLED_replies_led_red_if_status_led_is_red:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0002.
test/test_runner.c:46:SetBridgeLED_reads_two_bytes_of_payload:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Call 2 is named SendStatus? Call 2 is SendStatus.
test/test_runner.c:47:SetBridgeLED_replies_with_one_byte:PASS
Call 2 is named SendStatus? Call 2 is SendStatus.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:48:SetBridgeLED_replies_msg_status_ok_if_led_number_is_status_led:PASS
Call 2 is named SendStatus? Call 2 is SendStatus.
Value passed to call 2, arg 1? (uint8_t)0x01.
test/test_runner.c:49:SetBridgeLED_replies_msg_status_error_if_led_number_is_not_recognized:PASS
test/test_runner.c:50:SetBridgeLED_turns_off_led_if_payload_is_led_off:PASS
test/test_runner.c:51:SetBridgeLED_turns_led_on_and_green_if_payload_is_led_green:PASS
test/test_runner.c:52:SetBridgeLED_turns_led_on_and_red_if_payload_is_led_red:PASS

-----------------------
16 Tests 0 Failures 0 Ignored 
OK
