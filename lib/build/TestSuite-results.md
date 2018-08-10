test/test_runner.c:176:SpiEnableInterrupt_enables_the_transfer_is_done_interrupt:PASS
test/test_runner.c:179:SpiSlaveInit_configures_pin_Miso_as_an_output:PASS
test/test_runner.c:180:SpiSlaveInit_makes_Miso_a_pullup_input_when_no_transfer_is_in_progress:PASS
test/test_runner.c:181:SpiSlaveInit_enables_the_SPI_hardware_module:PASS
test/test_runner.c:182:SpiSlaveInit_clears_pending_SPI_interrupt:PASS
test/test_runner.c:186:SpiSlaveSignalDataIsReady_pulls_Miso_low:PASS
test/test_runner.c:189:SpiSlaveRead_waits_until_transfer_is_done:PASS
test/test_runner.c:190:SpiSlaveRead_returns_the_SPI_data_register_byte:PASS
test/test_runner.c:193:SpiSlaveSendBytes_loads_each_byte_into_its_tx_buffer:PASS

-----------------------
9 Tests 0 Failures 0 Ignored 
OK
