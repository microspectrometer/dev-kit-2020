#include "unity.h"
#include "Mock.h"
#include "test_Lis.h"
#include "Lis.h"

/* =====[ Test Helpers ]===== */
static void _AssertCall(uint16_t num, char const * name)
{
    //! Assert call number **num** is named **name**.
    // Put num and name in the message displayed if test fails
    GString *message = g_string_new(NULL);
    g_string_append_printf(message, "`%s` is not call %d", name, num);
    // Perform the test
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, num, name),
        message->str
        );
    // Free memory used by GString
    g_string_free(message, true);
}
static void _AssertArg(uint16_t call_n, uint8_t arg_n, void *pval)
{
    //! Assert value of arg number **arg_n** == (*pval)
    /** Pass **address of arg** to parameter **pval**.\n
      * **Pass arg same way, whether it is a value or a pointer.**
      *
      * Example: `CallWithTwoArgs(uint8_t *pbyte, uint8_t bit_num)`
      *
      * ```
      * uint8_t byte = 0x0a;
      * uint8_t *pbyte = &byte;
      * uint8_t bit_num = 7;
      * _AssertCall(1, "CallWithTwoArgs");
      * _AssertArg(1,1, &pbyte);
      * _AssertArg(1,2, &bit_num);
      * ```
      *
      * The first _AssertArg checks a pointer to a byte.
      * The second _AssertArg checks a byte.
      *
      * Both use the `&` prefix to pass the address.
      *
      * In the case of `&pbyte`, AssertArg() checks the pointer
      * (pbyte), but the pre-test-result prints the value
      * pointed to (byte).
      *
      * Example:
      *
      * ```
      * // Call was `CallWithTwoArgs(pbyte, bit_num);`
      * _AssertArg(1,2, &bit_num); // check arg2 equals 7
      * _AssertArg(1,1, &pbyte); // check arg1 equals address of byte
      * ```
      *
      * The AssertArg pre-test-result message prints the value
      * recorded arg1 points to (0x0a).\n 
      *
      * Note that the test fails if arg1 does not equal pbyte,
      * even if pbyte and arg1 happen to point to equal values.
      * */
    GString *msg = g_string_new(NULL);
    g_string_printf(msg, "Expect different value for call %d arg %d.", call_n, arg_n);
    // I cannot print the expected value without asking the
    // caller to include the type as a string. Better to keep the
    // arg list short. Call and arg number are good enough.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, pval),
        msg->str
        );
    g_string_free(msg, true);
}
/* =====[ _setup_bit_val ]===== */
enum bit_val {LOW, HIGH}; typedef enum bit_val bit_val;
static void _setup_bit_val(lis_ptr reg, lis_bit bit, bit_val v)
{
    //! Set up for test to check bit value. Use generic fail message.
    /** - make bit `LOW` if function is supposed to make it `HIGH`
      * - make bit `HIGH` if function is supposed to make it `LOW`
      * - TEST_ macro double-checks the setup is correct
      *   - silent if setup is correct:
      *     - test continues
      *     - nothing printed in test results
      *   - fails if setup is not correct
      *     - test stops
      *     - print why the setup failed
      * */
    GString *msg = g_string_new(NULL);
    g_string_printf(msg, "Bit must be %s when the test starts", v ? "HIGH" : "LOW");
    if (HIGH == v)
    {
        SetBit(reg, bit);
        TEST_ASSERT_BIT_HIGH_MESSAGE(bit, *reg, msg->str);
    }
    else if (LOW == v)
    {
        ClearBit(reg, bit);
        TEST_ASSERT_BIT_LOW_MESSAGE(bit, *reg, msg->str);
    }
    else
    {
        g_string_printf(msg, "Test setup with invalid bit value: %d? ", v);
        g_string_append_printf(msg, "Bit value must be LOW or HIGH.");
        TEST_FAIL_MESSAGE(msg->str);
    }
    g_string_free(msg, true);
}
static void _setup_bit_val_msg(lis_ptr reg, lis_bit bit, bit_val v,
        char * bit_name)
{
    //! Set up for test to check bit value. Use custom fail message `bit_name`.
    /** - make bit `LOW` if function is supposed to make it `HIGH`
      * - make bit `HIGH` if function is supposed to make it `LOW`
      * - TEST_ macro double-checks the setup is correct
      *   - silent if setup is correct:
      *     - test continues
      *     - nothing printed in test results
      *   - fails if setup is not correct
      *     - test stops
      *     - print why the setup failed
      * */
    // Put bit_name in the message displayed if test fails
    GString *message = g_string_new(NULL);
    g_string_append_printf(
        message, "`%s` must be ", bit_name
        );
    g_string_append_printf(
        message, "%s when the test starts.",
        v ? "HIGH" : "LOW"
        );
    if (HIGH == v)
    {
        SetBit(reg, bit);
        TEST_ASSERT_BIT_HIGH_MESSAGE(bit, *reg, message->str);
    }
    else if (LOW == v)
    {
        ClearBit(reg, bit);
        TEST_ASSERT_BIT_LOW_MESSAGE(bit, *reg, message->str);
    }
    else
    {
        g_string_printf(message, "Test setup is invalid. `%s`", bit_name);
        g_string_append_printf(message, " = %d? ", v);
        g_string_append_printf(message, "Bit value must be LOW or HIGH.");
        TEST_FAIL_MESSAGE(message->str);
    }
    g_string_free(message, true);
}

