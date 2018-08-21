test/test_runner.c:218:LisInit_sets_PixSelect_as_an_output:PASS
test/test_runner.c:219:LisInit_sets_Clk_as_an_output:PASS
test/test_runner.c:220:LisInit_sets_Rst_as_an_output:PASS
test/test_runner.c:221:LisInit_sets_Sync_as_an_input:PASS
test/test_runner.c:222:LisInit_idles_PixSelect_low:PASS
test/test_runner.c:223:LisInit_idles_Rst_low:PASS
test/test_runner.c:225:LisInit_configures_pin_Clk_to_generate_the_clock_signal:PASS
test/test_runner.c:226:LisInit_turns_on_the_clock_signal:PASS
test/test_runner.c:229:LisRunClkAt50kHz_sets_clock_to_50kHz:PASS
test/test_runner.c:230:LisRunClkAt50kHz_sets_clock_to_50percent_duty_cycle:PASS
test/test_runner.c:233:LisClkOn_outputs_the_clock_signal_on_pin_Clk:PASS
test/test_runner.c:236:LisClkOff_idles_Clk_low:PASS
test/test_runner.c:240:PwmResetCounterAtTop_configures_PWM_for_fast_PWM_mode:PASS
test/test_runner.c:241:PwmTopIsOcr0a_uses_the_value_in_OCR0A_for_TOP:PASS
test/test_runner.c:242:PwmClkIsCpuClk_uses_the_cpu_clock_with_no_prescaler:PASS
test/test_runner.c:243:PwmEnableOutputSetUntilMatch_sets_OC0B_at_bottom_and_clears_on_match:PASS
test/test_runner.c:244:PwmDisableOutput_disconnects_OC0B_and_restores_normal_io_output:PASS

-----------------------
17 Tests 0 Failures 0 Ignored 
OK
