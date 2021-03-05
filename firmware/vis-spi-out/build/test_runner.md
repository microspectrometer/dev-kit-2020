# test VisCmd
## VisCmd.LedNumIsValid
test/test_runner.c:54:LedNumIsValid_returns_TRUE_if_led_num_is_0:PASS
test/test_runner.c:55:LedNumIsValid_returns_TRUE_if_led_num_is_1:PASS
test/test_runner.c:56:LedNumIsValid_returns_FALSE_if_led_num_is_not_0_or_1:PASS
## VisCmd.LisReadout
test/test_runner.c:61:LisReadout_waits_for_Lis_Sync_to_go_HIGH_then_go_LOW:PASS
test/test_runner.c:62:LisReadout_reads_one_pixel_on_each_rising_edge_of_Lis_Clk:PASS
test/test_runner.c:63:LisReadout_LOOP_wait_for_the_rising_edge_of_Lis_Clk:PASS
test/test_runner.c:64:LisReadout_LOOP_start_the_ADC_conversion:PASS
test/test_runner.c:65:LisReadout_LOOP_wait_for_45_cycles_of_10MHz_clock:PASS
test/test_runner.c:66:LisReadout_LOOP_start_ADC_readout:PASS
test/test_runner.c:67:LisReadout_LOOP_wait_for_most_significant_byte_ADC_readout:PASS
test/test_runner.c:68:LisReadout_LOOP_save_MSB_to_frame_buffer:PASS
test/test_runner.c:69:LisReadout_LOOP_wait_for_least_significant_byte_ADC_readout:PASS
test/test_runner.c:70:LisReadout_LOOP_save_LSB_to_frame_buffer:PASS
## VisCmd.GetSensorLED
test/test_runner.c:75:GetSensorLED_waits_for_byte_led_num:PASS
test/test_runner.c:76:GetSensorLED_reads_byte_led_num:PASS
test/test_runner.c:77:GetSensorLED_sends_OK_and_LED_SETTING_if_led_num_is_valid:PASS
test/test_runner.c:78:GetSensorLED_sends_ERROR_and_pads_second_byte_if_led_num_is_invalid:PASS
## VisCmd.SetSensorLED
test/test_runner.c:83:SetSensorLED_waits_for_byte_led_num:PASS
test/test_runner.c:84:SetSensorLED_reads_byte_led_num:PASS
test/test_runner.c:85:SetSensorLED_waits_for_byte_led_setting:PASS
test/test_runner.c:86:SetSensorLED_reads_byte_led_setting:PASS
test/test_runner.c:87:SetSensorLED_sends_ERROR_if_led_num_is_invalid:PASS
test/test_runner.c:88:SetSensorLED_sends_ERROR_if_led_setting_is_invalid:PASS
test/test_runner.c:89:SetSensorLED_applies_LED_setting_if_valid:PASS
test/test_runner.c:90:SetSensorLED_sends_OK_if_num_and_setting_are_valid:PASS
## VisCmd.GetSensorConfig
test/test_runner.c:95:GetSensorConfig_sends_OK:PASS
test/test_runner.c:96:GetSensorConfig_sends_binning:PASS
test/test_runner.c:97:GetSensorConfig_sends_gain:PASS
test/test_runner.c:98:GetSensorConfig_sends_row_bitmap:PASS
## VisCmd.SetSensorConfig
test/test_runner.c:103:SetSensorConfig_receives_three_bytes_of_config_from_Bridge:PASS
INVALID: binning 
binning == 100, gain == 5, active_rows == 1
test/test_runner.c:104:SetSensorConfig_does_not_update_config_globals_if_config_is_invalid:PASS
INVALID: binning 
binning == 100, gain == 5, active_rows == 1

- Call 1 is named SpiSlaveTxByte? Yes.


- Value passed to call 1, arg 1? 
    - (uint8_t)0x01 is arg 1 to call SpiSlaveTxByte.

test/test_runner.c:105:SetSensorConfig_replies_ERROR_if_binning_is_invalid:PASS
INVALID: gain 
binning == 0, gain == 105, active_rows == 1

- Call 1 is named SpiSlaveTxByte? Yes.


- Value passed to call 1, arg 1? 
    - (uint8_t)0x01 is arg 1 to call SpiSlaveTxByte.

test/test_runner.c:106:SetSensorConfig_replies_ERROR_if_gain_is_invalid:PASS
INVALID: active_rows
binning == 0, gain == 5, active_rows == 255

- Call 1 is named SpiSlaveTxByte? Yes.


- Value passed to call 1, arg 1? 
    - (uint8_t)0x01 is arg 1 to call SpiSlaveTxByte.

test/test_runner.c:107:SetSensorConfig_replies_ERROR_if_active_rows_is_invalid:PASS
INVALID: binning gain active_rows
binning == 100, gain == 105, active_rows == 255

- Call 1 is named SpiSlaveTxByte? Yes.


- Value passed to call 1, arg 1? 
    - (uint8_t)0x01 is arg 1 to call SpiSlaveTxByte.

test/test_runner.c:108:SetSensorConfig_does_not_program_LIS_770i_if_config_is_invalid:PASS

- Call 1 is named LisWriteConfig? Yes.

test/test_runner.c:109:SetSensorConfig_writes_valid_config_to_LIS_770i_programmable_setup_register:PASS

