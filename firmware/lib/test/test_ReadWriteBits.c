#include "unity.h"
#include "test_ReadWriteBits.h"
#include "ReadWriteBits.h"
void SetBit_sets_bit_in_register(void)
{
    /* =====[ Setup ]===== */
    uint8_t port = 0x00;
    uint8_t bit = 0;
    TEST_ASSERT_BIT_LOW_MESSAGE(
        bit,
        port,
        "Cannot run test: must start with low bit in port!"
        );
    /* =====[ Operate ]===== */
    SetBit(&port, bit);
    TEST_ASSERT_BIT_HIGH(bit, port);
}
void ClearBit_clears_bit_in_register(void)
{
    /* =====[ Setup ]===== */
    uint8_t port = 0xFF;
    uint8_t bit = 0;
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        bit,
        port,
        "Cannot run test: must start with high bit in port!"
        );
    /* =====[ Operate ]===== */
    ClearBit(&port, bit);
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(bit, port);
}
void BitIsSet_returns_true_if_bit_is_set(void)
{
    /* =====[ Setup ]===== */
    uint8_t port = 0x01;
    uint8_t bit = 0;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(BitIsSet(&port, bit));
}
void BitIsSet_returns_false_if_bit_is_clear(void)
{
    /* =====[ Setup ]===== */
    uint8_t port = 0x00;
    uint8_t bit = 0;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(BitIsSet(&port, bit));
}
void BitIsClear_returns_true_if_bit_is_clear(void)
{
    /* =====[ Setup ]===== */
    uint8_t port = 0xFE;
    uint8_t bit = 0;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(BitIsClear(&port, bit));
}
void BitIsClear_returns_false_if_bit_is_set(void)
{
    /* =====[ Setup ]===== */
    uint8_t port = 0xFF;
    uint8_t bit = 0;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(BitIsClear(&port, bit));
}

