#ifndef _TEST_LIS_H
#define _TEST_LIS_H

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
void LisConfigIsValid_returns_false_if_binning_is_invalid(void);
void LisConfigIsValid_returns_false_if_gain_is_invalid(void);
void LisConfigIsValid_returns_false_if_active_rows_is_invalid(void);
void LisConfigIsValid_returns_true_if_config_is_valid(void);

#endif // _TEST_LIS_H