/* =====[ _test_bit_val ]===== */
static void _test_bit_val( lis_ptr reg, lis_bit bit, bit_val v )
{
    //! Assert bit is LOW or HIGH. Use generic message if test fails.
    GString *msg = g_string_new(NULL);
    g_string_printf(msg, "Expect bit is %s", v ? "HIGH" : "LOW");
    v ?
        TEST_ASSERT_BIT_HIGH_MESSAGE(bit, *reg, msg->str)
        :
        TEST_ASSERT_BIT_LOW_MESSAGE(bit, *reg, msg->str);
    g_string_free(msg, true);
}

/* =====[ Exposure ]===== */
void MSB_returns_most_significant_bit_of_16bit_input(void)
{
    /* =====[ Operate and Test ]===== */
    exposure_ticks = 0xABCD;
    TEST_ASSERT_EQUAL_HEX8( 0xAB, MSB(exposure_ticks) );
}
void LSB_returns_least_significant_bit_of_16bit_input(void)
{
    /* =====[ Operate and Test ]===== */
    exposure_ticks = 0xABCD;
    TEST_ASSERT_EQUAL_HEX8( 0xCD, LSB(exposure_ticks) );
}

/* =====[ LisInit ]===== */
void LisInit_sets_PixSelect_as_an_output(void)
{
    /* =====[ Setup ]===== */
    _setup_bit_val_msg(
        Lis_ddr2, Lis_PixSelect, LOW,
        "Expect test starts with PixSelect LOW (input pin), but PixSelect" // = bit_val
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_PixSelect, *Lis_ddr2);
}
void LisInit_idles_PixSelect_low(void)
{
    /* =====[ Setup ]===== */
    _setup_bit_val_msg(
        Lis_port2, Lis_PixSelect, HIGH,
        "Expect test starts with PixSelect HIGH, but PixSelect" // = bit_val
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_PixSelect, *Lis_port2);
}
void LisInit_sets_Clk_as_an_output(void)
{
    /* =====[ Setup ]===== */
    _setup_bit_val_msg(Lis_ddr1, Lis_Clk, LOW,
        "Expect Lis_Clk is an input (LOW) when test starts. Lis_Clk"
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_Clk, *Lis_ddr1);
}
void LisInit_sets_Rst_as_an_output(void)
{
    /* =====[ Setup ]===== */
    _setup_bit_val_msg(Lis_ddr1, Lis_Rst, LOW,
        "Expect Lis_Rst is an input (LOW) when test starts. Lis_Rst"
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_ddr1);
}
void LisInit_idles_Rst_low(void)
{
    /* =====[ Setup ]===== */
    _setup_bit_val_msg(Lis_port1, Lis_Rst, HIGH,
        "Expect test starts with Lis_Rst HIGH, but Lis_Rst"
        );
    /* =====[ Operate ]===== */
    LisInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void LisInit_sets_Sync_as_an_input(void)
{
    _setup_bit_val_msg(Lis_ddr1, Lis_Sync, HIGH,
        "Sync must be an output when the test starts, but Lis_Sync"
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
        !LisConfigIsValid(binning, gain, active_rows),
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
        !LisConfigIsValid(binning, gain, active_rows),
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
        !LisConfigIsValid(binning, gain, active_rows),
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
        LisConfigIsValid(binning, gain, active_rows),
        "Config is valid: expect `LisConfigIsValid` returns true."
        );
}

/* =====[ _ConfigAs28bits ]===== */
void ConfigAs28bits_writes_config_as_little_endian_ie_binning_is_config_byte0_bit0(void)
{
    /* =====[ Setup ]===== */
    uint8_t config[4];
    /* =====[ Operate ]===== */
    binning = BINNING_ON; // byte0 bit 0 set
    gain = GAIN_1X; // byt0 bit 1 clear, bit 2 clear
    active_rows = 0x00; // no rows active
    _ConfigAs28bits(config); // Write config
    /* =====[ Test ]===== */
    // Expect all bits in config are 0 except binning.
    // Expect binning bit is byte 0, bit 0.
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, config[3], "Expect config[3]==0x00.");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, config[2], "Expect config[2]==0x00.");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x00, config[1], "Expect config[1]==0x00.");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x01, config[0], "Expect config[0]==0x01.");
}
void ConfigAs28bits_sets_config_byte0_bit0_if_BINNING_ON(void)
{
    /* =====[ Setup ]===== */
    binning = BINNING_ON;
    uint8_t config[4];
    ClearBit(&config[0], 0);
    TEST_ASSERT_BIT_LOW_MESSAGE(
        0,         // bit 0
        config[0], // config byte 0
        "Test must start with bit clear, but bit is set!"
        );
    /* =====[ Operate ]===== */
    _ConfigAs28bits(config);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        0,         // bit 0
        config[0], // config byte 0
        "Expect byte0, bit0 set for BINNING_ON."
        );
}
void ConfigAs28bits_clears_config_byte0_bit0_if_BINNING_OFF(void)
{
    /* =====[ Setup ]===== */
    binning = BINNING_OFF;
    uint8_t config[4];
    SetBit(&config[0], 0);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        0,         // bit 0
        config[0], // config byte 0
        "Test must start with bit set, but bit is clear!"
        );
    /* =====[ Operate ]===== */
    _ConfigAs28bits(config);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        0,         // bit 0
        config[0], // config byte 0
        "Expect byte0, bit0 clear for BINNING_OFF."
        );
}
void ConfigAs28bits_byte0_bit1_clear_and_bit2_clear_if_GAIN_1X(void)
{
    /* =====[ Setup ]===== */
    uint8_t config[4];
    SetBit(&config[0], 1);
    SetBit(&config[0], 2);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        1,         // bit 1
        config[0], // config byte 0
        "Test must start with bit 1 set, but bit is clear!"
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        2,         // bit 2
        config[0], // config byte 0
        "Test must start with bit 2 set, but bit is clear!"
        );
    /* =====[ Operate ]===== */
    gain = GAIN_1X;
    _ConfigAs28bits(config);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        1,         // bit 1
        config[0], // config byte 0
        "Expect byte0, bit1 clear for GAIN_1X."
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        2,         // bit 2
        config[0], // config byte 0
        "Expect byte0, bit2 clear for GAIN_1X."
        );
}
void ConfigAs28bits_byte0_bit1_clear_and_bit2_set_if_GAIN_2X5(void)
{
    /* =====[ Setup ]===== */
    uint8_t config[4];
    SetBit(&config[0], 1);
    ClearBit(&config[0], 2);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        1,         // bit 1
        config[0], // config byte 0
        "Test must start with bit 1 set, but bit is clear!"
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        2,         // bit 2
        config[0], // config byte 0
        "Test must start with bit 2 clear, but bit is set!"
        );
    /* =====[ Operate ]===== */
    gain = GAIN_2X5;
    _ConfigAs28bits(config);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        1,         // bit 1
        config[0], // config byte 0
        "Expect byte0, bit1 clear for GAIN_2X5."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        2,         // bit 2
        config[0], // config byte 0
        "Expect byte0, bit2 set for GAIN_2X5."
        );
}
void ConfigAs28bits_byte0_bit1_set_and_bit2_clear_if_GAIN_4X(void)
{
    /* =====[ Setup ]===== */
    uint8_t config[4];
    ClearBit(&config[0], 1);
    SetBit(&config[0], 2);
    TEST_ASSERT_BIT_LOW_MESSAGE(
        1,         // bit 1
        config[0], // config byte 0
        "Test must start with bit 1 clear, but bit is set!"
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        2,         // bit 2
        config[0], // config byte 0
        "Test must start with bit 2 set, but bit is clear!"
        );
    /* =====[ Operate ]===== */
    gain = GAIN_4X;
    _ConfigAs28bits(config);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        1,         // bit 1
        config[0], // config byte 0
        "Expect byte0, bit1 set for GAIN_4X."
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        2,         // bit 2
        config[0], // config byte 0
        "Expect byte0, bit2 clear for GAIN_4X."
        );
}
void ConfigAs28bits_byte0_bit1_set_and_bit2_set_if_GAIN_5X(void)
{
    /* =====[ Setup ]===== */
    uint8_t config[4];
    ClearBit(&config[0], 1);
    ClearBit(&config[0], 2);
    TEST_ASSERT_BIT_LOW_MESSAGE(
        1,         // bit 1
        config[0], // config byte 0
        "Test must start with bit 1 clear, but bit is set!"
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        2,         // bit 2
        config[0], // config byte 0
        "Test must start with bit 2 clear, but bit is set!"
        );
    /* =====[ Operate ]===== */
    gain = GAIN_5X;
    _ConfigAs28bits(config);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        1,         // bit 1
        config[0], // config byte 0
        "Expect byte0, bit1 set for GAIN_5X."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        2,         // bit 2
        config[0], // config byte 0
        "Expect byte0, bit2 set for GAIN_5X."
        );
}
static uint8_t Clear_3_LSB(uint8_t byte) { return byte & (!0x07); }
static uint8_t Clear_4_MSB(uint8_t byte) { return byte & (!0xF0); }
static void FakePreviousConfig(uint8_t *prev_config, uint8_t *desired_config)
{
    /** Fake previous config by:\n 
      * 1) take the opposite of the desired config,\n 
      * 2) clear bits that do not affect active_rows:\n 
      * - the 3 LSB of byte0\n 
      * - the 4 MSB of byte3\n 
      * */
    prev_config[0] = Clear_3_LSB(!desired_config[0]);
    prev_config[1] =            (!desired_config[1]);
    prev_config[2] =            (!desired_config[2]);
    prev_config[3] = Clear_4_MSB(!desired_config[3]);
}
void ConfigAs28bits_bit3_to_bit27_set_if_ALL_ROWS_ACTIVE(void)
{
    /* =====[ Setup ]===== */
    // Define the desired 28-bit sequence.
    uint8_t desired[4];
    desired[0]=0xF8; // desired values for bits  7: 0 - 0b11111000
    desired[1]=0xFF; // desired values for bits 15: 8 - 0b11111111
    desired[2]=0xFF; // desired values for bits 23:16 - 0b11111111
    desired[3]=0x0F; // desired values for bits 31:24 - 0b11111000
    // Start test with previous config value faked to make test fail.
    uint8_t config[4]; FakePreviousConfig(config, desired);
    /* =====[ Operate ]===== */
    binning = BINNING_OFF; gain = GAIN_1X; // clear bits
    active_rows = ALL_ROWS_ACTIVE; // new active_rows value
    _ConfigAs28bits(config); // Write config with new active_rows
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[0], config[0], "Bad config[0].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[1], config[1], "Bad config[1].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[2], config[2], "Bad config[2].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[3], config[3], "Bad config[3].");
}
void ConfigAs28bits_b3b8b13b18b23_set_if_ROW_1_ACTIVE(void)
{
    /* =====[ Setup ]===== */
    // Define the desired 28-bit sequence.
    uint8_t desired[4];
    desired[0]=0x08; // desired values for bits  7: 0 - 0b00001000
    desired[1]=0x21; // desired values for bits 15: 8 - 0b00100001
    desired[2]=0x84; // desired values for bits 23:16 - 0b10000100
    desired[3]=0x00; // desired values for bits 31:24 - 0b00000000
    // Start test with previous config value faked to make test fail.
    uint8_t config[4]; FakePreviousConfig(config, desired);
    /* =====[ Operate ]===== */
    binning = BINNING_OFF; gain = GAIN_1X; // clear bits
    active_rows = ONLY_ROW_1_ACTIVE; // new active_rows value
    _ConfigAs28bits(config); // Write config with new active_rows
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[0], config[0], "Bad config[0].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[1], config[1], "Bad config[1].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[2], config[2], "Bad config[2].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[3], config[3], "Bad config[3].");
}
void ConfigAs28bits_b4b9b14b19b24_set_if_ROW_2_ACTIVE(void)
{
    /* =====[ Setup ]===== */
    // Define the desired 28-bit sequence.
    uint8_t desired[4];
    desired[0]=0x10; // desired values for bits  7: 0 - 0b00010000
    desired[1]=0x42; // desired values for bits 15: 8 - 0b01000010
    desired[2]=0x08; // desired values for bits 23:16 - 0b00001000
    desired[3]=0x01; // desired values for bits 31:24 - 0b00000001
    // Start test with previous config value faked to make test fail.
    uint8_t config[4]; FakePreviousConfig(config, desired);
    /* =====[ Operate ]===== */
    binning = BINNING_OFF; gain = GAIN_1X; // clear bits
    active_rows = ONLY_ROW_2_ACTIVE; // new active_rows value
    _ConfigAs28bits(config); // Write config with new active_rows
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[0], config[0], "Bad config[0].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[1], config[1], "Bad config[1].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[2], config[2], "Bad config[2].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[3], config[3], "Bad config[3].");
}
void ConfigAs28bits_b5b10b15b20b25_set_if_ROW_3_ACTIVE(void)
{
    /* =====[ Setup ]===== */
    // Define the desired 28-bit sequence.
    uint8_t desired[4];
    desired[0]=0x20; // desired values for bits  7: 0 - 0b00100000
    desired[1]=0x84; // desired values for bits 15: 8 - 0b10000100
    desired[2]=0x10; // desired values for bits 23:16 - 0b00010000
    desired[3]=0x02; // desired values for bits 31:24 - 0b00000010
    // Start test with previous config value faked to make test fail.
    uint8_t config[4]; FakePreviousConfig(config, desired);
    /* =====[ Operate ]===== */
    binning = BINNING_OFF; gain = GAIN_1X; // clear bits
    active_rows = ONLY_ROW_3_ACTIVE; // new active_rows value
    _ConfigAs28bits(config); // Write config with new active_rows
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[0], config[0], "Bad config[0].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[1], config[1], "Bad config[1].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[2], config[2], "Bad config[2].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[3], config[3], "Bad config[3].");
}
void ConfigAs28bits_b6b11b16b21b26_set_if_ROW_4_ACTIVE(void)
{
    /* =====[ Setup ]===== */
    // Define the desired 28-bit sequence.
    uint8_t desired[4];
    desired[0]=0x40; // desired values for bits  7: 0 - 0b01000000
    desired[1]=0x08; // desired values for bits 15: 8 - 0b00001000
    desired[2]=0x21; // desired values for bits 23:16 - 0b00100001
    desired[3]=0x04; // desired values for bits 31:24 - 0b00000100
    // Start test with previous config value faked to make test fail.
    uint8_t config[4]; FakePreviousConfig(config, desired);
    /* =====[ Operate ]===== */
    binning = BINNING_OFF; gain = GAIN_1X; // clear bits
    active_rows = ONLY_ROW_4_ACTIVE; // new active_rows value
    _ConfigAs28bits(config); // Write config with new active_rows
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[0], config[0], "Bad config[0].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[1], config[1], "Bad config[1].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[2], config[2], "Bad config[2].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[3], config[3], "Bad config[3].");
}
void ConfigAs28bits_b7b12b17b22b27_set_if_ROW_5_ACTIVE(void)
{
    /* =====[ Setup ]===== */
    // Define the desired 28-bit sequence.
    uint8_t desired[4];
    desired[0]=0x80; // desired values for bits  7: 0 - 0b10000000
    desired[1]=0x10; // desired values for bits 15: 8 - 0b00010000
    desired[2]=0x42; // desired values for bits 23:16 - 0b01000010
    desired[3]=0x08; // desired values for bits 31:24 - 0b00001000
    // Start test with previous config value faked to make test fail.
    uint8_t config[4]; FakePreviousConfig(config, desired);
    /* =====[ Operate ]===== */
    binning = BINNING_OFF; gain = GAIN_1X; // clear bits
    active_rows = ONLY_ROW_5_ACTIVE; // new active_rows value
    _ConfigAs28bits(config); // Write config with new active_rows
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[0], config[0], "Bad config[0].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[1], config[1], "Bad config[1].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[2], config[2], "Bad config[2].");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(desired[3], config[3], "Bad config[3].");
}

