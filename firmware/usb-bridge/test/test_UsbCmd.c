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
