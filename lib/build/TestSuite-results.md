test/test_runner.c:51:UsbRead_returns_false_if_there_was_no_data_read:FAIL: Change return type from bool to uint16_t.
test/test_runner.c:72:UsbRead_turns_LED_red_if_there_was_no_data_read:PASS
test/test_runner.c:95:UsbRead_returns_true_if_there_is_data_to_read:FAIL: Change return type from bool to uint16_t.
test/test_runner.c:128:UsbRead_should_read_until_buffer_is_empty:FAIL: Why it failed: Expected 7 calls, received 4 calls. First missed call: expected #5:'FtRead'. Wrong number of args in call #4 'FtRead', expected 1, was 0. Call #3: expected '0000', was '0000'. Call #4: expected 'FtRead', was 'FtDeactivateInterface'. 

-----------------------
4 Tests 3 Failures 0 Ignored 
FAIL