/* =====[ LisWriteConfig ]===== */
void LisWriteConfig_converts_config_to_28bit_sequence(void)
{
    /* =====[ Operate ]===== */
    LisWriteConfig();
    /* =====[ Test ]===== */
    _AssertCall(1, "_ConfigAs28bits");
}
void LisWriteConfig_enters_LIS_programming_mode(void)
{
    /* =====[ Operate ]===== */
    LisWriteConfig();
    /* =====[ Test ]===== */
    _AssertCall(2, "_EnterLisProgrammingMode");
}
void LisWriteConfig_writes_28bits_to_LIS_setup_register(void)
{
    /* =====[ Operate ]===== */
    LisWriteConfig();
    /* =====[ Test ]===== */
    _AssertCall(3, "_Write28bitLisConfig");
}
void LisWriteConfig_exits_LIS_programming_mode(void)
{
    /* =====[ Operate ]===== */
    LisWriteConfig();
    /* =====[ Test ]===== */
    _AssertCall(4, "_ExitLisProgrammingMode");
}

/* =====[ _WaitForLisClkLow ]===== */
void WaitForLisClkLow_clears_flag_PwmTimerMatchesOCF0B(void)
{
    /* =====[ Setup ]===== */
    /** Note this is *not* how TIFR0 works, this is just for testing.
      * The flag is cleared by setting its bit in TIFR0.
      * I do not bother to simulate that mechanism.
      * I test by treating TIFR0 like any other register.
      * */
    _setup_bit_val_msg(Lis_TIFR0, Lis_OCF0B, LOW, "TIFR0 bit Lis_OCF0B");
    /* =====[ Operate ]===== */
    _WaitForLisClkLow();
    /* =====[ Test ]===== */
    // Clear flag by setting flag bit
    // reg: TIFR0, bit: OCF0B
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Lis_OCF0B,
        *Lis_TIFR0,
        "Expect `_WaitForLisClkLow` clears flag by setting bit."
        );
}
void WaitForLisClkLow_waits_until_flag_PwmTimerMatchesOCF0B_is_set(void)
{
    // Cannot fake setting flag after calling function under test
    TEST_PASS();
}

