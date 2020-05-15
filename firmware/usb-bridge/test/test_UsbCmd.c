#include "unity.h"          // unity macros: TEST_BLAH
#include "test_UsbCmd.h"
#include "UsbCmd.h"
#include "ReadWriteBits.h"
#include "BiColorLed.h"

/* =====[ GetBridgeLED ]===== */
// ReadLedState() -> led_state
// returns OFF, GREEN, or RED
void ReadLedState_returns_OFF_if_LED_is_off(void)
{
    /* =====[ Setup ]===== */
    // Fake case: Bridge indicator LED is OFF.
    // (i.e., Bridge indicator LED was never turned on)
    uint8_t led_num = status_led; // Bridge LED 0
    ClearBit(BiColorLed_ddr, led_num);
    /* =====[ Test the Setup ]===== */
    // Assert LED is OFF
    TEST_ASSERT_TRUE(BitIsClear(BiColorLed_ddr, led_num));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(OFF, ReadLedState());
}
void ReadLedState_returns_GREEN_if_LED_is_on_and_green(void)
{
    /* =====[ Setup ]===== */
    // Fake case: Bridge indicator LED is on and GREEN.
    uint8_t led_num = status_led; // Bridge LED 0
    BiColorLedOn(led_num); BiColorLedGreen(led_num);
    /* =====[ Test the Setup ]===== */
    // Assert LED is on
    TEST_ASSERT_TRUE(BitIsSet(BiColorLed_ddr, led_num));
    // Assert LED is GREEN
    TEST_ASSERT_TRUE(BitIsClear(BiColorLed_port, led_num));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(GREEN, ReadLedState());
}
void ReadLedState_returns_RED_if_LED_is_on_and_red(void)
{
    /* =====[ Setup ]===== */
    // Fake case: Bridge indicator LED is on and RED.
    uint8_t led_num = status_led; // Bridge LED 0
    BiColorLedOn(led_num); BiColorLedRed(led_num);
    /* =====[ Test the Setup ]===== */
    // Assert LED is on
    TEST_ASSERT_TRUE(BitIsSet(BiColorLed_ddr, led_num));
    // Assert LED is RED
    TEST_ASSERT_TRUE(BitIsSet(BiColorLed_port, led_num));
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(RED, ReadLedState());
}
// GetBridgeLED() -> void
void GetBridgeLED_waits_for_byte_led_num(void)
{
    TEST_PASS();
}
void GetBridgeLED_reads_byte_led_num(void)
{
    TEST_PASS();
}
void GetBridgeLED_writes_two_bytes_to_USB_host(void)
{
    TEST_PASS();
}
void GetBridgeLED_writes_OK_and_LED_STATE_if_led_num_is_valid(void)
{
    TEST_PASS();
}
void GetBridgeLED_writes_ERROR_and_pads_second_byte_if_led_num_is_invalid(void)
{
    TEST_PASS();
}

/* =====[ SetBridgeLED ]===== */
// SetBridgeLED() -> void
void SetBridgeLED_waits_for_byte_led_num(void)
{
    TEST_PASS();
}
void SetBridgeLED_reads_byte_led_num(void)
{
    TEST_PASS();
}
void SetBridgeLED_waits_for_byte_led_setting(void)
{
    TEST_PASS();
}
void SetBridgeLED_reads_byte_led_setting(void)
{
    TEST_PASS();
}
void SetBridgeLED_applies_led_setting_and_writes_OK_if_parameters_are_valid(void)
{
    TEST_PASS();
}
void SetBridgeLED_writes_ERROR_if_led_num_is_invalid(void)
{
    TEST_PASS();
}
void SetBridgeLED_writes_ERROR_if_led_setting_is_invalid(void)
{
    TEST_PASS();
}

