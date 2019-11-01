Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0003.
test/test_runner.c:126:BridgeSetSensorConfig_reads_three_bytes_of_host_payload:PASS
Call 2 is named ReadSensor? Call 2 is ReadSensor.
test/test_runner.c:127:BridgeSetSensorConfig_checks_for_invalid_command_error_from_Sensor:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
test/test_runner.c:128:BridgeSetSensorConfig_does_not_send_payload_if_Sensor_says_invalid_cmd:PASS
Call 3 is named SerialWriteByte? Call 3 is SerialWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x02.
test/test_runner.c:129:BridgeSetSensorConfig_passes_invalid_cmd_reply_back_to_host:PASS
Call 2 is named SerialWriteByte? Call 2 is SerialWriteByte.
Value passed to call 2, arg 1? (uint8_t)0000.
test/test_runner.c:130:BridgeSetSensorConfig_responds_ok_if_Sensor_does_not_say_invalid_cmd:PASS
Call 3 is named SpiWriteByte? Call 3 is SpiWriteByte.
Value passed to call 3, arg 1? (uint8_t)0x01.
Call 4 is named SpiWriteByte? Call 4 is SpiWriteByte.
Value passed to call 4, arg 1? (uint8_t)0x01.
Call 5 is named SpiWriteByte? Call 5 is SpiWriteByte.
Value passed to call 5, arg 1? (uint8_t)0x1f.
test/test_runner.c:131:BridgeSetSensorConfig_passes_three_bytes_of_payload_to_Sensor:PASS
Call 6 is named ReadSensor? Call 6 is ReadSensor.
Value passed to call 6, arg 2? (uint16_t)0x0001.
Call 7 is named SerialWriteByte? Call 7 is SerialWriteByte.
Value passed to call 7, arg 1? (uint8_t)0000.
test/test_runner.c:132:BridgeSetSensorConfig_reads_and_sends_one_byte_Sensor_reply_to_host:PASS

-----------------------
7 Tests 0 Failures 0 Ignored 
OK