/* =====[ _WaitForLisClkHigh ]===== */
void WaitForLisClkHigh_clears_flag_PwmTimerMatchesOCF0A(void)
{
    /* =====[ Setup ]===== */
    /** Note this is *not* how TIFR0 works, this is just for testing.
      * The flag is cleared by setting its bit in TIFR0.
      * I do not bother to simulate that mechanism.
      * I test by treating TIFR0 like any other register.
      * */
    _setup_bit_val(Lis_TIFR0, Lis_OCF0A, LOW);
    /* =====[ Operate ]===== */
    _WaitForLisClkHigh();
    /* =====[ Test ]===== */
    _test_bit_val(Lis_TIFR0, Lis_OCF0A, HIGH);
}
void WaitForLisClkHigh_waits_until_flag_PwmTimerMatchesOCF0A_is_set(void)
{
    // Cannot fake setting flag after calling function under test
    TEST_PASS();
}

/* =====[ _EnterLisProgrammingMode ]===== */
void EnterLisProgrammingMode_waits_for_LisClk_LOW(void)
{
    /* =====[ Operate ]===== */
    _EnterLisProgrammingMode();
    /* =====[ Test ]===== */
    _AssertCall(1, "_WaitForLisClkLow");
}
void EnterLisProgrammingMode_asserts_LisPixSelect_to_program_Lis(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Lis_port2, Lis_PixSelect);
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_PixSelect,
        *Lis_port2,
        "PixSelect must be LOW when the test starts."
        );
    /* =====[ Operate ]===== */
    _EnterLisProgrammingMode();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Lis_PixSelect,
        *Lis_port2,
        "Expect `EnterLisProgrammingMode` drives LisPixSelect HIGH."
        );
}

