Call 1 is named UsbReadBytes? Call 1 is UsbReadBytes.
Value passed to call 1, arg 2? (uint16_t)0x0001.
test/test_runner.c:153:GetSensorLED_reads_one_byte_of_payload:PASS
Call 2 is named WriteSensor? Call 2 is WriteSensor.
Value passed to call 2, arg 2? (uint16_t)0x0002.
WriteSensor called with write_buffer[0] == 2
WriteSensor called with write_buffer[1] == 0
test/test_runner.c:154:GetSensorLED_passes_cmd_and_led_number_to_Sensor:PASS
Call 5 is named SendStatus? Call 5 is SendStatus.
Value passed to call 5, arg 1? (uint8_t)0x01.
Call 6 is named SendStatus? Call 6 is SendStatus.
Value passed to call 6, arg 1? (uint8_t)0x02.
test/test_runner.c:155:GetSensorLED_reads_msg_status_and_led_status_from_Sensor:PASS

-----------------------
3 Tests 0 Failures 0 Ignored 
OK
