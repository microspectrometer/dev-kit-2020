/* Vim: use ;mktgc to build and run unit tests with compiler=gcc */
/* ---Unit Test Framework--- */
#include <unity.h>
#include <Mock.h>
/* ---Fake Hardware Dependencies (pins/registers)--- */
#include "fake/BiColorLed-Hardware.h"
#include "fake/UartSpi-Hardware.h"
#include "fake/Pwm-Hardware.h"
#include "fake/Lis-Hardware.h"
/* ---Make Tests Visible To `test_runner.c`--- */
#include "test_SensorVis.h" // 
#include "test_Spi.h" // setup and teardown functions

/* ---Test Framework requires runner to define setup/teardown/mock pointers--- */
void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock; // record calls/args to mocked-out libs
/* ---Define an empty setup/teardown for pointing to--- */
void NothingToSetUp(void){}
void NothingToTearDown(void){}

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;

void ExampleTestSuite(bool run_test)
{if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
    }
}
void use_Queue_lib_for_SPI_rx_FIFO_buffer(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(test_lib_SensorVis_can_use_lib_Queue_and_sees_SpiFifo);
        RUN_TEST(test_Queue_lib_examples_of_push_and_pop);
        RUN_TEST(test_Queue_lib_example_of_push_wrap_around);
    }
}

