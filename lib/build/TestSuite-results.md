test/test_runner.c:67:FtSendCommand_Read_does_entire_command_phase_for_ReadCmd:PASS
test/test_runner.c:68:FtSendCommand_Write_does_entire_command_phase_for_WriteCmd:PASS
test/test_runner.c:71:FtBusTurnaround_returns_true_if_ok_to_proceed_with_command:PASS
test/test_runner.c:72:FtBusTurnaround_returns_false_if_not_ok_to_proceed:PASS
test/test_runner.c:75:FtRead_should_return_false_if_MISO_is_NAK:PASS
test/test_runner.c:76:FtRead_should_not_write_to_input_buffer_if_MISO_is_NAK:PASS
test/test_runner.c:77:FtRead_should_return_true_if_MISO_is_ACK:PASS
test/test_runner.c:78:FtRead_should_write_to_input_buffer_if_MISO_is_ACK:PASS
test/test_runner.c:80:FtRead_sad_path_is_implemented_like_this:PASS
test/test_runner.c:81:FtRead_happy_path_is_implemented_like_this:PASS
test/test_runner.c:83:FtWrite_should_return_false_if_slave_sends_NAK:PASS
test/test_runner.c:84:FtWrite_should_return_true_if_slave_sends_ACK:PASS
test/test_runner.c:86:FtWrite_implements_the_happy_path_like_this:PASS
test/test_runner.c:119:UsbWrite_happy_path_is_implemented_like_this:PASS
test/test_runner.c:120:UsbWrite_sad_path_is_implemented_like_this:PASS

-----------------------
15 Tests 0 Failures 0 Ignored 
OK
