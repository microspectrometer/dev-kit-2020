test/test_runner.c:199:UartSpiInit_loads_0_into_baud_rate_register:PASS
test/test_runner.c:200:UartSpiInit_sets_Sck_as_an_output:PASS
test/test_runner.c:201:UartSpiInit_sets_AdcConv_to_idle_low:PASS
test/test_runner.c:202:UartSpiInit_sets_AdcConv_as_an_output:PASS
test/test_runner.c:203:UartSpiInit_enables_the_UART_in_Master_SPI_Mode:PASS
test/test_runner.c:204:UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1:PASS
test/test_runner.c:205:UartSpiInit_cfgs_SPI_to_transfer_MSB_first:PASS
test/test_runner.c:206:UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior:PASS
test/test_runner.c:208:UartSpiTransferIsDone_returns_true_when_the_transfer_is_done:PASS
test/test_runner.c:209:UartSpiRead_initiates_adc_conversion_and_readout:PASS
test/test_runner.c:210:UartSpiRead_does_a_16bit_SPI_transfer_with_the_ADC:PASS
test/test_runner.c:211:UartSpiRead_writes_the_16bit_adc_reading_to_the_input_address:PASS
test/test_runner.c:213:StoreAdcReadingsInFrame_fills_an_array_with_adc_readings:PASS

-----------------------
13 Tests 0 Failures 0 Ignored 
OK
