// Pick a sensor
#ifdef LIS // <------------- $ make sensor=LIS
#include "LisConfigs.h"
#endif
#ifdef S13131 // <---------- $ make sensor=S13131
#include "S13131Configs.h"
#endif

#include <unity.h>
#include <Mock.h>
#include "test_example.h"
#include "test_VisCmd.h"
#include "test_AutoExpose.h"
#include "StatusCodes.h" // Python-to-Firmware communication status codes
#include "HardwareFake.h" // Fake hardware (registers, pins)

void (*setUp)(void);
void EmptySetUp(void) {
}
void (*tearDown)(void);
void EmptyTearDown(void) {
}
Mock_s *mock; // Tests record mocked calls and call args here
void SetUp_Mock(void){
    mock = Mock_new();
}
void TearDown_Mock(void){
    Mock_destroy(mock);
    mock = NULL;
}

bool Yep=true, Nope=false; // Yep/Nope := tests on/off

void test_example(bool run_test)
{
    if (run_test)
    {
        setUp = EmptySetUp; tearDown = EmptyTearDown;
        RUN_TEST(test_Can_build_test_suite);
        RUN_TEST(test_Can_call_function_defined_in_app_lib);
        RUN_TEST(test_Can_call_inline_function_defined_in_app_lib);
    }
}

void test_VisCmd(bool run_test)
{
    if (run_test)
    {
        puts("# test VisCmd");

        puts("## VisCmd.LedNumIsValid");
        setUp = EmptySetUp;
        tearDown = EmptyTearDown;
        RUN_TEST(LedNumIsValid_returns_TRUE_if_led_num_is_0);
        RUN_TEST(LedNumIsValid_returns_TRUE_if_led_num_is_1);
        RUN_TEST(LedNumIsValid_returns_FALSE_if_led_num_is_not_0_or_1);

        puts("## VisCmd.LisReadout");
        setUp = EmptySetUp;
        tearDown = EmptyTearDown;
        RUN_TEST(LisReadout_waits_for_Lis_Sync_to_go_HIGH_then_go_LOW);
        RUN_TEST(LisReadout_reads_one_pixel_on_each_rising_edge_of_Lis_Clk);
        RUN_TEST(LisReadout_LOOP_wait_for_the_rising_edge_of_Lis_Clk);
        RUN_TEST(LisReadout_LOOP_start_the_ADC_conversion);
        RUN_TEST(LisReadout_LOOP_wait_for_45_cycles_of_10MHz_clock);
        RUN_TEST(LisReadout_LOOP_start_ADC_readout);
        RUN_TEST(LisReadout_LOOP_wait_for_most_significant_byte_ADC_readout);
        RUN_TEST(LisReadout_LOOP_save_MSB_to_frame_buffer);
        RUN_TEST(LisReadout_LOOP_wait_for_least_significant_byte_ADC_readout);
        RUN_TEST(LisReadout_LOOP_save_LSB_to_frame_buffer);

        puts("## VisCmd.GetPeak");
        setUp = EmptySetUp;
        tearDown = EmptyTearDown;
        RUN_TEST(GetPeak_is_visible);

        puts("## VisCmd.AutoExpose");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(AutoExpose_turns_led1_red_to_indicate_starting);
        RUN_TEST(AutoExpose_sets_min_peak_at_target_minus_tolerance);
        RUN_TEST(AutoExpose_clamps_min_peak_at_max_dark_if_target_minus_tolerance_is_GREATER_THAN_target);
        RUN_TEST(AutoExpose_clamps_min_peak_at_max_dark_if_target_minus_tolerance_is_LESS_THAN_max_dark);
        RUN_TEST(AutoExpose_sets_max_peak_at_target_plus_tolerance);
        RUN_TEST(AutoExpose_clamps_max_peak_at_65535_counts_if_target_plus_tolerance_is_LESS_THAN_target);
        RUN_TEST(AutoExpose_loops_until_done);
        RUN_TEST(AutoExpose_exposes_the_LIS_770i_pixels);
        RUN_TEST(AutoExpose_reads_pixel_counts_into_global_frame_buffer);
        RUN_TEST(AutoExpose_finds_frame_peak_in_range_start_pixel_to_stop_pixel);
        RUN_TEST(AutoExpose_is_done_if_peak_less_than_max_dark_AND_exposure_at_max);
        RUN_TEST(AutoExpose_scales_exposure_by_10_if_peak_less_than_max_dark);
        RUN_TEST(AutoExpose_clamps_exposure_at_max_exposure_if_10_x_exposure_is_GREATER_THAN_max_exposure);
        RUN_TEST(AutoExpose_scales_exposure_by_half_if_peak_ABOVE_max_peak);
        RUN_TEST(AutoExpose_clamps_exposure_at_min_exposure_if_half_exposure_is_LESS_THAN_min_exposure);
        RUN_TEST(AutoExpose_is_done_if_peak_BELOW_min_peak_and_exposure_at_max_exposure);
        RUN_TEST(AutoExpose_scales_exposure_by_target_div_peak_if_peak_BELOW_min_peak_and_exposure_not_at_max);
        RUN_TEST(AutoExpose_clamps_exposure_at_max_exposure_if_gain_is_GREATER_THAN_max_exposure);
        RUN_TEST(AutoExpose_is_done_if_peak_is_in_the_target_range);
        RUN_TEST(AutoExpose_turns_led1_green_to_indicate_it_hit_the_target_range);
        RUN_TEST(AutoExpose_gives_up_if_it_iterates_for_max_tries);

    }
}

void test_AutoExpose(bool run_test)
{
    if (run_test)
    {
        puts("# test AutoExpose");

    }
}

int main(void)
{
    UNITY_BEGIN();
    test_example(Nope);
    test_VisCmd(Yep);
    test_AutoExpose(Yep);
    return UNITY_END();
}