void unittest_GetSensorLED(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_GetSensorLED; tearDown = TearDown_GetSensorLED;
        RUN_TEST(GetSensorLED_receives_led_number);
        RUN_TEST(GetSensorLED_replies_msg_status_error_if_led_does_not_exist);
        RUN_TEST(GetSensorLED_sends_no_additional_bytes_if_msg_status_is_error);
        RUN_TEST(GetSensorLED_replies_msg_status_ok_if_led_number_is_recognized);
        RUN_TEST(GetSensorLED_sends_led_status_byte_after_sending_msg_status_ok);
        RUN_TEST(GetSensorLED_replies_led_off_if_led_is_off);
        RUN_TEST(GetSensorLED_replies_led_green_if_led_is_green);
        RUN_TEST(GetSensorLED_replies_led_red_if_led_is_red);
    }
}
void unittest_SetSensorLED(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_SetSensorLED; tearDown = TearDown_SetSensorLED;
        RUN_TEST(SetSensorLED_receives_led_number_and_led_state_from_Bridge);
        RUN_TEST(SetSensorLED_replies_with_one_byte);
        RUN_TEST(SetSensorLED_replies_msg_status_error_if_led_number_is_not_valid);
        RUN_TEST(SetSensorLED_replies_msg_status_error_if_led_state_is_not_valid);
        RUN_TEST(SetSensorLED_replies_msg_status_ok_if_led_number_and_led_state_are_valid);
        RUN_TEST(SetSensorLED_turns_off_led_if_payload_is_led_off);
        RUN_TEST(SetSensorLED_turns_led_on_and_green_if_payload_is_led_green);
        RUN_TEST(SetSensorLED_turns_led_on_and_red_if_payload_is_led_red);
    }
}
void unittest_GetSensorConfig(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_GetSensorConfig; tearDown = TearDown_GetSensorConfig;
        RUN_TEST(GetSensorConfig_sends_msg_status_ok_to_Bridge);
        RUN_TEST(GetSensorConfig_sends_three_bytes_of_data_to_Bridge_after_sending_ok);
    }
}
void unittest_RepresentConfigAs4bytes(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(RepresentConfigAs4bytes_writes_config_byte0_bit0_set_if_binning_is_on);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_byte0_bit0_clear_if_binning_is_off);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_byte0_bits1to2_clear_if_gain_is_1x);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_byte0_bit1_clear_bit2_set_if_gain_is_2p5x);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_byte0_bit1_set_bit2_clear_if_gain_is_4x);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_byte0_bits1to2_set_if_gain_is_5x);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_as_little_endian_so_prog_bit27_is_byte_3_bit3_not_byte0_bit_3);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_bits3to27_set_if_all_rows_are_active);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_b3b8b13b18b23_set_if_row1_is_active);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_b4b9b14b19b24_set_if_row2_is_active);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_b5b10b15b20b25_set_if_row3_is_active);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_b6b11b16b21b26_set_if_row4_is_active);
        RUN_TEST(RepresentConfigAs4bytes_writes_config_b7b12b17b22b27_set_if_row5_is_active);
    }
}
void unittest_RepresentConfigAs28bits(bool run_test)
{ // ---This is old. Need four bytes, not a uint32_t.---
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_bit0_set_if_binning_is_on);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_bit0_clear_if_binning_is_off);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_bits1to2_clear_if_gain_is_1x);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_bit1_clear_bit2_set_if_gain_is_2p5x);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_bit1_set_bit2_clear_if_gain_is_4x);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_bits1to2_set_if_gain_is_5x);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_bits3to27_set_if_all_rows_are_active);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_b3b8b13b18b23_set_if_row1_is_active);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_b4b9b14b19b24_set_if_row2_is_active);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_b5b10b15b20b25_set_if_row3_is_active);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_b6b11b16b21b26_set_if_row4_is_active);
        RUN_TEST(RepresentConfigAs28bits_returns_uint32_with_b7b12b17b22b27_set_if_row5_is_active);
    }
}
void unittest_SetSensorConfig(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_SetSensorConfig; tearDown = TearDown_SetSensorConfig;
        RUN_TEST(SetSensorConfig_receives_three_bytes_of_config_from_Bridge);
        RUN_TEST(SetSensorConfig_replies_msg_status_error_if_binning_is_invalid);
        RUN_TEST(SetSensorConfig_replies_msg_status_error_if_gain_is_invalid);
        RUN_TEST(SetSensorConfig_replies_msg_status_error_if_active_rows_is_invalid);
        RUN_TEST(SetSensorConfig_replies_msg_status_ok_if_all_config_bytes_are_valid);
        RUN_TEST(SetSensorConfig_programs_the_photodiode_array_with_the_config);
    }
}
void unittest_GetExposure(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_GetExposure; tearDown = TearDown_GetExposure;
        RUN_TEST(GetExposure_sends_status_byte_ok);
        RUN_TEST(GetExposure_sends_two_bytes_of_exposure_time_most_significant_byte_first);
    }
}
void unittest_SetExposure(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_SetExposure; tearDown = TearDown_SetExposure;
        RUN_TEST(SetExposure_receives_two_bytes_of_exposure_msb_first_from_Bridge);
        RUN_TEST(SetExposure_replies_msg_status_ok);
        RUN_TEST(SetExposure_converts_two_data_bytes_to_new_16_bit_exposure_ticks_value);
    }
}
void unittest_NumPixelsInFrame(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(npixels_is_a_macro_constant_equal_to_784);
        RUN_TEST(NumPixelsInFrame_returns_784_if_binning_is_off);
        RUN_TEST(NumPixelsInFrame_returns_392_if_binning_is_on);
    }
}
void unittest_WordToTwoByteArray(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(WordToTwoByteArray_stores_16bit_word_msb_first_in_input_array);
    }
}
void unittest_LisReadout(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
    }
}
void unittest_GetFrame(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(GetFrame_exposes_the_photodiode_array);
    }
}
void unittest_CaptureFrame(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_CaptureFrame; tearDown = TearDown_CaptureFrame;
        RUN_TEST(CaptureFrame_sends_status_byte_ok);
        RUN_TEST(CaptureFrame_collects_a_frame_of_pixel_data);
        RUN_TEST(CaptureFrame_sends_two_bytes_msb_first_with_number_of_pixels_in_frame);
        RUN_TEST(CaptureFrame_sends_another_status_byte_ok);
        RUN_TEST(CaptureFrame_sends_the_frame_as_two_bytes_per_pixel_msb_first);
    }
}
void develop_LIS_programming_sequence(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(bit_number_is_correct_on_each_iteration_of_while_loop);
    }
}

int main(void)
{
    UNITY_BEGIN();
    use_Queue_lib_for_SPI_rx_FIFO_buffer(Nope); // good
    unittest_GetSensorLED(Nope);
    unittest_SetSensorLED(Nope);
    /* setUp = NothingToSetUp; tearDown = NothingToTearDown; */
    // Put single tests here (move single tests to test suite later).
    unittest_GetSensorConfig(Nope);
    unittest_SetSensorConfig(Nope);
    unittest_GetExposure(Nope);
    unittest_SetExposure(Nope);
    unittest_NumPixelsInFrame(Nope);
    unittest_WordToTwoByteArray(Nope);
    unittest_LisReadout(Nope);
    unittest_GetFrame(Nope);
    unittest_CaptureFrame(Nope);
    unittest_RepresentConfigAs28bits(Nope); // deprecated
    unittest_RepresentConfigAs4bytes(Yep);
    // TODO: move the LIS_programming_sequence to a function
    develop_LIS_programming_sequence(Nope);
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    /* RUN_TEST(LisReadout_reads_npixels); */
    return UNITY_END();
}
