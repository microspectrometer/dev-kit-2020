Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:31:GetBridgeLED_receives_led_number:PASS
Valid LED number:
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Invalid LED number:
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
test/test_runner.c:32:GetBridgeLED_always_replies_with_two_bytes:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:33:GetBridgeLED_replies_msg_status_ok_if_led_number_is_recognized:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0x01.
test/test_runner.c:34:GetBridgeLED_replies_msg_status_error_if_led_is_non_existent:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
test/test_runner.c:35:GetBridgeLED_replies_led_off_if_led_is_off:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:36:GetBridgeLED_replies_led_green_if_led_is_green:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x02.
test/test_runner.c:37:GetBridgeLED_replies_led_red_if_led_is_red:PASS

-----------------------
7 Tests 0 Failures 0 Ignored 
OK
