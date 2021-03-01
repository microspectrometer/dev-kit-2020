// $ gcc code-not-used/test_VisCmd-ReplyCommandInvalid.c -S -o code-not-used/test_VisCmd-ReplyCommandInvalid.asm -I../lib/src -Isrc -Itest -I../lib/test -I../mock-c/code -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -g
#include "test-boiler-plate.h"
#include "VisCmd-ReplyCommandInvalid.h"
#include "test_VisCmd-ReplyCommandInvalid.h"

void (*setUp)(void);
void (*tearDown)(void);
Mock_s *mock; // Tests record mocked calls and call args here
void SetUp_Mock(void){
    mock = Mock_new();
}
void TearDown_Mock(void){
    Mock_destroy(mock);
    mock = NULL;
}

void ReplyCommandInvalid_transmits_one_byte_over_SPI(void)
{
    /* =====[ Operate ]===== */
    ReplyCommandInvalid();
    /* =====[ Test ]===== */
    _AssertCall(1, "SpiSlaveTxByte");
    _test_call_count_is(1);
}
void ReplyCommandInvalid_sends_byte_INVALID_CMD(void)
{
    /* =====[ Operate ]===== */
    ReplyCommandInvalid();
    /* =====[ Test ]===== */
    // Expect call: SpiSlaveTxByte(INVALID_CMD)
    uint16_t call_n = 1;
    _SilentAssertCall(call_n, "SpiSlaveTxByte");
    _AssertArgByteVal(call_n, 1, INVALID_CMD);
}

int main(void)
{
    UNITY_BEGIN();
    puts("# test VisCmd");
    puts("## VisCmd.ReplyCommandInvalid");
    setUp = SetUp_Mock;
    tearDown = TearDown_Mock;
    RUN_TEST(ReplyCommandInvalid_transmits_one_byte_over_SPI);
    RUN_TEST(ReplyCommandInvalid_sends_byte_INVALID_CMD);
    return UNITY_END();
}

