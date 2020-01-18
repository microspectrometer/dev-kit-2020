#include "unity.h"
#include "test_Lis.h"
#include "Lis.h"

/* =====[ LisInit ]===== */
void LisInit_sets_PixSelect_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr2 = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_PixSelect,
        *Lis_ddr2,
        "PixSelect must be an input pin when the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_PixSelect, *Lis_ddr2);
}
void LisInit_idles_PixSelect_low(void)
{
    /* =====[ Setup ]===== */
    *Lis_port2 = 0xFF;
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Lis_PixSelect,
        *Lis_port2,
        "PixSelect must be HIGH when the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_PixSelect, *Lis_port2);
}
void LisInit_sets_Clk_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr1 = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_Clk,
        *Lis_ddr1,
        "Clk must be an input pin when the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_Clk, *Lis_ddr1);
}
void LisInit_sets_Rst_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Lis_ddr1 = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_Rst,
        *Lis_ddr1,
        "Rst must be an input pin when the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_ddr1);
}
void LisInit_idles_Rst_low(void)
{
    /* =====[ Setup ]===== */
    *Lis_port1 = 0xFF;
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Lis_Rst,
        *Lis_port1,
        "Rst must be HIGH when the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void LisInit_sets_Sync_as_an_input(void)
{
    *Lis_ddr1 = 0xFF;
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Lis_Sync,
        *Lis_ddr1,
        "Sync must be an output pin when the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_Sync, *Lis_ddr1);
}
void LisInit_resets_PWM_timer_at_top(void)
{
    /* =====[ Setup ]===== */
    *Lis_TCCR0A = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_WGM00,
        *Lis_TCCR0A,
        "WGM00 must be clear when the test starts."
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_WGM01,
        *Lis_TCCR0A,
        "WGM01 must be clear when the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_WGM00, *Lis_TCCR0A);
    TEST_ASSERT_BIT_HIGH(Lis_WGM01, *Lis_TCCR0A);
}
void LisInit_PWM_timer_top_is_OCR0A(void)
{
    /* =====[ Setup ]===== */
    *Lis_TCCR0B = 0x00;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_WGM02,
        *Lis_TCCR0B,
        "WGM02 must be clear before test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_WGM02, *Lis_TCCR0B);
}
void LisInit_PWM_timer_is_clocked_by_CPU_with_no_prescaling(void)
{
    /* =====[ Setup ]===== */
    *Lis_TCCR0B = 0x06;
    TEST_ASSERT_BIT_LOW_MESSAGE(
            Lis_CS00,
            *Lis_TCCR0B,
            "CS00 must be clear before test starts."
            );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
            Lis_CS01,
            *Lis_TCCR0B,
            "CS01 must be set before test starts."
            );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
            Lis_CS02,
            *Lis_TCCR0B,
            "CS02 must be set before test starts."
            );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_CS00, *Lis_TCCR0B);
    TEST_ASSERT_BIT_LOW(Lis_CS01, *Lis_TCCR0B);
    TEST_ASSERT_BIT_LOW(Lis_CS02, *Lis_TCCR0B);
}
void LisInit_sets_PWM_frequency_at_50kHz(void)
{
    /* =====[ Setup ]===== */
    *Lis_OCR0A = 0x00;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    int fcpu = 10e6; int fclk = 50e3;
    // 10.0e6/50.0e3 = 200.0 tics
    TEST_ASSERT_EQUAL_UINT8(fcpu/fclk, *Lis_OCR0A);
}
void LisInit_sets_PWM_duty_cycle_to_50_percent(void)
{
    /* =====[ Setup ]===== */
    *Lis_OCR0B = 0;
    *Lis_OCR0A = 0;
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    float duty_cycle = (float)(*Lis_OCR0B)/(float)(*Lis_OCR0A);
    TEST_ASSERT_EQUAL_FLOAT(0.5, duty_cycle);
}
void LisInit_outputs_the_PWM_clock_on_pin_Clk(void)
{
    /* =====[ Setup ]===== */
    *Lis_TCCR0A = 0xDF;
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Lis_COM0B0, *Lis_TCCR0A,
        "COM0B0 must be HIGH before the test starts."
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_COM0B1, *Lis_TCCR0A,
        "COM0B1 must be LOW before the test starts."
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_COM0B0, *Lis_TCCR0A);
    TEST_ASSERT_BIT_HIGH(Lis_COM0B1, *Lis_TCCR0A);
}

/* =====[ LisConfigIsValid ]===== */
void LisConfigIsValid_returns_false_if_binning_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake some config values.
    binning = 0xFF;
    gain = GAIN_1X;
    active_rows = ALL_ROWS_ACTIVE;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        !LisConfigIsValid(),
        "Binning is invalid: expect `LisConfigIsValid` returns false."
        );
}
void LisConfigIsValid_returns_false_if_gain_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake some config values.
    binning = BINNING_ON;
    gain = 0xFF;
    active_rows = ALL_ROWS_ACTIVE;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        !LisConfigIsValid(),
        "Gain is invalid: expect `LisConfigIsValid` returns false."
        );
}
void LisConfigIsValid_returns_false_if_active_rows_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake some config values.
    binning = BINNING_OFF;
    gain = GAIN_2X5;
    active_rows = 0xFF;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        !LisConfigIsValid(),
        "Rows is invalid: expect `LisConfigIsValid` returns false."
        );
}
void LisConfigIsValid_returns_true_if_config_is_valid(void)
{
    /* =====[ Setup ]===== */
    // Fake some config values.
    binning = BINNING_OFF;
    gain = GAIN_2X5;
    active_rows = ALL_ROWS_ACTIVE;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        LisConfigIsValid(),
        "Config is valid: expect `LisConfigIsValid` returns true."
        );
}

/* =====[ LisWriteConfig ]===== */
void LisWriteConfig_converts_config_to_28bit_sequence(void){}
void LisWriteConfig_enters_LIS_programming_mode(void){}
void LisWriteConfig_writes_28bits_to_LIS_setup_register(void){}
void LisWriteConfig_exits_LIS_programming_mode(void){}
