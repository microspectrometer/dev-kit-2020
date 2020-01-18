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
    //! Change in global config state implies config bytes received
    /* =====[ Setup ]===== */
    // Fake some initial config values.
    binning = BINNING_ON;
    gain = GAIN_1X;
    active_rows = ALL_ROWS_ACTIVE;
    // Fake sending new config values by queuing them.
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, BINNING_OFF);
    QueuePush(SpiFifo, GAIN_5X);
    QueuePush(SpiFifo, ONLY_ROW_1_ACTIVE);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(BINNING_OFF, binning, "binning");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(GAIN_5X, gain, "gain");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(ONLY_ROW_1_ACTIVE, active_rows, "active_rows");
}
void SetSensorConfig_replies_ERROR_if_binning_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake sending an invalid value for binning
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, BINNING_OFF+100);
    QueuePush(SpiFifo, GAIN_5X);
    QueuePush(SpiFifo, ONLY_ROW_1_ACTIVE);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE(
        AssertCall(mock, call_n, "SpiSlaveTxByte")
        );
    uint8_t arg_n = 1; status_code byte = ERROR;
    TEST_ASSERT_TRUE(
        AssertArg(mock, call_n, arg_n, (uint8_t *)&byte)
        );
}
void SetSensorConfig_replies_ERROR_if_gain_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake sending an invalid value for gain
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, BINNING_OFF);
    QueuePush(SpiFifo, GAIN_5X+100);
    QueuePush(SpiFifo, ONLY_ROW_1_ACTIVE);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE(
        AssertCall(mock, call_n, "SpiSlaveTxByte")
        );
    uint8_t arg_n = 1; status_code byte = ERROR;
    TEST_ASSERT_TRUE(
        AssertArg(mock, call_n, arg_n, (uint8_t *)&byte)
        );
}
void SetSensorConfig_replies_ERROR_if_active_rows_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake sending an invalid value for active_rows
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, BINNING_OFF);
    QueuePush(SpiFifo, GAIN_5X);
    QueuePush(SpiFifo, 0xFF);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE(
        AssertCall(mock, call_n, "SpiSlaveTxByte")
        );
    uint8_t arg_n = 1; status_code byte = ERROR;
    TEST_ASSERT_TRUE(
        AssertArg(mock, call_n, arg_n, (uint8_t *)&byte)
        );
}
void SetSensorConfig_replies_OK_if_all_config_values_are_valid(void)
{
    /* =====[ Setup ]===== */
    // Fake sending valid config values
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, BINNING_OFF);
    QueuePush(SpiFifo, GAIN_5X);
    QueuePush(SpiFifo, ONLY_ROW_1_ACTIVE);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, call_n, "SpiSlaveTxByte"),
        "Expect call `SpiSlaveTxByte` - send status code."
        );
    uint8_t arg_n = 1; status_code byte = OK;
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, (uint8_t *)&byte),
        "Expect SpiSlaveTxByte(OK) (0x00)."
        );
}
void SetSensorConfig_writes_config_to_LIS_770i_programmable_setup_register(void)
{
    /* =====[ Setup ]===== */
    // Fake sending valid config values
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, BINNING_OFF);
    QueuePush(SpiFifo, GAIN_5X);
    QueuePush(SpiFifo, ONLY_ROW_1_ACTIVE);
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    TEST_FAIL_MESSAGE("Implement test.");
}
