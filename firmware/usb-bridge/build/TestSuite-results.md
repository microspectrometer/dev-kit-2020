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
7 Tests 0 Failures 0 Ignored 
OK