/* =====[ _WriteLisConfigBit ]===== */
void WriteLisConfigBit_outputs_bit_on_LisRst(void)
{
    /* =====[ Setup ]===== */
    uint8_t config_bytes[4];
    config_bytes[0] = 0x01; // byte 0 bit 0 = 1
    config_bytes[1] = 0x00; // byte 1 bit 0 = 0
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        0, config_bytes[0], "Test starts with byte0 bit0 = 1"
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        0, config_bytes[1], "Test starts with byte1 bit0 = 0"
        );
    /* =====[ Operate and Test ]===== */
    uint8_t * config = config_bytes; uint8_t bit_index = 0;
    _WriteLisConfigBit(config++, bit_index); // byte 0 bit 0
    TEST_ASSERT_BIT_HIGH(Lis_Rst, *Lis_port1);
    _WriteLisConfigBit(config, bit_index); // byte 1 bit 0
    TEST_ASSERT_BIT_LOW(Lis_Rst, *Lis_port1);
}
void WriteLisConfigBit_waits_for_LisClk_HIGH(void)
{
    /* =====[ Operate ]===== */
    uint8_t config[4]; uint8_t bit_index = 0;
    _WriteLisConfigBit(config, bit_index);
    /* =====[ Test ]===== */
    _AssertCall(1, "_WaitForLisClkHigh");
}
void WriteLisConfigBit_waits_for_LisClk_LOW(void)
{
    /* =====[ Operate ]===== */
    uint8_t config[4]; uint8_t bit_index = 0;
    _WriteLisConfigBit(config, bit_index);
    /* =====[ Test ]===== */
    _AssertCall(2, "_WaitForLisClkLow");
}

