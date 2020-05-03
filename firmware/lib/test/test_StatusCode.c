#include "unity.h"
#include "test_StatusCode.h"
#include "StatusCode.h"

// led_setting_is_valid(led_state setting) -> bool
void led_setting_is_valid_returns_TRUE_if_setting_is_OFF(void)
{
    TEST_ASSERT_TRUE(led_setting_is_valid(OFF));
}
void led_setting_is_valid_returns_TRUE_if_setting_is_GREEN(void)
{
    TEST_ASSERT_TRUE(led_setting_is_valid(GREEN));
}
void led_setting_is_valid_returns_TRUE_if_setting_is_RED(void)
{
    TEST_ASSERT_TRUE(led_setting_is_valid(RED));
}
void led_setting_is_valid_returns_FALSE_if_setting_is_any_other_value(void)
{
    led_state invalid = 3;
    TEST_ASSERT_TRUE(OFF   != invalid);
    TEST_ASSERT_TRUE(GREEN != invalid);
    TEST_ASSERT_TRUE(RED   != invalid);
    TEST_ASSERT_FALSE(led_setting_is_valid(invalid));
}
