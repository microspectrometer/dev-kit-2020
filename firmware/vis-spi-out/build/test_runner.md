# test VisCmd
## VisCmd.LedNumIsValid
test/test_runner.c:53:LedNumIsValid_returns_TRUE_if_led_num_is_0:PASS
test/test_runner.c:54:LedNumIsValid_returns_TRUE_if_led_num_is_1:PASS
test/test_runner.c:55:LedNumIsValid_returns_FALSE_if_led_num_is_not_0_or_1:PASS
## VisCmd.LisReadout
test/test_runner.c:60:LisReadout_waits_for_Lis_Sync_to_go_HIGH_then_go_LOW:PASS
test/test_runner.c:61:LisReadout_reads_one_pixel_on_each_rising_edge_of_Lis_Clk:PASS
test/test_runner.c:62:LisReadout_LOOP_wait_for_the_rising_edge_of_Lis_Clk:PASS
test/test_runner.c:63:LisReadout_LOOP_start_the_ADC_conversion:PASS
test/test_runner.c:64:LisReadout_LOOP_wait_for_45_cycles_of_10MHz_clock:PASS
test/test_runner.c:65:LisReadout_LOOP_start_ADC_readout:PASS
test/test_runner.c:66:LisReadout_LOOP_wait_for_most_significant_byte_ADC_readout:PASS
test/test_runner.c:67:LisReadout_LOOP_save_MSB_to_frame_buffer:PASS
test/test_runner.c:68:LisReadout_LOOP_wait_for_least_significant_byte_ADC_readout:PASS
test/test_runner.c:69:LisReadout_LOOP_save_LSB_to_frame_buffer:PASS

-----------------------
13 Tests 0 Failures 0 Ignored 
OK
