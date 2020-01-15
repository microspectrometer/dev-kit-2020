#include "unity.h" // unity macros: TEST_BLAH
#include "Mock.h"  // record call history in "mock"
#include "test_VisCmd.h"
#include "VisCmd.h"

void ReplyCommandInvalid_transmits_one_byte_over_SPI(void)
{
    /* =====[ Operate ]===== */
    ReplyCommandInvalid();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE(
        AssertCall(mock, call_n, "SpiSlaveTxByte")
        );
}
void ReplyCommandInvalid_sends_byte_INVALID_CMD(void)
{
    /** ReplyCommandInvalid behavior:\n 
      * - transmits one byte over SPI\n 
      * - sends byte INVALID CMD\n 
      * */
    /* =====[ Operate ]===== */
    ReplyCommandInvalid();
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SpiSlaveTxByte"),
        "Expect first call to be SpiSlaveTxByte."
        );
    uint8_t arg_n = 1; status_code byte = INVALID_CMD;
    TEST_ASSERT_TRUE(
        AssertArg(mock, call_n, arg_n, &byte)
        ); 
}
