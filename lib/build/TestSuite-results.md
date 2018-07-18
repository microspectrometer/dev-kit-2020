test/test_runner.c:59:FtSendCommand_Read_does_entire_command_phase_for_ReadCmd:PASS
test/test_runner.c:60:FtSendCommand_Write_does_entire_command_phase_for_WriteCmd:PASS
test/test_runner.c:63:FtBusTurnaround_returns_true_if_ok_to_proceed_with_command:PASS
test/test_runner.c:64:FtBusTurnaround_returns_false_if_not_ok_to_proceed:PASS
test/test_runner.c:67:FtRead_should_return_false_if_MISO_is_NAK:PASS
test/test_runner.c:68:FtRead_should_not_write_to_input_buffer_if_MISO_is_NAK:PASS
test/test_runner.c:69:FtRead_should_return_true_if_MISO_is_ACK:PASS
test/test_runner.c:70:FtRead_should_write_to_input_buffer_if_MISO_is_ACK:PASS
test/test_runner.c:72:FtRead_sad_path_is_implemented_like_this:PASS
test/test_runner.c:73:FtRead_happy_path_is_implemented_like_this:PASS
test/test_runner.c:75:FtWrite_should_return_false_if_slave_sends_NAK:PASS
test/test_runner.c:76:FtWrite_should_return_true_if_slave_sends_ACK:PASS
test/test_runner.c:78:FtWrite_implements_the_happy_path_like_this:PASS

-----------------------
13 Tests 0 Failures 0 Ignored 
OK
