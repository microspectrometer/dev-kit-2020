Call 2 is named ReadSensor? Call 2 is ReadSensor.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
test/test_runner.c:46:SetBridgeLED_reads_and_ignores_Sensor_reply_to_SetBridgeLED:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0002.
test/test_runner.c:47:SetBridgeLED_reads_two_bytes_of_payload_from_usb_host:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Call 2 is named ReadSensor? Call 2 is ReadSensor.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
test/test_runner.c:48:SetBridgeLED_replies_with_one_byte:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:49:SetBridgeLED_replies_msg_status_error_if_led_number_is_not_recognized:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
test/test_runner.c:50:SetBridgeLED_replies_msg_status_ok_if_led_number_is_status_led:PASS
test/test_runner.c:51:SetBridgeLED_turns_off_led_if_payload_is_led_off:PASS
test/test_runner.c:52:SetBridgeLED_turns_led_on_and_green_if_payload_is_led_green:PASS
test/test_runner.c:53:SetBridgeLED_turns_led_on_and_red_if_payload_is_led_red:PASS

-----------------------
8 Tests 0 Failures 0 Ignored 
OK
