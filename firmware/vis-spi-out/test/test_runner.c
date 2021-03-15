// Pick a sensor
#ifdef LIS // <------------- $ make sensor=LIS
#include "LisConfigs.h"
#endif
#ifdef S13131 // <---------- $ make sensor=S13131
#endif

#include <unity.h>
#include <Mock.h>
#include "HardwareFake.h" // Fake hardware (registers, pins)
#include "StatusCodes.h" // Python-to-Firmware communication status codes

#include "test_example.h"
#include "test_VisCmd.h"
#include "test_AutoExpose.h"

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

#ifdef LIS
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
#endif
#ifdef S13131
        puts("## VisCmd.S13131Readout");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(S13131Readout_must_be_called_immediately_after_S13131Expose);
        RUN_TEST(S13131Readout_stores_pixel_values_in_a_global_array_named_frame);
        RUN_TEST(S13131Readout_reads_one_pixel_on_each_falling_edge_of_CLK);
        RUN_TEST(S13131Readout_LOOP_wait_for_a_CLK_falling_edge);
        RUN_TEST(S13131Readout_LOOP_start_the_ADC_conversion);
        RUN_TEST(S13131Readout_LOOP_wait_for_45_cycles_of_10MHz_clock);
        RUN_TEST(S13131Readout_LOOP_start_ADC_readout);
        RUN_TEST(S13131Readout_LOOP_wait_for_most_significant_byte_ADC_readout);
        RUN_TEST(S13131Readout_LOOP_save_MSB_to_frame_buffer);
        RUN_TEST(S13131Readout_LOOP_wait_for_least_significant_byte_ADC_readout);
        RUN_TEST(S13131Readout_LOOP_save_LSB_to_frame_buffer);

#endif

        puts("## VisCmd.GetSensorLED");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(GetSensorLED_waits_for_byte_led_num);
        RUN_TEST(GetSensorLED_reads_byte_led_num);
        RUN_TEST(GetSensorLED_sends_OK_and_LED_SETTING_if_led_num_is_valid);
        RUN_TEST(GetSensorLED_sends_ERROR_and_pads_second_byte_if_led_num_is_invalid);

        puts("## VisCmd.SetSensorLED");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(SetSensorLED_waits_for_byte_led_num);
        RUN_TEST(SetSensorLED_reads_byte_led_num);
        RUN_TEST(SetSensorLED_waits_for_byte_led_setting);
        RUN_TEST(SetSensorLED_reads_byte_led_setting);
        RUN_TEST(SetSensorLED_sends_ERROR_if_led_num_is_invalid);
        RUN_TEST(SetSensorLED_sends_ERROR_if_led_setting_is_invalid);
        RUN_TEST(SetSensorLED_applies_LED_setting_if_valid);
        RUN_TEST(SetSensorLED_sends_OK_if_num_and_setting_are_valid);

#ifdef LIS
        puts("## VisCmd.GetSensorConfig");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(GetSensorConfig_sends_OK);
        RUN_TEST(GetSensorConfig_sends_binning);
        RUN_TEST(GetSensorConfig_sends_gain);
        RUN_TEST(GetSensorConfig_sends_row_bitmap);

        puts("## VisCmd.SetSensorConfig");
        setUp = SetUp_Mock;
        tearDown = TearDown_Mock;
        RUN_TEST(SetSensorConfig_receives_three_bytes_of_config_from_Bridge);
        RUN_TEST(SetSensorConfig_does_not_update_config_globals_if_config_is_invalid);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_binning_is_invalid);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_gain_is_invalid);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_active_rows_is_invalid);
        RUN_TEST(SetSensorConfig_does_not_program_LIS_770i_if_config_is_invalid);
        RUN_TEST(SetSensorConfig_writes_valid_config_to_LIS_770i_programmable_setup_register);
        RUN_TEST(SetSensorConfig_replies_OK_if_all_config_values_are_valid);
        RUN_TEST(SetSensorConfig_the_OK_is_sent_after_LIS_is_programmed_with_new_config);
#endif
#ifdef S13131
        // S13131 has nothing to configure
#endif


        puts("## VisCmd.GetExposure");
        setUp = EmptySetUp;
        tearDown = EmptyTearDown;
        RUN_TEST(GetExposure_sends_OK);
        RUN_TEST(GetExposure_sends_MSB_of_exposure_ticks);
        RUN_TEST(GetExposure_sends_LSB_of_exposure_ticks);

        puts("## VisCmd.SetExposure");
        setUp = EmptySetUp;
        tearDown = EmptyTearDown;
        RUN_TEST(SetExposure_waits_for_byte_exposure_MSB);
        RUN_TEST(SetExposure_reads_byte_exposure_MSB);
        RUN_TEST(SetExposure_waits_for_byte_exposure_LSB);
        RUN_TEST(SetExposure_reads_byte_exposure_LSB);
        RUN_TEST(SetExposure_updates_global_exposure_ticks);
        RUN_TEST(SetExposure_sends_OK);

        puts("## VisCmd.CaptureFrame");
        setUp = EmptySetUp;
        tearDown = EmptyTearDown;
        RUN_TEST(CaptureFrame_sends_OK);
        RUN_TEST(CaptureFrame_checks_binning_to_determine_number_of_pixels_in_frame);
        RUN_TEST(CaptureFrame_sends_num_pixels_MSB);
        RUN_TEST(CaptureFrame_sends_num_pixels_LSB);
        RUN_TEST(CaptureFrame_exposes_the_pixels);
        RUN_TEST(CaptureFrame_does_readout_of_num_pixels_into_the_frame_buffer);
        RUN_TEST(CaptureFrame_sends_the_pixel_readings_stored_in_the_frame_buffer);



        puts("## VisCmd.GetPeak");
        setUp = EmptySetUp;
        tearDown = EmptyTearDown;
        RUN_TEST(GetPeak_finds_the_peak_between_start_pixel_and_stop_pixel_inclusive);
        RUN_TEST(GetPeak_ignores_peaks_at_pixels_before_start_pixel_and_after_stop_pixel);


    }
}

void test_AutoExpose(bool run_test)
{
    if (run_test)
    {
        puts("# test AutoExpose");

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
        RUN_TEST(AutoExpose_exposes_the_pixels);
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

int main(void)
{
    UNITY_BEGIN();
    test_example(Nope);
    test_VisCmd(Yep);
    test_AutoExpose(Yep);
    puts("# Results");
    return UNITY_END();
}
