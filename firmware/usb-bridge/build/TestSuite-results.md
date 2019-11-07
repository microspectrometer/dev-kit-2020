Call 1 is named UsbWrite? Call 1 is UsbWrite.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:155:SerialWriteByte_writes_one_byte_over_USB:PASS
test/test_runner.c:162:LookupBridgeCmd_takes_key_and_returns_fn_ptr:PASS
test/test_runner.c:163:LookupBridgeCmd_returns_NULL_if_key_is_not_found:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
Value passed to call 4, arg 1? (uint8_t)0000.
test/test_runner.c:31:GetBridgeLED_reads_and_ignores_Sensor_reply_to_GetBridgeLED:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:32:GetBridgeLED_receives_led_number:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:33:GetBridgeLED_replies_msg_status_error_if_led_does_not_exist:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:34:GetBridgeLED_sends_no_additional_bytes_if_msg_status_is_error:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
test/test_runner.c:35:GetBridgeLED_replies_msg_status_ok_if_led_number_is_recognized:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x01.
test/test_runner.c:36:GetBridgeLED_sends_led_status_byte_after_sending_msg_status_ok:PASS
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
Value passed to call 4, arg 1? (uint8_t)0000.
test/test_runner.c:37:GetBridgeLED_replies_led_off_if_led_is_off:PASS
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x01.
test/test_runner.c:38:GetBridgeLED_replies_led_green_if_led_is_green:PASS
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x02.
test/test_runner.c:39:GetBridgeLED_replies_led_red_if_led_is_red:PASS
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
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:61:BridgeGetSensorLED_reads_one_byte_of_host_payload:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
test/test_runner.c:62:BridgeGetSensorLED_checks_for_invalid_command_error_from_Sensor:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
test/test_runner.c:63:BridgeGetSensorLED_does_not_send_payload_if_Sensor_says_invalid_cmd:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x02.
test/test_runner.c:64:BridgeGetSensorLED_passes_invalid_cmd_reply_back_to_host:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:65:BridgeGetSensorLED_responds_ok_if_Sensor_does_not_say_invalid_cmd:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x63.
test/test_runner.c:66:BridgeGetSensorLED_writes_led_number_to_Sensor:PASS
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0x01.
test/test_runner.c:67:BridgeGetSensorLED_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host:PASS
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0000.
Call 6 is named ReadSensor? Call 6 is ReadSensor.
Call 7 is named SerialWriteByte? Call 7 is SerialWriteByte.
Value passed to call 7, arg 1? (uint8_t)0x02.
test/test_runner.c:68:BridgeGetSensorLED_reads_and_sends_led_status_byte_if_Sensor_status_is_ok:PASS
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0x01.
test/test_runner.c:69:BridgeGetSensorLED_reads_no_more_bytes_if_Sensor_status_is_error:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0002.
test/test_runner.c:77:BridgeSetSensorLED_reads_two_bytes_of_host_payload:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
test/test_runner.c:78:BridgeSetSensorLED_checks_for_invalid_command_error_from_Sensor:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
test/test_runner.c:79:BridgeSetSensorLED_does_not_send_payload_if_Sensor_says_invalid_cmd:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x02.
test/test_runner.c:80:BridgeSetSensorLED_passes_invalid_cmd_reply_back_to_host:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:81:BridgeSetSensorLED_responds_ok_if_Sensor_does_not_say_invalid_cmd:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
Call 4 is named SpiWriteByte? Call 4 is SpiWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x02.
test/test_runner.c:82:BridgeSetSensorLED_passes_two_bytes_of_payload_to_Sensor:PASS
Call 5 is named ReadSensor? Call 5 is ReadSensor.
Value passed to call 5, arg 2? (uint16_t)0x0001.
Call 6 is named SerialWriteByte? Call 6 is SerialWriteByte.
Value passed to call 6, arg 1? (uint8_t)0000.
test/test_runner.c:83:BridgeSetSensorLED_reads_and_sends_one_byte_Sensor_reply_to_host:PASS
Call 1 is named SerialWriteByte? Call 1 is SerialWriteByte.
Value passed to call 1, arg 1? (uint8_t)0000.
test/test_runner.c:91:BridgeGetSensorConfig_replies_ok_to_USB_host:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:92:BridgeGetSensorConfig_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:93:BridgeGetSensorConfig_reads_no_more_bytes_if_Sensor_status_is_error:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Value passed to call 4, arg 2? (uint16_t)0x0003.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0000.
Call 6 is named SerialWriteByte? Call 6 is SerialWriteByte.
Value passed to call 6, arg 1? (uint8_t)0000.
Call 7 is named SerialWriteByte? Call 7 is SerialWriteByte.
Value passed to call 7, arg 1? (uint8_t)0000.
test/test_runner.c:94:BridgeGetSensorConfig_if_status_is_ok_then_read_config_data_from_Sensor_and_send_to_USB_host:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0003.
test/test_runner.c:102:BridgeSetSensorConfig_reads_three_bytes_of_host_payload:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:103:BridgeSetSensorConfig_sends_msg_status_ok_after_reading_payload:PASS
Call 3 is named ReadSensor? Call 3 is ReadSensor.
test/test_runner.c:104:BridgeSetSensorConfig_checks_for_invalid_command_error_from_Sensor:PASS
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
test/test_runner.c:105:BridgeSetSensorConfig_does_not_send_payload_if_Sensor_says_invalid_cmd:PASS
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x02.
test/test_runner.c:106:BridgeSetSensorConfig_passes_invalid_cmd_reply_back_to_host:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
Call 4 is named SpiWriteByte? Call 4 is SpiWriteByte.
Value passed to call 4, arg 1? (uint8_t)0000.
Call 5 is named SpiWriteByte? Call 5 is SpiWriteByte.
Value passed to call 5, arg 1? (uint8_t)0000.
test/test_runner.c:107:BridgeSetSensorConfig_passes_three_bytes_of_payload_to_Sensor_if_it_does_not_say_invalid_cmd:PASS
Call 6 is named ReadSensor? Call 6 is ReadSensor.
Value passed to call 6, arg 2? (uint16_t)0x0001.
Call 7 is named SerialWriteByte? Call 7 is SerialWriteByte.
Value passed to call 7, arg 1? (uint8_t)0000.
test/test_runner.c:108:BridgeSetSensorConfig_reads_and_sends_one_byte_Sensor_reply_to_host:PASS
Call 1 is named SerialWriteByte? Call 1 is SerialWriteByte.
Value passed to call 1, arg 1? (uint8_t)0000.
test/test_runner.c:116:BridgeGetExposure_replies_ok_to_USB_host:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
Value passed to call 2, arg 2? (uint16_t)0x0001.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x02.
test/test_runner.c:117:BridgeGetExposure_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x02.
test/test_runner.c:118:BridgeGetExposure_reads_no_more_bytes_if_Sensor_status_is_error:PASS
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Value passed to call 4, arg 2? (uint16_t)0x0002.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0000.
Call 6 is named SerialWriteByte? Call 6 is SerialWriteByte.
Value passed to call 6, arg 1? (uint8_t)0x32.
test/test_runner.c:119:BridgeGetExposure_reads_two_bytes_of_exposure_time_from_Sensor_MSB_first_and_sends_to_USB_host:PASS
Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0002.
test/test_runner.c:127:BridgeSetExposure_reads_two_bytes_of_payload_from_usb_host:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:128:BridgeSetExposure_sends_msg_status_ok_to_usb_host:PASS
Call 3 is named ReadSensor? Call 3 is ReadSensor.
test/test_runner.c:129:BridgeSetExposure_checks_for_invalid_command_error_from_Sensor:PASS
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
test/test_runner.c:130:BridgeSetExposure_does_not_send_payload_if_Sensor_says_invalid_cmd:PASS
Call 4 is named SerialWriteByte? Call 4 is SerialWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x02.
test/test_runner.c:131:BridgeSetExposure_passes_invalid_cmd_reply_back_to_host:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
Call 4 is named SpiWriteByte? Call 4 is SpiWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x32.
test/test_runner.c:132:BridgeSetExposure_passes_two_bytes_of_payload_to_Sensor_if_it_does_not_say_invalid_cmd:PASS
Call 5 is named ReadSensor? Call 5 is ReadSensor.
Value passed to call 5, arg 2? (uint16_t)0x0001.
Call 6 is named SerialWriteByte? Call 6 is SerialWriteByte.
Value passed to call 6, arg 1? (uint8_t)0000.
test/test_runner.c:133:BridgeSetExposure_reads_and_sends_one_byte_Sensor_reply_to_host:PASS
Call 1 is named SerialWriteByte? Call 1 is SerialWriteByte.
Value passed to call 1, arg 1? (uint8_t)0000.
test/test_runner.c:141:BridgeCaptureFrame_sends_msg_status_ok_to_usb_host:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:142:BridgeCaptureFrame_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
test/test_runner.c:143:BridgeCaptureFrame_reads_no_more_bytes_if_Sensor_status_is_error:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0000.
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Value passed to call 4, arg 2? (uint16_t)0x0002.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0x01.
Call 6 is named SerialWriteByte? Call 6 is SerialWriteByte.
Value passed to call 6, arg 1? (uint8_t)0x88.
test/test_runner.c:144:BridgeCaptureFrame_reads_npixels_in_frame_and_sends_to_USB_host:PASS
Call 8 is named SerialWriteByte? Call 8 is SerialWriteByte.
Value passed to call 8, arg 1? (uint8_t)0x01.
test/test_runner.c:145:BridgeCaptureFrame_reads_another_status_byte_from_Sensor_and_sends_to_USB_host:PASS
Call 8 is named SerialWriteByte? Call 8 is SerialWriteByte.
Value passed to call 8, arg 1? (uint8_t)0x01.
test/test_runner.c:146:BridgeCaptureFrame_does_not_read_frame_if_Sensor_status_is_error:PASS
Call 8 is named SerialWriteByte? Call 8 is SerialWriteByte.
Value passed to call 8, arg 1? (uint8_t)0000.
Call 9 is named ReadSensor? Call 9 is ReadSensor.
Value passed to call 9, arg 2? (uint16_t)0x0310.
Call 10 is named SerialWriteByte? Call 10 is SerialWriteByte.
Call 793 is named SerialWriteByte? Call 793 is SerialWriteByte.
test/test_runner.c:147:BridgeCaptureFrame_reads_and_sends_frame_if_Sensor_status_is_ok:PASS

-----------------------
65 Tests 0 Failures 0 Ignored 
OK
