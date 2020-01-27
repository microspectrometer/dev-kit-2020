#include "unity.h"
#include "Mock.h"
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
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "_ConfigAs28bits"),
        "Expect call to `_ConfigAs28bits`."
        );
}
void LisWriteConfig_enters_LIS_programming_mode(void){}
void LisWriteConfig_writes_28bits_to_LIS_setup_register(void){}
void LisWriteConfig_exits_LIS_programming_mode(void){}

