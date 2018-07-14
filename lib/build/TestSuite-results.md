test/test_runner.c:51:UsbRead_returns_false_if_there_was_no_data_read:FAIL: Change return type from bool to uint16_t.
test/test_runner.c:72:UsbRead_turns_LED_red_if_there_was_no_data_read:PASS
test/test_runner.c:95:UsbRead_returns_true_if_there_is_data_to_read:FAIL: Change return type from bool to uint16_t.
test/test_runner.c:127:UsbRead_should_read_until_buffer_is_empty:FAIL: TODO: mock a list of FtRead return values and test UsbRead for the number of bytes read.

-----------------------
4 Tests 3 Failures 0 Ignored 
FAIL
