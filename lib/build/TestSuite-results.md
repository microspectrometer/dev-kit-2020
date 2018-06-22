test/test_runner.c:39:FtHasDataToRead_returns_true_if_MISO_is_low:PASS
test/test_runner.c:40:FtHasDataToRead_returns_false_if_MISO_is_high:PASS
test/test_runner.c:41:FtHasRoomToWrite_returns_true_if_MIOSIO_bit_0_is_low:PASS
test/test_runner.c:42:FtHasRoomToWrite_returns_false_if_MIOSIO_bit_0_is_high:PASS
test/test_runner.c:44:FtActivateInterface_pulls_SS_low:PASS
test/test_runner.c:45:FtDeactivateInterface_pulls_SS_high:PASS
test/test_runner.c:46:FtPushData_pulls_SCK_high:PASS
test/test_runner.c:47:FtPullData_pulls_SCK_low:PASS
test/test_runner.c:48:FtLetMasterDriveBus_configures_MIOSIO_port_for_MCU_output:PASS
test/test_runner.c:49:FtLetSlaveDriveBus_configures_MIOSIO_port_for_MCU_input:PASS
test/test_runner.c:50:FtOutputByte_outputs_a_byte_on_port_MIOSIO:PASS
test/test_runner.c:51:FtReadData_returns_the_value_on_MIOSIO_pins:PASS
test/test_runner.c:52:FtIsBusOk_returns_true_if_MISO_is_low:PASS
test/test_runner.c:53:FtIsBusOk_returns_false_if_MISO_is_high:PASS
test/test_runner.c:57:FtSendCommand_Read_does_entire_command_phase_for_ReadCmd:PASS
test/test_runner.c:60:FtBusTurnaround_returns_true_if_ok_to_proceed_with_command:PASS
test/test_runner.c:61:FtBusTurnaround_returns_false_if_not_ok_to_proceed:PASS
test/test_runner.c:64:FtRead_does_not_write_to_mem_and_returns_false_if_NAK:PASS
test/test_runner.c:65:FtRead_should_write_to_mem_and_return_true_if_ACK:PASS

-----------------------
19 Tests 0 Failures 0 Ignored 
OK