/* =====[ GetSensorLED ]===== */
// GetSensorLED() -> void
void GetSensorLED_waits_for_byte_led_num(void)
{
    TEST_PASS();
}
void GetSensorLED_reads_byte_led_num(void)
{
    TEST_PASS();
}
void GetSensorLED_sends_command_to_sensor(void)
{
    TEST_PASS();
}
void GetSensorLED_sends_led_num_to_sensor(void)
{
    TEST_PASS();
}
void GetSensorLED_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void)
{
    TEST_PASS();
}
void GetSensorLED_waits_for_sensor_to_signal_STATUS_data_ready(void)
{
    TEST_PASS();
}
void GetSensorLED_reads_status_from_sensor(void)
{
    TEST_PASS();
}
void GetSensorLED_waits_for_sensor_to_signal_LED_SETTING_data_ready(void)
{
    TEST_PASS();
}
void GetSensorLED_reads_led_setting_from_sensor(void)
{
    TEST_PASS();
}
void GetSensorLED_writes_sensor_status(void)
{
    TEST_PASS();
}
void GetSensorLED_writes_sensor_led_setting(void)
{
    TEST_PASS();
}

/* =====[ SetSensorLED ]===== */
// SetSensorLED() -> void
void SetSensorLED_waits_for_byte_led_num(void)
{
    TEST_PASS();
}
void SetSensorLED_reads_byte_led_num(void)
{
    TEST_PASS();
}
void SetSensorLED_waits_for_byte_led_setting(void)
{
    TEST_PASS();
}
void SetSensorLED_reads_byte_led_setting(void)
{
    TEST_PASS();
}
void SetSensorLED_sends_command_to_sensor(void)
{
    TEST_PASS();
}
void SetSensorLED_sends_led_num_to_sensor(void)
{
    TEST_PASS();
}
void SetSensorLED_sends_led_setting_to_sensor(void)
{
    TEST_PASS();
}
void SetSensorLED_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void)
{
    TEST_PASS();
}
void SetSensorLED_waits_for_sensor_to_signal_STATUS_data_ready(void)
{
    TEST_PASS();
}
void SetSensorLED_reads_status_from_sensor(void)
{
    TEST_PASS();
}
void SetSensorLED_writes_sensor_status(void)
{
    TEST_PASS();
}

/* =====[ SetSensorConfig ]===== */
// SetSensorConfig() -> void
void SetSensorConfig_waits_for_byte_binning(void)
{
    /* =====[ Operate ]===== */
    /* SetSensorConfig(); */
    TEST_PASS();
}
void SetSensorConfig_reads_byte_binning(void)
{
    TEST_PASS();
}
void SetSensorConfig_waits_for_byte_gain(void)
{
    TEST_PASS();
}
void SetSensorConfig_reads_byte_gain(void)
{
    TEST_PASS();
}
void SetSensorConfig_waits_for_byte_row_bitmap(void)
{
    TEST_PASS();
}
void SetSensorConfig_reads_byte_row_bitmap(void)
{
    TEST_PASS();
}
void SetSensorConfig_sends_command_to_sensor(void)
{
    TEST_PASS();
}
void SetSensorConfig_sends_binning_to_sensor(void)
{
    TEST_PASS();
}
void SetSensorConfig_sends_gain_to_sensor(void)
{
    TEST_PASS();
}
void SetSensorConfig_sends_row_bitmap_to_sensor(void)
{
    TEST_PASS();
}
void SetSensorConfig_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void)
{
    TEST_PASS();
}
void SetSensorConfig_waits_for_sensor_to_signal_STATUS_data_ready(void)
{
    TEST_PASS();
}
void SetSensorConfig_reads_status_from_sensor(void)
{
    TEST_PASS();
}
void SetSensorConfig_writes_sensor_status(void)
{
    TEST_PASS();
}

