#include <stdio.h>          // printf
#include <stdbool.h>        // bool, true, false
#include <unity.h>          // used by all test client code
#include <Mock.h>           // used by all test client code
#include <RecordedCall.h>   // used by *DOF* `mock_lib.c`
#include <RecordedArg.h>    // used by *DOF* `mock_lib.c`
#include "test_ReadWriteBits.h"

void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock;

bool Yep=true, Nope=false;

void DevelopingReadWriteBits(bool run_test) { if (run_test) {
    setUp       = SetUp_libReadWriteBits;
    tearDown    = TearDown_libReadWriteBits;
    RUN_TEST(SetBit_sets_a_bit);
    RUN_TEST(SetBit_does_not_clear_any_bits);
    RUN_TEST(ClearBit_clears_a_bit);
    RUN_TEST(ClearBit_does_not_set_any_bits);
    RUN_TEST(ToggleBit_toggles_a_bit_low_to_high);
    RUN_TEST(ToggleBit_toggles_a_bit_high_to_low);
    RUN_TEST(BitIsSet_is_true_if_bit_is_set);
    RUN_TEST(BitIsSet_is_false_if_bit_is_clear);
    RUN_TEST(BitIsClear_is_true_if_bit_is_clear);
    RUN_TEST(BitIsClear_is_false_if_bit_is_set);
}}
int main()
{
    UNITY_BEGIN();
    DevelopingReadWriteBits(Nope);
    return UNITY_END();
}
