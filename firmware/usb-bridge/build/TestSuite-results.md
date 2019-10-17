Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:138:BridgeGetSensorLED_reads_one_byte_of_payload:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:139:BridgeGetSensorLED_responds_ok_after_reading_host_payload:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x02.
Call 4 is named ReadSensor? Call 4 is ReadSensor.
test/test_runner.c:140:BridgeGetSensorLED_passes_cmd_to_Sensor_and_waits_for_response:PASS
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0x01.
test/test_runner.c:141:BridgeGetSensorLED_passes_Sensor_command_response_back_to_host:PASS
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0x01.
test/test_runner.c:142:BridgeGetSensorLED_exits_if_Sensor_responds_error_to_command:PASS
Call 6 is named SpiWriteByte? Call 6 is SpiWriteByte.
Value passed to call 6, arg 1? (uint8_t)0000.
test/test_runner.c:143:BridgeGetSensorLED_sends_Sensor_led_number_if_Sensor_responds_ok_to_command:PASS
Call 7 is named ReadSensor? Call 7 is ReadSensor.
Value passed to call 7, arg 2? (uint16_t)0x0002.
Call 8 is named ReadSensor? Call 8 is ReadSensor.
Value passed to call 8, arg 2? (uint16_t)0x0001.
test/test_runner.c:144:BridgeGetSensorLED_reads_two_bytes_of_reply_size_and_nbytes_of_reply_from_Sensor:PASS
Call 9 is named SerialWriteByte? Call 9 is SerialWriteByte.
Value passed to call 9, arg 1? (uint8_t)0000.
Call 10 is named SerialWriteByte? Call 10 is SerialWriteByte.
Value passed to call 10, arg 1? (uint8_t)0x02.
Call 11 is named SerialWriteByte? Call 11 is SerialWriteByte.
Value passed to call 11, arg 1? (uint8_t)0000.
Call 12 is named SerialWriteByte? Call 12 is SerialWriteByte.
Value passed to call 12, arg 1? (uint8_t)0x02.
test/test_runner.c:145:BridgeGetSensorLED_passes_reply_size_and_reply_to_host:PASS

-----------------------
8 Tests 0 Failures 0 Ignored 
OK
