Call 1 is named WriteSpiMaster? Call 1 is WriteSpiMaster.
test/test_runner.c:135:CaptureFrame_sends_status_byte_ok:PASS
Call 2 is named GetFrame? Call 2 is GetFrame.
test/test_runner.c:136:CaptureFrame_collects_a_frame_of_pixel_data:PASS
Call 3 is named WriteSpiMaster? Call 3 is WriteSpiMaster.
Number of pixels is 392 = 0x188.
Value passed to call 3, arg 2? (uint16_t)0x0002.
Expected msb is 0x01
Expected lsb is 0x88
test/test_runner.c:137:CaptureFrame_sends_two_bytes_msb_first_with_number_of_pixels_in_frame:PASS
Call 4 is named WriteSpiMaster? Call 4 is WriteSpiMaster.
test/test_runner.c:138:CaptureFrame_sends_another_status_byte_ok:PASS
Call 5 is named WriteSpiMaster? Call 5 is WriteSpiMaster.
Number of pixels is 392 = 0x188. Number of bytes in frame is 784 = 0x310.
Value passed to call 5, arg 2? (uint16_t)0x0310.
test/test_runner.c:139:CaptureFrame_sends_the_frame_as_two_bytes_per_pixel_msb_first:PASS

-----------------------
5 Tests 0 Failures 0 Ignored 
OK
