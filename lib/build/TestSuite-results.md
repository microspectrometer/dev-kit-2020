test/test_runner.c:39:FtSetMisoAsInput_configures_MISO_as_an_input_pin:PASS
test/test_runner.c:40:FtEnablePullupOnMiso_enables_pullup_on_MISO:PASS
test/test_runner.c:41:FtSetMiosioAsInputs_configures_MIOSIO_as_an_input_port:PASS
test/test_runner.c:42:FtEnablePullupsOnMiosio_configures_pullups_on_MIOSIO:PASS
test/test_runner.c:43:FtSckLow_drives_SCK_low:PASS
test/test_runner.c:44:FtSsHigh_drives_SS_high:PASS
test/test_runner.c:47:FtHasDataToRead_returns_true_if_MISO_is_low:PASS
test/test_runner.c:48:FtHasDataToRead_returns_false_if_MISO_is_high:PASS
test/test_runner.c:49:FtHasRoomToWrite_returns_true_if_MIOSIO_bit_0_is_low:PASS
test/test_runner.c:50:FtHasRoomToWrite_returns_false_if_MIOSIO_bit_0_is_high:PASS
test/test_runner.c:53:FtActivateInterface_pulls_SS_low:PASS
test/test_runner.c:54:FtDeactivateInterface_pulls_SS_high:PASS
test/test_runner.c:55:FtPushData_pulls_SCK_high:PASS
test/test_runner.c:56:FtPullData_pulls_SCK_low:PASS
test/test_runner.c:57:FtLetMasterDriveBus_configures_MIOSIO_port_for_MCU_output:PASS
test/test_runner.c:58:FtLetSlaveDriveBus_configures_MIOSIO_port_for_MCU_input:PASS
test/test_runner.c:59:FtOutputByte_outputs_a_byte_on_port_MIOSIO:PASS
test/test_runner.c:60:FtReadData_returns_the_value_on_MIOSIO_pins:PASS
test/test_runner.c:61:FtWriteData_byte_outputs_byte_on_MIOSIO_pins:PASS
test/test_runner.c:62:FtIsBusOk_returns_true_if_MISO_is_low:PASS
test/test_runner.c:63:FtIsBusOk_returns_false_if_MISO_is_high:PASS

-----------------------
21 Tests 0 Failures 0 Ignored 
OK
