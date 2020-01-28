#ifndef _TEST_LIS_H
#define _TEST_LIS_H

/* =====[ LisInit ]===== */
void LisInit_sets_PixSelect_as_an_output(void);
void LisInit_idles_PixSelect_low(void);
void LisInit_sets_Clk_as_an_output(void);
void LisInit_sets_Rst_as_an_output(void);
void LisInit_idles_Rst_low(void);
void LisInit_sets_Sync_as_an_input(void);
void LisInit_sets_PWM_frequency_at_50kHz(void);
void LisInit_sets_PWM_duty_cycle_to_50_percent(void);
void LisInit_resets_PWM_timer_at_top(void);
void LisInit_PWM_timer_top_is_OCR0A(void);
void LisInit_PWM_timer_is_clocked_by_CPU_with_no_prescaling(void);
void LisInit_outputs_the_PWM_clock_on_pin_Clk(void);

/* =====[ LisConfigIsValid ]===== */
void LisConfigIsValid_returns_false_if_binning_is_invalid(void);
void LisConfigIsValid_returns_false_if_gain_is_invalid(void);
void LisConfigIsValid_returns_false_if_active_rows_is_invalid(void);
void LisConfigIsValid_returns_true_if_config_is_valid(void);

/* =====[ _ConfigAs28bits ]===== */
void ConfigAs28bits_writes_config_as_little_endian_ie_binning_is_config_byte0_bit0(void);
void ConfigAs28bits_sets_config_byte0_bit0_if_BINNING_ON(void);
void ConfigAs28bits_clears_config_byte0_bit0_if_BINNING_OFF(void);
void ConfigAs28bits_byte0_bit1_clear_and_bit2_clear_if_GAIN_1X(void);
void ConfigAs28bits_byte0_bit1_clear_and_bit2_set_if_GAIN_2X5(void);
void ConfigAs28bits_byte0_bit1_set_and_bit2_clear_if_GAIN_4X(void);
void ConfigAs28bits_byte0_bit1_set_and_bit2_set_if_GAIN_5X(void);
void ConfigAs28bits_bit3_to_bit27_set_if_ALL_ROWS_ACTIVE(void);
void ConfigAs28bits_b3b8b13b18b23_set_if_ROW_1_ACTIVE(void);
void ConfigAs28bits_b4b9b14b19b24_set_if_ROW_2_ACTIVE(void);
void ConfigAs28bits_b5b10b15b20b25_set_if_ROW_3_ACTIVE(void);
void ConfigAs28bits_b6b11b16b21b26_set_if_ROW_4_ACTIVE(void);
void ConfigAs28bits_b7b12b17b22b27_set_if_ROW_5_ACTIVE(void);

/* =====[ LisWriteConfig ]===== */
void LisWriteConfig_converts_config_to_28bit_sequence(void);
void LisWriteConfig_enters_LIS_programming_mode(void);
void LisWriteConfig_writes_28bits_to_LIS_setup_register(void);
void LisWriteConfig_exits_LIS_programming_mode(void);

/* =====[ _WaitForLisClkLow ]===== */
void WaitForLisClkLow_clears_flag_PwmTimerMatchesOCF0B(void);
void WaitForLisClkLow_waits_until_flag_PwmTimerMatchesOCF0B_is_set(void);

/* =====[ _WaitForLisClkHigh ]===== */
void WaitForLisClkHigh_clears_flag_PwmTimerMatchesOCF0A(void);
void WaitForLisClkHigh_waits_until_flag_PwmTimerMatchesOCF0A_is_set(void);

/* =====[ _EnterLisProgrammingMode ]===== */
void EnterLisProgrammingMode_waits_for_LisClk_LOW(void);
void EnterLisProgrammingMode_asserts_LisPixSelect_to_program_Lis(void);

/* =====[ _ExitLisProgrammingMode ]===== */
void ExitLisProgrammingMode_outputs_LOW_on_pin_LisRst(void);
void ExitLisProgrammingMode_outputs_LOW_on_pin_LisPixSelect(void);

/* =====[ _WriteLisConfigBit ]===== */
void WriteLisConfigBit_outputs_bit_on_LisRst(void);
void WriteLisConfigBit_waits_for_LisClk_HIGH(void);
void WriteLisConfigBit_waits_for_LisClk_LOW(void);

#endif // _TEST_LIS_H
