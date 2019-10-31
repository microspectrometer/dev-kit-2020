Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0002.
test/test_runner.c:74:BridgeSetSensorLED_reads_two_bytes_of_host_payload:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:75:BridgeSetSensorLED_responds_ok_after_reading_host_payload:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
Call 4 is named SpiWriteByte? Call 4 is SpiWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x02.
test/test_runner.c:76:BridgeSetSensorLED_passes_two_bytes_of_payload_to_Sensor:PASS
Call 5 is named ReadSensor? Call 5 is ReadSensor.
Value passed to call 5, arg 2? (uint16_t)0x0001.
Call 6 is named SerialWriteByte? Call 6 is SerialWriteByte.
Value passed to call 6, arg 1? (uint8_t)0000.
test/test_runner.c:77:BridgeSetSensorLED_reads_and_sends_one_byte_Sensor_reply_to_host:PASS

-----------------------
4 Tests 0 Failures 0 Ignored 
OK
