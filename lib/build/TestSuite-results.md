test/test_runner.c:42:FtActivateInterface_pulls_SS_low:PASS
test/test_runner.c:43:FtPushData_pulls_SCK_high:PASS
test/test_runner.c:44:FtLetMasterDriveBus_configures_MIOSIO_port_for_MCU_output:PASS
test/test_runner.c:45:FtOutputByte_outputs_a_byte_on_port_MIOSIO:PASS
test/test_runner.c:46:FtPullData_pulls_SCK_low:PASS
test/test_runner.c:49:FtSendCommand_Read_does_entire_command_phase_for_ReadCmd:PASS
test/test_runner.c:52:FtBusTurnaround_handles_the_entire_bus_turnaround:PASS
test/test_runner.c:53:FtBusTurnaround_returns_false_if_not_ok_to_proceed:PASS
test/test_runner.c:56:FtRead_reads_bytes_from_MIOSIO:PASS
test/test_runner.c:59:FtHasDataToRead_returns_true_if_MISO_is_low:PASS
test/test_runner.c:60:FtHasDataToRead_returns_false_if_MISO_is_high:PASS
test/test_runner.c:61:FtHasRoomToWrite_returns_true_if_MIOSIO_bit_0_is_low:PASS
test/test_runner.c:62:FtLetSlaveDriveBus_configures_MIOSIO_port_for_MCU_input:PASS
test/test_runner.c:63:FtIsBusOk_returns_true_if_MISO_is_low:PASS
test/test_runner.c:64:FtIsBusOk_returns_false_if_MISO_is_high:PASS
test/test_runner.c:65:FtReadData_returns_the_value_on_MIOSIO_pins:PASS
test/test_runner.c:66:FtDeactivateInterface_pulls_SS_high:PASS

-----------------------
17 Tests 0 Failures 0 Ignored 
OK