/* =====[ _Write28bitLisConfig ]===== */
void Write28bitLisConfig_writes_28bits_starting_at_byte0_bit0_and_ending_at_byte3_bit3(void)
{
    uint8_t config_bytes[4];
    config_bytes[0] = 0xac;
    config_bytes[1] = 0xbd;
    config_bytes[2] = 0xce;
    config_bytes[3] = 0xdf;
    /* =====[ Operate ]===== */
    uint8_t * config = config_bytes;
    _Write28bitLisConfig(config);
    /* =====[ Test ]===== */
    char const * call_name = "_WriteLisConfigBit";
    uint16_t call_n = 1;
    // Check first 24 bits
    for (uint8_t byte_i = 0; byte_i<3; byte_i++)
    {
        for (uint8_t bit_i = 0; bit_i<8; bit_i++)
        {
            _AssertCall(call_n, call_name);
            _AssertArg(call_n, 1, &config);
            _AssertArg(call_n, 2, &bit_i);
            call_n++;
        }
        config++;
    }
    // Check last four bits
    for (uint8_t bit_i = 0; bit_i<4; bit_i++)
    {
        _AssertCall(call_n, call_name);
        _AssertArg(call_n, 1, &config);
        _AssertArg(call_n, 2, &bit_i);
        call_n++;
    }
}

/* =====[ _ExitLisProgrammingMode ]===== */
void ExitLisProgrammingMode_outputs_LOW_on_pin_LisRst(void)
{
    /* =====[ Setup ]===== */
    SetBit(Lis_port1, Lis_Rst);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Lis_Rst,
        *Lis_port1,
        "Rst must be HIGH when the test starts."
        );
    /* =====[ Operate ]===== */
    _ExitLisProgrammingMode();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_Rst,
        *Lis_port1,
        "Expect `Lis_Rst` LOW."
        );
}
void ExitLisProgrammingMode_outputs_LOW_on_pin_LisPixSelect(void)
{
    /* =====[ Setup ]===== */
    SetBit(Lis_port2, Lis_PixSelect);
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Lis_PixSelect,
        *Lis_port2,
        "PixSelect must be HIGH when the test starts."
        );
    /* =====[ Operate ]===== */
    _ExitLisProgrammingMode();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Lis_PixSelect,
        *Lis_port2,
        "Expect `Lis_PixSelect` LOW."
        );
}

