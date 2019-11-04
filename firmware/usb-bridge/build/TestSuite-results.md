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

-----------------------
7 Tests 0 Failures 0 Ignored 
OK
