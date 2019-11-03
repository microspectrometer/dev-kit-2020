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
Value passed to call 5, arg 1? (uint8_t)0x01.
Call 6 is named SerialWriteByte? Call 6 is SerialWriteByte.
Value passed to call 6, arg 1? (uint8_t)0x01.
Call 7 is named SerialWriteByte? Call 7 is SerialWriteByte.
Value passed to call 7, arg 1? (uint8_t)0x1f.
test/test_runner.c:94:BridgeGetSensorConfig_if_status_is_ok_then_read_config_data_from_Sensor_and_send_to_USB_host:PASS

-----------------------
4 Tests 0 Failures 0 Ignored 
OK
