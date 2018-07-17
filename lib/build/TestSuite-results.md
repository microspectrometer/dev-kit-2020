test/test_runner.c:58:FtSendCommand_Read_does_entire_command_phase_for_ReadCmd:PASS
test/test_runner.c:61:FtBusTurnaround_returns_true_if_ok_to_proceed_with_command:PASS
test/test_runner.c:62:FtBusTurnaround_returns_false_if_not_ok_to_proceed:PASS
test/test_runner.c:65:FtRead_does_not_write_to_mem_and_returns_false_if_NAK:PASS
test/test_runner.c:66:FtRead_should_write_to_mem_and_return_true_if_ACK:PASS
test/test_runner.c:71:UsbRead_should_return_0_if_there_was_no_data_to_read:PASS
test/test_runner.c:72:UsbRead_turns_LED_red_if_there_was_no_data_read:PASS
test/test_runner.c:73:UsbRead_returns_N_if_there_were_N_bytes_to_read:PASS
test/test_runner.c:74:UsbRead_should_read_until_buffer_is_empty:PASS
test/test_runner.c:75:UsbRead_copies_bytes_to_the_input_read_buffer_address:PASS

-----------------------
10 Tests 0 Failures 0 Ignored 
OK
