#include <unity.h>
#include <Mock.h>
#include "HardwareFake.h" // Fake hardware (registers, pins)
#include "StatusCodes.h" // Python-to-Firmware communication status codes

#include "test_UsbCmd.h"

void EmptySetUp(void){
}
void (*setUp)(void) = EmptySetUp;
void EmptyTearDown(void){
}
void (*tearDown)(void) = EmptyTearDown;
Mock_s *mock;

void test_UsbCmd(void)
{
    puts("# UsbCmd tests");

    puts("## ReadLedState");
    RUN_TEST(ReadLedState_returns_OFF_if_LED_is_off);
    RUN_TEST(ReadLedState_returns_GREEN_if_LED_is_on_and_green);
    RUN_TEST(ReadLedState_returns_RED_if_LED_is_on_and_red);

    puts("## GetBridgeLED");
    RUN_TEST(GetBridgeLED_waits_for_byte_led_num);
    RUN_TEST(GetBridgeLED_reads_byte_led_num);
    RUN_TEST(GetBridgeLED_writes_two_bytes_to_USB_host);
    RUN_TEST(GetBridgeLED_writes_OK_and_LED_STATE_if_led_num_is_valid);
    RUN_TEST(GetBridgeLED_writes_ERROR_and_pads_second_byte_if_led_num_is_invalid);

    puts("## SetBridgeLED");
    RUN_TEST(SetBridgeLED_waits_for_byte_led_num);
    RUN_TEST(SetBridgeLED_reads_byte_led_num);
    RUN_TEST(SetBridgeLED_waits_for_byte_led_setting);
    RUN_TEST(SetBridgeLED_reads_byte_led_setting);
    RUN_TEST(SetBridgeLED_applies_led_setting_and_writes_OK_if_parameters_are_valid);
    RUN_TEST(SetBridgeLED_writes_ERROR_if_led_num_is_invalid);
    RUN_TEST(SetBridgeLED_writes_ERROR_if_led_setting_is_invalid);

    puts("## GetSensorLED");
    RUN_TEST(GetSensorLED_waits_for_byte_led_num);
    RUN_TEST(GetSensorLED_reads_byte_led_num);
    RUN_TEST(GetSensorLED_sends_command_to_sensor);
    RUN_TEST(GetSensorLED_sends_led_num_to_sensor);
    RUN_TEST(GetSensorLED_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
    RUN_TEST(GetSensorLED_waits_for_sensor_to_signal_STATUS_data_ready);
    RUN_TEST(GetSensorLED_reads_status_from_sensor);
    RUN_TEST(GetSensorLED_waits_for_sensor_to_signal_LED_SETTING_data_ready);
    RUN_TEST(GetSensorLED_reads_led_setting_from_sensor);
    RUN_TEST(GetSensorLED_writes_sensor_status);
    RUN_TEST(GetSensorLED_writes_sensor_led_setting);

    puts("## SetSensorLED");
    RUN_TEST(SetSensorLED_waits_for_byte_led_num);
    RUN_TEST(SetSensorLED_reads_byte_led_num);
    RUN_TEST(SetSensorLED_waits_for_byte_led_setting);
    RUN_TEST(SetSensorLED_reads_byte_led_setting);
    RUN_TEST(SetSensorLED_sends_command_to_sensor);
    RUN_TEST(SetSensorLED_sends_led_num_to_sensor);
    RUN_TEST(SetSensorLED_sends_led_setting_to_sensor);
    RUN_TEST(SetSensorLED_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
    RUN_TEST(SetSensorLED_waits_for_sensor_to_signal_STATUS_data_ready);
    RUN_TEST(SetSensorLED_reads_status_from_sensor);
    RUN_TEST(SetSensorLED_writes_sensor_status);

    puts("## GetSensorConfig");
    RUN_TEST(GetSensorConfig_sends_command_to_sensor);
    RUN_TEST(GetSensorConfig_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
    RUN_TEST(GetSensorConfig_waits_for_sensor_to_signal_STATUS_data_ready);
    RUN_TEST(GetSensorConfig_reads_status_from_sensor);
    RUN_TEST(GetSensorConfig_waits_for_sensor_to_signal_BINNING_data_ready);
    RUN_TEST(GetSensorConfig_reads_binning_from_sensor);
    RUN_TEST(GetSensorConfig_waits_for_sensor_to_signal_GAIN_data_ready);
    RUN_TEST(GetSensorConfig_reads_gain_from_sensor);
    RUN_TEST(GetSensorConfig_waits_for_sensor_to_signal_ROW_BITMAP_data_ready);
    RUN_TEST(GetSensorConfig_reads_row_bitmap_from_sensor);
    RUN_TEST(GetSensorConfig_writes_sensor_status);
    RUN_TEST(GetSensorConfig_writes_sensor_binning);
    RUN_TEST(GetSensorConfig_writes_sensor_gain);
    RUN_TEST(GetSensorConfig_writes_sensor_row_bitmap);

    puts("## SetSensorConfig");
    RUN_TEST(SetSensorConfig_waits_for_byte_binning);
    RUN_TEST(SetSensorConfig_reads_byte_binning);
    RUN_TEST(SetSensorConfig_waits_for_byte_gain);
    RUN_TEST(SetSensorConfig_reads_byte_gain);
    RUN_TEST(SetSensorConfig_waits_for_byte_row_bitmap);
    RUN_TEST(SetSensorConfig_reads_byte_row_bitmap);
    RUN_TEST(SetSensorConfig_sends_command_to_sensor);
    RUN_TEST(SetSensorConfig_sends_binning_to_sensor);
    RUN_TEST(SetSensorConfig_sends_gain_to_sensor);
    RUN_TEST(SetSensorConfig_sends_row_bitmap_to_sensor);
    RUN_TEST(SetSensorConfig_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
    RUN_TEST(SetSensorConfig_waits_for_sensor_to_signal_STATUS_data_ready);
    RUN_TEST(SetSensorConfig_reads_status_from_sensor);
    RUN_TEST(SetSensorConfig_writes_sensor_status);

    puts("## GetExposure");
    RUN_TEST(GetExposure_sends_command_to_sensor);
    RUN_TEST(GetExposure_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
    RUN_TEST(GetExposure_waits_for_sensor_to_signal_STATUS_data_ready);
    RUN_TEST(GetExposure_reads_status_from_sensor);
    RUN_TEST(GetExposure_waits_for_sensor_to_signal_EXPOSURE_MSB_data_ready);
    RUN_TEST(GetExposure_reads_exposure_MSB_from_sensor);
    RUN_TEST(GetExposure_waits_for_sensor_to_signal_EXPOSURE_LSB_data_ready);
    RUN_TEST(GetExposure_reads_exposure_LSB_from_sensor);
    RUN_TEST(GetExposure_writes_sensor_status);
    RUN_TEST(GetExposure_writes_exposure_MSB);
    RUN_TEST(GetExposure_writes_exposure_LSB);

    puts("## SetExposure");
    RUN_TEST(SetExposure_waits_for_byte_exposure_MSB);
    RUN_TEST(SetExposure_reads_byte_exposure_MSB);
    RUN_TEST(SetExposure_waits_for_byte_exposure_LSB);
    RUN_TEST(SetExposure_reads_byte_exposure_LSB);
    RUN_TEST(SetExposure_sends_command_to_sensor);
    RUN_TEST(SetExposure_sends_exposure_MSB_to_sensor);
    RUN_TEST(SetExposure_sends_exposure_LSB_to_sensor);
    RUN_TEST(SetExposure_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
    RUN_TEST(SetExposure_waits_for_sensor_to_signal_STATUS_data_ready);
    RUN_TEST(SetExposure_reads_status_from_sensor);
    RUN_TEST(SetExposure_writes_sensor_status);

    puts("## CaptureFrame");
    RUN_TEST(CaptureFrame_sends_command_to_sensor);
    RUN_TEST(CaptureFrame_writes_OK_to_indicate_it_sent_the_command_to_the_sensor);
    RUN_TEST(CaptureFrame_waits_for_sensor_to_signal_STATUS_data_ready);
    RUN_TEST(CaptureFrame_reads_status_from_sensor);
    RUN_TEST(CaptureFrame_waits_for_sensor_to_signal_NUM_PIXELS_MSB_data_ready);
    RUN_TEST(CaptureFrame_reads_num_pixels_MSB_from_sensor);
    RUN_TEST(CaptureFrame_waits_for_sensor_to_signal_NUM_PIXELS_LSB_data_ready);
    RUN_TEST(CaptureFrame_reads_num_pixels_LSB_from_sensor);
    RUN_TEST(CaptureFrame_writes_sensor_status);
    RUN_TEST(CaptureFrame_writes_num_pixels_MSB);
    RUN_TEST(CaptureFrame_writes_num_pixels_LSB);
    RUN_TEST(CaptureFrame_returns_if_status_is_not_OK);
    RUN_TEST(CaptureFrame_waits_for_the_next_byte_of_frame_data);
    RUN_TEST(CaptureFrame_writes_the_next_byte_of_frame_data);
    RUN_TEST(CaptureFrame_loops_wait_for_byte_then_write_byte_for_a_total_nbytes_of_2x_num_pixels);

    puts("## AutoExposure");
    puts("FAIL: Implement Test");

    puts("## GetAutoExposeConfig");
    puts("FAIL: Implement Test");

    puts("## SetAutoExposeConfig");
    puts("FAIL: Implement Test");
}

int main(void)
{
    UNITY_BEGIN();
    test_UsbCmd();
    return UNITY_END();
}
