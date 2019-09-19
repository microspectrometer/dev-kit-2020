test/test_runner.c:188:SpiEnableInterrupt_enables_the_transfer_is_done_interrupt:PASS
test/test_runner.c:191:SpiSlaveInit_configures_pin_Miso_as_an_output:PASS
test/test_runner.c:192:SpiSlaveInit_makes_Miso_a_pullup_input_when_no_transfer_is_in_progress:PASS
test/test_runner.c:193:SpiSlaveInit_enables_the_SPI_hardware_module:PASS
test/test_runner.c:194:SpiSlaveInit_clears_pending_SPI_interrupt:PASS
test/test_runner.c:198:SpiSlaveSignalDataIsReady_pulls_Miso_low:PASS
test/test_runner.c:201:SpiSlaveRead_waits_until_transfer_is_done:PASS
test/test_runner.c:202:SpiSlaveRead_returns_the_SPI_data_register_byte:PASS
test/test_runner.c:205:SpiSlaveSendBytes_waits_for_master_read_between_each_byte:PASS

-----------------------
9 Tests 0 Failures 0 Ignored 
OK
