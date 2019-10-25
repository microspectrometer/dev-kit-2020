Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:59:BridgeGetSensorLED_reads_one_byte_of_host_payload:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:60:BridgeGetSensorLED_responds_ok_after_reading_host_payload:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x03.
Call 4 is named SpiWriteByte? Call 4 is SpiWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x63.
test/test_runner.c:61:BridgeGetSensorLED_writes_cmd_and_payload_to_Sensor:PASS
Call 5 is named ReadSensor? Call 5 is ReadSensor.
Value passed to call 5, arg 2? (uint16_t)0x0002.
test/test_runner.c:62:BridgeGetSensorLED_reads_two_bytes_of_reply_from_Sensor:PASS
Call 6 is named SerialWriteByte? Call 6 is SerialWriteByte.
Value passed to call 6, arg 1? (uint8_t)0000.
Call 7 is named SerialWriteByte? Call 7 is SerialWriteByte.
Value passed to call 7, arg 1? (uint8_t)0x01.
test/test_runner.c:63:BridgeGetSensorLED_writes_sensor_reply_to_host:PASS

-----------------------
5 Tests 0 Failures 0 Ignored 
OK
