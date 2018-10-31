#include "test_DebugLeds.h"
#include <unity.h>              // testing framework
#include "DebugLeds.h"          // lib under test
/* =====[ List of Tests ]===== */
// Init
    // [x] DebugLedsTurnOn_led_N_turns_on_led_N
    // [ ] DebugLedsTurnAllOn_turns_on_all_4_leds
    // [x] DebugLedsTurnRed_led_N_turns_led_N_red
    // [x] DebugLedsTurnGreen_led_N_turns_led_N_green
    // [x] DebugLedsTurnAllRed_turns_all_4_leds_red
    // [x] DebugLedsTurnAllGreen_turns_all_4_leds_green
    // [ ] DebugLedsToggleAll_toggles_all_the_leds
uint8_t const debug_led1 = 0;
uint8_t const debug_led2 = 1;
uint8_t const debug_led3 = 2;
uint8_t const debug_led4 = 3;

uint8_t fake_DebugLeds_ddr;
uint8_t fake_DebugLeds_port;
uint8_t volatile * const DebugLeds_ddr = &fake_DebugLeds_ddr;
uint8_t volatile * const DebugLeds_port = &fake_DebugLeds_port;

void SetUp_DebugLeds(void)
{
    *DebugLeds_ddr = 0x00;
    *DebugLeds_port = 0x00;
}
void TearDown_DebugLeds(void)
{}
void DebugLedsTurnOn_led_N_turns_on_led_N(void)
{
    uint8_t const debug_ledN = debug_led1;
    /* =====[ Operate ]===== */
    DebugLedsTurnOn(debug_ledN);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(debug_ledN, *DebugLeds_ddr);
}
void DebugLedsTurnAllOn_turns_on_all_4_leds(void)
{
    /* =====[ Operate ]===== */
    DebugLedsTurnAllOn();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(debug_led1, *DebugLeds_ddr);
    TEST_ASSERT_BIT_HIGH(debug_led2, *DebugLeds_ddr);
    TEST_ASSERT_BIT_HIGH(debug_led3, *DebugLeds_ddr);
    TEST_ASSERT_BIT_HIGH(debug_led4, *DebugLeds_ddr);
}
void DebugLedsTurnRed_led_N_turns_led_N_red(void)
{
    uint8_t const debug_ledN = debug_led1;
    /* =====[ Operate ]===== */
    DebugLedsTurnRed(debug_ledN);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(debug_ledN, *DebugLeds_port);
}
void DebugLedsTurnGreen_led_N_turns_led_N_green(void)
{
    uint8_t const debug_ledN = debug_led1;
    *DebugLeds_port = 0xFF;
    /* =====[ Operate ]===== */
    DebugLedsTurnGreen(debug_ledN);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(debug_ledN, *DebugLeds_port);
}
void DebugLedsTurnAllRed_turns_all_4_leds_red(void)
{
    /* =====[ Operate ]===== */
    DebugLedsTurnAllRed();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(debug_led1, *DebugLeds_port);
    TEST_ASSERT_BIT_HIGH(debug_led2, *DebugLeds_port);
    TEST_ASSERT_BIT_HIGH(debug_led3, *DebugLeds_port);
    TEST_ASSERT_BIT_HIGH(debug_led4, *DebugLeds_port);
}
void DebugLedsTurnAllGreen_turns_all_4_leds_green(void)
{
    *DebugLeds_port = 0xFF;
    /* =====[ Operate ]===== */
    DebugLedsTurnAllGreen();
    TEST_ASSERT_BIT_LOW(debug_led1, *DebugLeds_port);
    TEST_ASSERT_BIT_LOW(debug_led2, *DebugLeds_port);
    TEST_ASSERT_BIT_LOW(debug_led3, *DebugLeds_port);
    TEST_ASSERT_BIT_LOW(debug_led4, *DebugLeds_port);
}
void DebugLedsToggleAll_toggles_all_the_leds(void)
{
    /* =====[ Setup ]===== */
    *DebugLeds_port = 0xFF;
    /* =====[ Operate ]===== */
    DebugLedsToggleAll();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(debug_led1, *DebugLeds_port);
    TEST_ASSERT_BIT_LOW(debug_led2, *DebugLeds_port);
    TEST_ASSERT_BIT_LOW(debug_led3, *DebugLeds_port);
    TEST_ASSERT_BIT_LOW(debug_led4, *DebugLeds_port);
    /* =====[ Setup ]===== */
    *DebugLeds_port = 0x00;
    /* =====[ Operate ]===== */
    DebugLedsToggleAll();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(debug_led1, *DebugLeds_port);
    TEST_ASSERT_BIT_HIGH(debug_led2, *DebugLeds_port);
    TEST_ASSERT_BIT_HIGH(debug_led3, *DebugLeds_port);
    TEST_ASSERT_BIT_HIGH(debug_led4, *DebugLeds_port);
}

