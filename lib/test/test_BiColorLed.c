#include "test_BiColorLed.h"
#include <BiColorLed.h>
#include <unity.h>

void BiColorLedRed_is_a_high_on_led_pin(void)
{
    /* =====[ Setup ]===== */
    *BiColorLed_port = 0x00;
    /* =====[ Operate ]===== */
    BiColorLedRed(status_led);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(status_led,*BiColorLed_port);
}
void BiColorLedGreen_is_a_low_on_led_pin(void)
{
    /* =====[ Setup ]===== */
    *BiColorLed_port = 0xFF;
    /* =====[ Operate ]===== */
    BiColorLedGreen(status_led);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(status_led,*BiColorLed_port);
}
void BiColorLedToggleColor_changes_green_to_red(void)
{
    /* =====[ Setup ]===== */
    *BiColorLed_port = 0x00;
    /* =====[ Operate ]===== */
    BiColorLedToggleColor(status_led);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(status_led,*BiColorLed_port);
}
void BiColorLedToggleColor_changes_red_to_green(void)
{
    /* =====[ Setup ]===== */
    *BiColorLed_port = 0xFF;
    /* =====[ Operate ]===== */
    BiColorLedToggleColor(status_led);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(status_led,*BiColorLed_port);
}
void BiColorLedOff_is_a_low_on_led_pin_and_data_direction(void)
{
    /* =====[ Setup ]===== */
    *BiColorLed_port = 0xFF;
    *BiColorLed_ddr = 0xFF;
    /* =====[ Operate ]===== */
    BiColorLedOff(status_led);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(status_led, *BiColorLed_port);
    TEST_ASSERT_BIT_LOW(status_led, *BiColorLed_ddr);
}
void BiColorLedOn_is_low_on_led_pin_but_high_on_led_pin_data_direction(void)
{
    /* =====[ Setup ]===== */
    *BiColorLed_port = 0x00;
    *BiColorLed_ddr = 0x00;
    /* =====[ Operate ]===== */
    BiColorLedOn(status_led);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(status_led, *BiColorLed_port);
    TEST_ASSERT_BIT_HIGH(status_led, *BiColorLed_ddr);
}
