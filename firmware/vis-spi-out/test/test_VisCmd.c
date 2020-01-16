#include "unity.h" // unity macros: TEST_BLAH
#include "Mock.h"  // record call history in "mock"
#include "test_VisCmd.h"
#include "VisCmd.h"

/* ---Global Queue--- */
#include "Queue.h"          // VisCmd accesses the Spi Rx Buffer
extern volatile Queue_s * SpiFifo;
#define max_length_of_queue 5 // bytes

/* =====[ ReplyCommandInvalid ]===== */
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
        AssertArg(mock, call_n, arg_n, (uint8_t *)&byte)
        );
}

/* =====[ SetSensorConfig ]===== */
void SetSensorConfig_receives_three_bytes_of_config_from_Bridge(void)
{
    /* =====[ Setup ]===== */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Fake placing three bytes in the queue.
    QueuePush(SpiFifo, BINNING_OFF);
    QueuePush(SpiFifo, GAIN_5X);
    QueuePush(SpiFifo, ALL_ROWS_ACTIVE);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(BINNING_OFF, binning);
    TEST_ASSERT_EQUAL_HEX8(GAIN_5X, gain);
    TEST_ASSERT_EQUAL_HEX8(ALL_ROWS_ACTIVE, active_rows);
}
void SetSensorConfig_replies_msg_status_error_if_binning_is_invalid(void)
{
    TEST_FAIL_MESSAGE("Implement test.");
}
