Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:60:BridgeGetSensorLED_reads_one_byte_of_host_payload:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:61:BridgeGetSensorLED_responds_ok_after_reading_host_payload:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x63.
test/test_runner.c:62:BridgeGetSensorLED_writes_led_number_to_Sensor:PASS
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0x01.
test/test_runner.c:63:BridgeGetSensorLED_reads_msg_status_byte_from_Sensor_and_sends_to_USB_host:PASS
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0000.
Call 6 is named ReadSensor? Call 6 is ReadSensor.
Call 7 is named SerialWriteByte? Call 7 is SerialWriteByte.
Value passed to call 7, arg 1? (uint8_t)0x02.
test/test_runner.c:64:BridgeGetSensorLED_reads_and_sends_led_status_byte_if_Sensor_status_is_ok:PASS
Call 4 is named ReadSensor? Call 4 is ReadSensor.
Call 5 is named SerialWriteByte? Call 5 is SerialWriteByte.
Value passed to call 5, arg 1? (uint8_t)0x01.
test/test_runner.c:65:BridgeGetSensorLED_reads_no_more_bytes_if_Sensor_status_is_error:PASS

-----------------------
6 Tests 0 Failures 0 Ignored 
OK