/* =====[ GetSensorConfig ]===== */
// GetSensorConfig() -> void
void GetSensorConfig_sends_command_to_sensor(void)
{
    /* =====[ Operate ]===== */
    /* GetSensorConfig(); */
    TEST_PASS();
}
void GetSensorConfig_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void)
{
    TEST_PASS();
}
void GetSensorConfig_waits_for_sensor_to_signal_STATUS_data_ready(void)
{
    TEST_PASS();
}
void GetSensorConfig_reads_status_from_sensor(void)
{
    TEST_PASS();
}
void GetSensorConfig_waits_for_sensor_to_signal_BINNING_data_ready(void)
{
    TEST_PASS();
}
void GetSensorConfig_reads_binning_from_sensor(void)
{
    TEST_PASS();
}
void GetSensorConfig_waits_for_sensor_to_signal_GAIN_data_ready(void)
{
    TEST_PASS();
}
void GetSensorConfig_reads_gain_from_sensor(void)
{
    TEST_PASS();
}
void GetSensorConfig_waits_for_sensor_to_signal_ROW_BITMAP_data_ready(void)
{
    TEST_PASS();
}
void GetSensorConfig_reads_row_bitmap_from_sensor(void)
{
    TEST_PASS();
}
void GetSensorConfig_writes_sensor_status(void)
{
    TEST_PASS();
}
void GetSensorConfig_writes_sensor_binning(void)
{
    TEST_PASS();
}
void GetSensorConfig_writes_sensor_gain(void)
{
    TEST_PASS();
}
void GetSensorConfig_writes_sensor_row_bitmap(void)
{
    TEST_PASS();
}

/* =====[ GetExposure ]===== */
void GetExposure_sends_command_to_sensor(void)
{
    /* =====[ Operate ]===== */
    /* GetExposure(); */
    TEST_PASS();
}
void GetExposure_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void)
{
    TEST_PASS();
}
void GetExposure_waits_for_sensor_to_signal_STATUS_data_ready(void)
{
    TEST_PASS();
}
void GetExposure_reads_status_from_sensor(void)
{
    TEST_PASS();
}
void GetExposure_waits_for_sensor_to_signal_EXPOSURE_MSB_data_ready(void)
{
    TEST_PASS();
}
void GetExposure_reads_exposure_MSB_from_sensor(void)
{
    TEST_PASS();
}
void GetExposure_waits_for_sensor_to_signal_EXPOSURE_LSB_data_ready(void)
{
    TEST_PASS();
}
void GetExposure_reads_exposure_LSB_from_sensor(void)
{
    TEST_PASS();
}
void GetExposure_writes_sensor_status(void)
{
    TEST_PASS();
}
void GetExposure_writes_exposure_MSB(void)
{
    TEST_PASS();
}
void GetExposure_writes_exposure_LSB(void)
{
    TEST_PASS();
}

/* =====[ SetExposure ]===== */
// SetExposure() -> void
void SetExposure_waits_for_byte_exposure_MSB(void)
{
    /* =====[ Operate ]===== */
    /* SetExposure(); */
    TEST_PASS();
}
void SetExposure_reads_byte_exposure_MSB(void)
{
    TEST_PASS();
}
void SetExposure_waits_for_byte_exposure_LSB(void)
{
    /* =====[ Operate ]===== */
    TEST_PASS();
}
void SetExposure_reads_byte_exposure_LSB(void)
{
    TEST_PASS();
}
void SetExposure_sends_command_to_sensor(void)
{
    TEST_PASS();
}
void SetExposure_sends_exposure_MSB_to_sensor(void)
{
    TEST_PASS();
}
void SetExposure_sends_exposure_LSB_to_sensor(void)
{
    TEST_PASS();
}
void SetExposure_writes_OK_to_indicate_it_sent_the_command_to_the_sensor(void)
{
    TEST_PASS();
}
void SetExposure_waits_for_sensor_to_signal_STATUS_data_ready(void)
{
    TEST_PASS();
}
void SetExposure_reads_status_from_sensor(void)
{
    TEST_PASS();
}
void SetExposure_writes_sensor_status(void)
{
    TEST_PASS();
}

