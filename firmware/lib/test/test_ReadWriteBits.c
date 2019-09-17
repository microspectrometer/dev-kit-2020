#include "test_ReadWriteBits.h"
#include <ReadWriteBits.h>
#include <unity.h>

void SetUp_libReadWriteBits(void){}
void TearDown_libReadWriteBits(void){}

uint8_t reg; uint8_t *reg_address=&reg; uint8_t bit;

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

/* =====[ Old: tests of Macro versions from before I knew how to inline. ]===== */
/* void MacroBitIsSet_is_true_if_bit_is_set(void) */
/* { */
/*     /1* =====[ Setup ]===== *1/ */
/*     reg=0x10; bit=4; */
/*     /1* =====[ Operate and Test ]===== *1/ */
/*     TEST_ASSERT_TRUE(MacroBitIsSet(&reg, bit)); */
/* } */
/* void MacroBitIsSet_is_false_if_bit_is_clear(void) */
/* { */
/*     /1* =====[ Setup ]===== *1/ */
/*     reg=0x10; bit=3; */
/*     /1* =====[ Operate and Test ]===== *1/ */
/*     TEST_ASSERT_FALSE(MacroBitIsSet(&reg, bit)); */
/* } */
/* void MacroBitIsClear_is_true_if_bit_is_clear(void) */
/* { */
/*     /1* =====[ Setup ]===== *1/ */
/*     reg=0xEF; bit=4; */
/*     /1* =====[ Operate and Test ]===== *1/ */
/*     TEST_ASSERT_TRUE(MacroBitIsClear(&reg, bit)); */
/* } */
/* void MacroBitIsClear_is_false_if_bit_is_set(void) */
/* { */
/*     /1* =====[ Setup ]===== *1/ */
/*     reg=0xEF; bit=3; */
/*     /1* =====[ Operate and Test ]===== *1/ */
/*     TEST_ASSERT_FALSE(MacroBitIsClear(&reg, bit)); */
/* } */
/* void MacroSetBit_sets_a_bit(void) */
/* { */
/*     /1* =====[ Setup ]===== *1/ */
/*     uint8_t actual_reg=0x00; uint8_t bit_number=4; */
/*     /1* =====[ Operate ]===== *1/ */
/*     MacroSetBit(&actual_reg, bit_number); */
/*     /1* =====[ Test ]===== *1/ */
/*     uint8_t expect_reg=0x10; */
/*     TEST_ASSERT_EQUAL_UINT8(expect_reg, actual_reg); */
/* } */
/* void MacroSetBit_does_not_clear_any_bits(void) */
/* { */
/*     /1* =====[ Setup ]===== *1/ */
/*     uint8_t actual_reg = 0xFF; uint8_t bit_number = 4; */
/*     /1* =====[ Operate ]===== *1/ */
/*     MacroSetBit(&actual_reg, bit_number); */
/*     /1* =====[ Test ]===== *1/ */
/*     uint8_t expect_reg = 0xFF; */
/*     TEST_ASSERT_EQUAL_UINT8(expect_reg, actual_reg); */
/* } */
/* void MacroClearBit_clears_a_bit(void) */
/* { */
/*     /1* =====[ Setup ]===== *1/ */
/*     uint8_t actual_reg = 0xFF; uint8_t bit_number = 4; */
/*     /1* =====[ Operate ]===== *1/ */
/*     MacroClearBit(&actual_reg, bit_number); */
/*     /1* =====[ Test ]===== *1/ */
/*     uint8_t expect_reg = 0xEF; */
/*     TEST_ASSERT_EQUAL_UINT8(expect_reg, actual_reg); */
/* } */
/* void MacroClearBit_does_not_set_any_bits(void) */
/* { */
/*     /1* =====[ Setup ]===== *1/ */
/*     uint8_t actual_reg = 0x00; uint8_t bit_number = 4; */
/*     /1* =====[ Operate ]===== *1/ */
/*     MacroClearBit(&actual_reg, bit_number); */
/*     /1* =====[ Test ]===== *1/ */
/*     uint8_t expect_reg = 0x00; */
/*     TEST_ASSERT_EQUAL_UINT8(expect_reg, actual_reg); */
/* } */