- Call 2 is named SpiSlaveTxByte? Yes.


- Value passed to call 2, arg 1? 
    - (uint8_t)0000 is arg 1 to call SpiSlaveTxByte.

test/test_runner.c:110:SetSensorConfig_replies_OK_if_all_config_values_are_valid:PASS

- Call 1 is named LisWriteConfig? Yes.


- Call 2 is named SpiSlaveTxByte? Yes.


- Value passed to call 2, arg 1? 
    - (uint8_t)0000 is arg 1 to call SpiSlaveTxByte.

test/test_runner.c:111:SetSensorConfig_the_OK_is_sent_after_LIS_is_programmed_with_new_config:PASS
## VisCmd.GetExposure
test/test_runner.c:116:GetExposure_sends_OK:PASS
test/test_runner.c:117:GetExposure_sends_MSB_of_exposure_ticks:PASS
test/test_runner.c:118:GetExposure_sends_LSB_of_exposure_ticks:PASS
## VisCmd.SetExposure
test/test_runner.c:123:SetExposure_waits_for_byte_exposure_MSB:PASS
test/test_runner.c:124:SetExposure_reads_byte_exposure_MSB:PASS
test/test_runner.c:125:SetExposure_waits_for_byte_exposure_LSB:PASS
test/test_runner.c:126:SetExposure_reads_byte_exposure_LSB:PASS
test/test_runner.c:127:SetExposure_updates_global_exposure_ticks:PASS
test/test_runner.c:128:SetExposure_sends_OK:PASS
## VisCmd.CaptureFrame
test/test_runner.c:133:CaptureFrame_sends_OK:PASS
test/test_runner.c:134:CaptureFrame_checks_binning_to_determine_number_of_pixels_in_frame:PASS
test/test_runner.c:135:CaptureFrame_sends_num_pixels_MSB:PASS
test/test_runner.c:136:CaptureFrame_sends_num_pixels_LSB:PASS
test/test_runner.c:137:CaptureFrame_exposes_the_pixels:PASS
test/test_runner.c:138:CaptureFrame_does_readout_of_num_pixels_into_the_frame_buffer:PASS
test/test_runner.c:139:CaptureFrame_sends_the_pixel_readings_stored_in_the_frame_buffer:PASS
## VisCmd.GetPeak
test/test_runner.c:547:GetPeak_is_visible:FAIL: Implement test.
## VisCmd.AutoExpose
test/test_runner.c:151:AutoExpose_turns_led1_red_to_indicate_starting:PASS
test/test_runner.c:152:AutoExpose_sets_min_peak_at_target_minus_tolerance:PASS
test/test_runner.c:153:AutoExpose_clamps_min_peak_at_max_dark_if_target_minus_tolerance_is_GREATER_THAN_target:PASS
test/test_runner.c:154:AutoExpose_clamps_min_peak_at_max_dark_if_target_minus_tolerance_is_LESS_THAN_max_dark:PASS
test/test_runner.c:155:AutoExpose_sets_max_peak_at_target_plus_tolerance:PASS
test/test_runner.c:156:AutoExpose_clamps_max_peak_at_65535_counts_if_target_plus_tolerance_is_LESS_THAN_target:PASS
test/test_runner.c:157:AutoExpose_loops_until_done:PASS

- Call 1 is named LisExpose? Yes.

test/test_runner.c:158:AutoExpose_exposes_the_LIS_770i_pixels:PASS

- Call 2 is named LisReadout? Yes.

test/test_runner.c:159:AutoExpose_reads_pixel_counts_into_global_frame_buffer:PASS
test/test_runner.c:160:AutoExpose_finds_frame_peak_in_range_start_pixel_to_stop_pixel:PASS
test/test_runner.c:161:AutoExpose_is_done_if_peak_less_than_max_dark_AND_exposure_at_max:PASS
test/test_runner.c:162:AutoExpose_scales_exposure_by_10_if_peak_less_than_max_dark:PASS
test/test_runner.c:163:AutoExpose_clamps_exposure_at_max_exposure_if_10_x_exposure_is_GREATER_THAN_max_exposure:PASS
test/test_runner.c:164:AutoExpose_scales_exposure_by_half_if_peak_ABOVE_max_peak:PASS
test/test_runner.c:165:AutoExpose_clamps_exposure_at_min_exposure_if_half_exposure_is_LESS_THAN_min_exposure:PASS
test/test_runner.c:166:AutoExpose_is_done_if_peak_BELOW_min_peak_and_exposure_at_max_exposure:PASS
test/test_runner.c:167:AutoExpose_scales_exposure_by_target_div_peak_if_peak_BELOW_min_peak_and_exposure_not_at_max:PASS
test/test_runner.c:168:AutoExpose_clamps_exposure_at_max_exposure_if_gain_is_GREATER_THAN_max_exposure:PASS
test/test_runner.c:169:AutoExpose_is_done_if_peak_is_in_the_target_range:PASS
test/test_runner.c:170:AutoExpose_turns_led1_green_to_indicate_it_hit_the_target_range:PASS
test/test_runner.c:171:AutoExpose_gives_up_if_it_iterates_for_max_tries:PASS
# test AutoExpose

-----------------------
76 Tests 1 Failures 0 Ignored 
FAIL
