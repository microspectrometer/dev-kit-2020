// test_runner.c
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

// test_SensorVis.c
// ---This is old. Need four bytes, not a uint32_t.---
// tdd RepresentConfigAs28bits
void RepresentConfigAs28bits_returns_uint32_with_bit0_set_if_binning_is_on(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_on;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_BIT_HIGH(0,RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_bit0_clear_if_binning_is_off(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_BIT_LOW(0,RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_bits1to2_clear_if_gain_is_1x(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_BIT_LOW(1,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_LOW(2,RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_bit1_clear_bit2_set_if_gain_is_2p5x(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain25x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate and Test ]===== */
    /* printf("Config: %010x\n", RepresentConfigAs28bits(binning, gain, active_rows)); */
    TEST_ASSERT_BIT_LOW(1,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(2,RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_bit1_set_bit2_clear_if_gain_is_4x(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain4x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate and Test ]===== */
    /* printf("Config: %010x\n", RepresentConfigAs28bits(binning, gain, active_rows)); */
    TEST_ASSERT_BIT_HIGH(1,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_LOW(2,RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_bits1to2_set_if_gain_is_5x(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain5x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_BIT_HIGH(1,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(2,RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_bits3to27_set_if_all_rows_are_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t active_rows = all_rows_active;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_HEX32(0x0FFFFFF8, RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(3,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(4,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(5,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(6,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(7,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(8,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(9,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(10,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(11,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(12,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(13,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(14,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(15,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(16,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(17,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(18,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(19,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(20,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(21,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(22,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(23,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(24,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(25,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(26,RepresentConfigAs28bits(binning, gain, active_rows));
    TEST_ASSERT_BIT_HIGH(27,RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_b3b8b13b18b23_set_if_row1_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row1 = 0;
    uint8_t const row2 = 1; (void)row2;
    uint8_t const row3 = 2; (void)row3;
    uint8_t const row4 = 3; (void)row4;
    uint8_t const row5 = 4; (void)row5;
    uint8_t active_rows = (1<<row1);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_HEX32(0x00842108, RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_b4b9b14b19b24_set_if_row2_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row1 = 0; (void)row1;
    uint8_t const row2 = 1;
    uint8_t const row3 = 2; (void)row3;
    uint8_t const row4 = 3; (void)row4;
    uint8_t const row5 = 4; (void)row5;
    uint8_t active_rows = (1<<row2);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_HEX32(0x01084210, RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_b5b10b15b20b25_set_if_row3_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row1 = 0; (void)row1;
    uint8_t const row2 = 1; (void)row2;
    uint8_t const row3 = 2;
    uint8_t const row4 = 3; (void)row4;
    uint8_t const row5 = 4; (void)row5;
    uint8_t active_rows = (1<<row3);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_HEX32(0x02108420, RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_b6b11b16b21b26_set_if_row4_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row1 = 0; (void)row1;
    uint8_t const row2 = 1; (void)row2;
    uint8_t const row3 = 2; (void)row3;
    uint8_t const row4 = 3;
    uint8_t const row5 = 4; (void)row5;
    uint8_t active_rows = (1<<row4);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_HEX32(0x04210840, RepresentConfigAs28bits(binning, gain, active_rows));
}
void RepresentConfigAs28bits_returns_uint32_with_b7b12b17b22b27_set_if_row5_is_active(void)
{
    /* =====[ Setup ]===== */
    uint8_t binning = binning_off;
    uint8_t gain = gain1x;
    uint8_t const row1 = 0; (void)row1;
    uint8_t const row2 = 1; (void)row2;
    uint8_t const row3 = 2; (void)row3;
    uint8_t const row4 = 3; (void)row4;
    uint8_t const row5 = 4;
    uint8_t active_rows = (1<<row5);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_EQUAL_HEX32(0x08421080, RepresentConfigAs28bits(binning, gain, active_rows));
}

// test_SensorVis.h
// ---This is old. Need four bytes, not a uint32_t.---
void RepresentConfigAs28bits_returns_uint32_with_bit0_set_if_binning_is_on(void);
void RepresentConfigAs28bits_returns_uint32_with_bit0_clear_if_binning_is_off(void);
void RepresentConfigAs28bits_returns_uint32_with_bits1to2_clear_if_gain_is_1x(void);
void RepresentConfigAs28bits_returns_uint32_with_bits1to2_set_if_gain_is_5x(void);
void RepresentConfigAs28bits_returns_uint32_with_bit1_clear_bit2_set_if_gain_is_2p5x(void);
void RepresentConfigAs28bits_returns_uint32_with_bit1_set_bit2_clear_if_gain_is_4x(void);
void RepresentConfigAs28bits_returns_uint32_with_bits3to27_set_if_all_rows_are_active(void);
void RepresentConfigAs28bits_returns_uint32_with_b3b8b13b18b23_set_if_row1_is_active(void);
void RepresentConfigAs28bits_returns_uint32_with_b4b9b14b19b24_set_if_row2_is_active(void);
void RepresentConfigAs28bits_returns_uint32_with_b5b10b15b20b25_set_if_row3_is_active(void);
void RepresentConfigAs28bits_returns_uint32_with_b6b11b16b21b26_set_if_row4_is_active(void);
void RepresentConfigAs28bits_returns_uint32_with_b7b12b17b22b27_set_if_row5_is_active(void);
//
