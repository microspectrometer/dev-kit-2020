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

    }
}
int main(void)
{
    UNITY_BEGIN();
    test_example(Nope);
    test_VisCmd(Yep);
    return UNITY_END();
}
