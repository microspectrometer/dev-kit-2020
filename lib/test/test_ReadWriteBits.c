#include "test_ReadWriteBits.h"
#include <ReadWriteBits.h>
#include <unity.h>

uint8_t reg; uint8_t *reg_address=&reg; uint8_t bit;

void SetUp_libReadWriteBits(void){}
void TearDown_libReadWriteBits(void){}

void SetBit_sets_a_bit(void) {
    reg=0x00; bit=4; uint8_t reg_bit4_is_set=0x10;
    SetBit(reg_address, bit);
    TEST_ASSERT_EQUAL_UINT8(reg_bit4_is_set, reg);
}
void SetBit_does_not_clear_any_bits(void) {
    reg=0xFF; bit=4; uint8_t reg_before_SetBit=reg;
    SetBit(reg_address, bit);
    TEST_ASSERT_EQUAL_UINT8(reg_before_SetBit, reg);
}
void ClearBit_clears_a_bit(void) {
    reg=0xFF; bit=4; uint8_t reg_bit4_is_clear=0xEF;
    ClearBit(reg_address, bit);
    TEST_ASSERT_EQUAL_UINT8(reg_bit4_is_clear, reg);
}
void ClearBit_does_not_set_any_bits(void) {
    reg=0x00; bit=4; uint8_t reg_before_ClearBit=reg;
    ClearBit(reg_address, bit);
    TEST_ASSERT_EQUAL_UINT8(reg_before_ClearBit, reg);
}
void ToggleBit_toggles_a_bit_low_to_high(void) {
    reg=0x00; bit=4; uint8_t reg_bit4_toggled=0x10;
    ToggleBit(reg_address, bit);
    TEST_ASSERT_EQUAL_UINT8(reg_bit4_toggled, reg);
}
void ToggleBit_toggles_a_bit_high_to_low(void) {
    reg=0xFF; bit=4; uint8_t reg_bit4_toggled=0xEF;
    ToggleBit(reg_address, bit);
    TEST_ASSERT_EQUAL_UINT8(reg_bit4_toggled, reg);
}
void BitIsSet_is_true_if_bit_is_set(void) {
    reg=0x10; bit=4;
    TEST_ASSERT_TRUE(BitIsSet(reg_address,bit));
}
void BitIsSet_is_false_if_bit_is_clear(void) {
    reg=0x10; bit=3;
    TEST_ASSERT_FALSE(BitIsSet(reg_address,bit));
}
void BitIsClear_is_true_if_bit_is_clear(void) {
    reg=0xEF; bit=4;
    TEST_ASSERT_TRUE(BitIsClear(reg_address,bit));
}
void BitIsClear_is_false_if_bit_is_set(void) {
    reg=0xEF; bit=3;
    TEST_ASSERT_FALSE(BitIsClear(reg_address,bit));
}
