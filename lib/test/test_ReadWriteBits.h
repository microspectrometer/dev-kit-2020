#ifndef _TEST_READWRITEBITS_H
#define _TEST_READWRITEBITS_H

void SetUp_libReadWriteBits(void); void TearDown_libReadWriteBits(void);
void SetBit_sets_a_bit(void);
void SetBit_does_not_clear_any_bits(void);
void ClearBit_clears_a_bit(void);
void ClearBit_does_not_set_any_bits(void);
void ToggleBit_toggles_a_bit_low_to_high(void);
void ToggleBit_toggles_a_bit_high_to_low(void);
void BitIsSet_is_true_if_bit_is_set(void);
void BitIsSet_is_false_if_bit_is_clear(void);
void BitIsClear_is_true_if_bit_is_clear(void);
void BitIsClear_is_false_if_bit_is_set(void);

#endif // _TEST_READWRITEBITS_H
