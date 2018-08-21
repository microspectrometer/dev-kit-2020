#ifndef _TEST_LIS_H
#define _TEST_LIS_H

void LisInit_sets_PixSelect_as_an_output(void);
void LisInit_sets_Clk_as_an_output(void);
void LisInit_sets_Rst_as_an_output(void);
void LisInit_sets_Sync_as_an_input(void);
void LisInit_idles_PixSelect_low(void);
void LisInit_idles_Rst_low(void);
void SetUp_LisInit(void); void TearDown_LisInit(void);
void LisInit_configures_pin_Clk_to_generate_the_clock_signal(void);
void LisInit_turns_on_the_clock_signal(void);
//
void SetUp_LisRunClkAt50kHz(void); void TearDown_LisRunClkAt50kHz(void);
void LisRunClkAt50kHz_sets_clock_to_50kHz(void);
void LisRunClkAt50kHz_sets_clock_to_50percent_duty_cycle(void);
//
void SetUp_LisClkOn(void); void TearDown_LisClkOn(void);
void LisClkOn_outputs_the_clock_signal_on_pin_Clk(void);
//
void SetUp_LisClkOff(void); void TearDown_LisClkOff(void);
void LisClkOff_idles_Clk_low(void);
void LisExpose_exposes_pixels_for_nticks_of_LIS_clock(void);

#endif // _TEST_LIS_H
