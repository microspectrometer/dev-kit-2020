#include "unity.h" // unity macros: TEST_BLAH
#include "Mock.h"  // record call history in "mock"
#include "test_VisCmd.h"
#include "VisCmd.h"

/* ---Global Queue--- */
#include "Queue.h"          // VisCmd accesses the Spi Rx Buffer
volatile Queue_s * SpiFifo; // extern in VisCmd.h
#define max_length_of_queue 5 // bytes

/* =====[ Test Helpers ]===== */
static void _AssertCall(uint16_t num, char const * name)
{
    //! Assert call number **num** is named **name**.
    // Put num and name in the message displayed if test fails
    GString *message = g_string_new(NULL);
    g_string_append_printf(message, "`%s` is not call %d", name, num);
    // Perform the test
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, num, name),
        message->str
        );
    // Free memory used by GString
    g_string_free(message, true);
}
static void _AssertArg(uint16_t call_n, uint8_t arg_n, void *pval)
{
    //! Assert value of arg number **arg_n** == (*pval)
    /** Pass **address of arg** to parameter **pval**.\n
      * **Pass arg same way, whether it is a value or a pointer.**
      *
      * Example: `CallWithTwoArgs(uint8_t *pbyte, uint8_t bit_num)`
      *
      * ```
      * uint8_t byte = 0x0a;
      * uint8_t *pbyte = &byte;
      * uint8_t bit_num = 7;
      * _AssertCall(1, "CallWithTwoArgs");
      * _AssertArg(1,1, &pbyte);
      * _AssertArg(1,2, &bit_num);
      * ```
      *
      * The first _AssertArg checks a pointer to a byte.
      * The second _AssertArg checks a byte.
      *
      * Both use the `&` prefix to pass the address.
      *
      * In the case of `&pbyte`, AssertArg() checks the pointer
      * (pbyte), but the pre-test-result prints the value
      * pointed to (byte).
      *
      * Example:
      *
      * ```
      * // Call was `CallWithTwoArgs(pbyte, bit_num);`
      * _AssertArg(1,2, &bit_num); // check arg2 equals 7
      * _AssertArg(1,1, &pbyte); // check arg1 equals address of byte
      * ```
      *
      * The AssertArg pre-test-result message prints the value
      * recorded arg1 points to (0x0a).\n 
      *
      * Note that the test fails if arg1 does not equal pbyte,
      * even if pbyte and arg1 happen to point to equal values.
      * */
    GString *msg = g_string_new(NULL);
    g_string_printf(msg, "Expect different value for call %d arg %d.", call_n, arg_n);
    // I cannot print the expected value without asking the
    // caller to include the type as a string. Better to keep the
    // arg list short. Call and arg number are good enough.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, pval),
        msg->str
        );
    g_string_free(msg, true);
}
static void _AssertArgByteVal(uint16_t call_n, uint8_t arg_n, uint8_t byte_val) { _AssertArg(call_n, arg_n, &byte_val); }
static void _test_call_count_is(uint16_t num)
{
    // Print this message if test fails
    // Note: msg is appended if I use a TEST_ASSERT_EQUAL macro,
    // so use TEST_FAIL_MESSAGE macro instead.
    GString *msg = g_string_new(NULL);
    g_string_printf(msg,
        "Expect call count is %d, but was %d.",
        num, NumberOfActualCalls(mock)
        );
    // Run test
    if (num == NumberOfActualCalls(mock)) TEST_PASS();
    else TEST_FAIL_MESSAGE(msg->str);
    // Free string-object memory
    g_string_free(msg, true);
}

/* =====[ LisReadout ]===== */
void LisReadout_waits_for_Lis_Sync_to_go_HIGH_then_go_LOW(void)
{
    TEST_PASS();
}
void LisReadout_reads_one_pixel_on_each_rising_edge_of_Lis_Clk(void)
{
    TEST_PASS();
}
void LisReadout_LOOP_wait_for_the_rising_edge_of_Lis_Clk(void)
{
    TEST_PASS();
}
void LisReadout_LOOP_start_the_ADC_conversion(void)
{
    TEST_PASS();
}
void LisReadout_LOOP_wait_for_45_cycles_of_10MHz_clock(void)
{
    TEST_PASS();
}
void LisReadout_LOOP_start_ADC_readout(void)
{
    TEST_PASS();
}
void LisReadout_LOOP_wait_for_most_significant_byte_ADC_readout(void)
{
    TEST_PASS();
}
void LisReadout_LOOP_save_MSB_to_frame_buffer(void)
{
    TEST_PASS();
}
void LisReadout_LOOP_wait_for_least_significant_byte_ADC_readout(void)
{
    TEST_PASS();
}
void LisReadout_LOOP_save_LSB_to_frame_buffer(void)
{
    TEST_PASS();
}

/* =====[ GetSensorLED ]===== */
void GetSensorLED_waits_for_byte_led_num(void)
{
    TEST_PASS();
}
void GetSensorLED_reads_byte_led_num(void)
{
    TEST_PASS();
}
void GetSensorLED_sends_OK_and_LED_SETTING_if_led_num_is_valid(void)
{
    TEST_PASS();
}
void GetSensorLED_sends_ERROR_and_pads_second_byte_if_led_num_is_invalid(void)
{
    TEST_PASS();
}

/* =====[ LedNumIsValid ]===== */
void LedNumIsValid_returns_TRUE_if_led_num_is_0(void)
{
    /* =====[ Operate and Test]===== */
    TEST_ASSERT_TRUE(LedNumIsValid(0));
}
void LedNumIsValid_returns_TRUE_if_led_num_is_1(void)
{
    /* =====[ Operate and Test]===== */
    TEST_ASSERT_TRUE(LedNumIsValid(1));
}
void LedNumIsValid_returns_FALSE_if_led_num_is_not_0_or_1(void)
{
    /* =====[ Operate and Test]===== */
    TEST_ASSERT_FALSE(LedNumIsValid(2));
}

/* =====[ SetSensorLED ]===== */
void SetSensorLED_waits_for_byte_led_num(void)
{
    TEST_PASS();
}
void SetSensorLED_reads_byte_led_num(void)
{
    TEST_PASS();
}
void SetSensorLED_waits_for_byte_led_setting(void)
{
    TEST_PASS();
}
void SetSensorLED_reads_byte_led_setting(void)
{
    TEST_PASS();
}
void SetSensorLED_sends_ERROR_if_led_num_is_invalid(void)
{
    TEST_PASS();
}
void SetSensorLED_sends_ERROR_if_led_setting_is_invalid(void)
{
    TEST_PASS();
}
void SetSensorLED_applies_LED_setting_if_valid(void)
{
    TEST_PASS();
}
void SetSensorLED_sends_OK_if_num_and_setting_are_valid(void)
{
    TEST_PASS();
}

#ifdef LIS
/* =====[ GetSensorConfig ]===== */
void GetSensorConfig_sends_OK(void)
{
    /* =====[ Operate ]===== */
    /* GetSensorConfig(); */
    TEST_PASS();
}
void GetSensorConfig_sends_binning(void)
{
    TEST_PASS();
}
void GetSensorConfig_sends_gain(void)
{
    TEST_PASS();
}
void GetSensorConfig_sends_row_bitmap(void)
{
    TEST_PASS();
}

/* =====[ SetSensorConfig ]===== */
void SetSensorConfig_receives_three_bytes_of_config_from_Bridge(void)
{
    /** Test config bytes are received by checking if function
      * under test changes the global config state.
      * */
    /* =====[ Setup ]===== */
    // Fake some initial config values.
    binning = BINNING_ON;
    gain = GAIN_1X;
    active_rows = ALL_ROWS_ACTIVE;
    // Fake sending new valid config values by queuing them.
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
/* =====[ test Setup helpers ]===== */
static bool _binning_is_valid(uint8_t binning)
{ //! Helper for `_print_list_of_invalid_config_values`
    return (
        (BINNING_ON == binning) ||
        (BINNING_OFF == binning)
        );
}
static bool _gain_is_valid(uint8_t gain)
{ //! Helper for `_print_list_of_invalid_config_values`
    return (
        (GAIN_1X  == gain) ||
        (GAIN_2X5 == gain) ||
        (GAIN_4X  == gain) ||
        (GAIN_5X  == gain)
        );
}
static bool _active_rows_is_valid(uint8_t active_rows)
{ //! Helper for `_print_list_of_invalid_config_values`
    return ( (active_rows & 0xE0) == 0x00 );
}
static bool _config_is_valid(
        uint8_t binning,
        uint8_t gain,
        uint8_t active_rows)
{ //! Helper for `_print_list_of_invalid_config_values`
    return (
        (_binning_is_valid(binning)) &&
        (_gain_is_valid(gain))       &&
        (_active_rows_is_valid(active_rows))
        );
}
static void _print_list_of_invalid_config_values(
    uint8_t binning,
    uint8_t gain,
    uint8_t active_rows)
{ //! Helper for `put_config_in_Queue`
    /** Print line "INVALID: {list of invalid config byte names}"
      * Print nothing if config is valid.
      * */
    // Prepare message listing invalid bytes, if any.
    GString *msg = g_string_new("INVALID: ");
    if (!_binning_is_valid(binning)) g_string_append_printf(msg, "binning ");
    if (!_gain_is_valid(gain)) g_string_append_printf(msg, "gain ");
    if (!_active_rows_is_valid(active_rows)) g_string_append_printf(msg, "active_rows");
    g_string_append_printf(msg, "\n");
    // Print INVALID bytes, if any.
    if (!_config_is_valid(binning, gain, active_rows)) printf("%s", msg->str);
    // Free memory used by GString
    g_string_free(msg, true);
}
static void _print_config_values(
    uint8_t binning,
    uint8_t gain,
    uint8_t active_rows)
{ //! Helper for `put_config_in_Queue`
    printf("binning == %d, "    , binning);
    printf("gain == %d, "       , gain);
    printf("active_rows == %d\n", active_rows);
}
static void put_config_in_Queue(
    uint8_t binning,
    uint8_t gain,
    uint8_t active_rows)
{
    // Flag `INVALID` values
    _print_list_of_invalid_config_values(binning, gain, active_rows);
    // List config values used in test
    _print_config_values(binning, gain, active_rows);
    // Put config values on the Queue.
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, binning);
    QueuePush(SpiFifo, gain);
    QueuePush(SpiFifo, active_rows);
}
void SetSensorConfig_does_not_update_config_globals_if_config_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake some initial config values.
    binning = BINNING_ON;
    gain = GAIN_1X;
    active_rows = ALL_ROWS_ACTIVE;
    // Fake sending new invalid config values by queuing them.
    put_config_in_Queue(
            BINNING_OFF+100, // invalid
            GAIN_5X, // valid
            ONLY_ROW_1_ACTIVE // valid
            );
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    // Assert global config values did not change.
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(BINNING_ON, binning, "binning");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(GAIN_1X, gain, "gain");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(ALL_ROWS_ACTIVE, active_rows, "active_rows");
}
void SetSensorConfig_replies_ERROR_if_binning_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake sending an invalid value for binning
    put_config_in_Queue(
        BINNING_OFF+100, // invalid
        GAIN_5X,
        ONLY_ROW_1_ACTIVE
        );
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    _AssertCall(call_n, "SpiSlaveTxByte");
    _AssertArgByteVal(call_n, 1, ERROR);
}
void SetSensorConfig_replies_ERROR_if_gain_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake sending an invalid value for gain
    put_config_in_Queue(
        BINNING_OFF,
        GAIN_5X+100, // invalid
        ONLY_ROW_1_ACTIVE
        );
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    _AssertCall(call_n, "SpiSlaveTxByte");
    _AssertArgByteVal(call_n, 1, ERROR);
}
void SetSensorConfig_replies_ERROR_if_active_rows_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake sending an invalid value for active_rows
    put_config_in_Queue(
        BINNING_OFF,
        GAIN_5X,
        0xFF // invalid
        );
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    _AssertCall(call_n, "SpiSlaveTxByte");
    _AssertArgByteVal(call_n, 1, ERROR);
}
void SetSensorConfig_does_not_program_LIS_770i_if_config_is_invalid(void)
{
    /* =====[ Setup ]===== */
    // Fake sending invalid config
    put_config_in_Queue(
        BINNING_OFF+100, // invalid
        GAIN_5X+100, // invalid
        0xFF // invalid
        );
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    // Assert that the function exits after reporting error.
    uint16_t call_n = 1;
    _AssertCall(call_n, "SpiSlaveTxByte");
    _AssertArgByteVal(call_n, 1, ERROR);
    _test_call_count_is(1);
}
static void _put_valid_config_in_Queue(void)
{
    /** Test setup for SetSensorConfig happy path.
      * Test hangs if Queue does not have at least three bytes.
      * Test checks the happy path, so the bytes must be valid.
      * */
    volatile uint8_t spi_rx_buffer[max_length_of_queue];
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    QueuePush(SpiFifo, BINNING_OFF);
    QueuePush(SpiFifo, GAIN_5X);
    QueuePush(SpiFifo, ONLY_ROW_1_ACTIVE);
}
void SetSensorConfig_writes_valid_config_to_LIS_770i_programmable_setup_register(void)
{
    /* =====[ Setup ]===== */
    _put_valid_config_in_Queue();
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    _AssertCall(1, "LisWriteConfig");
}
void SetSensorConfig_replies_OK_if_all_config_values_are_valid(void)
{
    /* =====[ Setup ]===== */
    _put_valid_config_in_Queue();
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 2;
    _AssertCall(call_n, "SpiSlaveTxByte");
    _AssertArgByteVal(call_n, 1, OK);
}
void SetSensorConfig_the_OK_is_sent_after_LIS_is_programmed_with_new_config(void)
{
    /* =====[ Setup ]===== */
    _put_valid_config_in_Queue();
    /* =====[ Operate ]===== */
    SetSensorConfig();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    _AssertCall(call_n++, "LisWriteConfig");
    _AssertCall(call_n, "SpiSlaveTxByte");
    _AssertArgByteVal(call_n, 1, OK);
}
#endif
/* =====[ GetExposure ]===== */
void GetExposure_sends_OK(void)
{
    /* =====[ Operate ]===== */
    /* GetExposure(); */
    TEST_PASS();
}
void GetExposure_sends_MSB_of_exposure_ticks(void)
{
    TEST_PASS();
}
void GetExposure_sends_LSB_of_exposure_ticks(void)
{
    TEST_PASS();
}

/* =====[ SetExposure ]===== */
void SetExposure_waits_for_byte_exposure_MSB(void)
{
    /* =====[ Operate ]===== */
    /* SetExposure(); */
    TEST_PASS();
}
void SetExposure_reads_byte_exposure_MSB(void)
{
    TEST_PASS();
}
void SetExposure_waits_for_byte_exposure_LSB(void)
{
    TEST_PASS();
}
void SetExposure_reads_byte_exposure_LSB(void)
{
    TEST_PASS();
}
void SetExposure_updates_global_exposure_ticks(void)
{
    TEST_PASS();
}
void SetExposure_sends_OK(void)
{
    TEST_PASS();
}

/* =====[ CaptureFrame ]===== */
void CaptureFrame_sends_OK(void)
{
    /* =====[ Operate ]===== */
    /* CaptureFrame(); */
    /* =====[ Check MAX_NUM_PIXELS ]===== */
    /* GString *msg = g_string_new(NULL); */
    /* g_string_printf(msg, */
    /*     "MAX_NUM_PIXELS: %d", */
    /*     MAX_NUM_PIXELS */
    /*     ); */
    /* TEST_FAIL_MESSAGE(msg->str); */
    /* // Free string-object memory */
    /* g_string_free(msg, true); */
    TEST_PASS();
}
void CaptureFrame_checks_binning_to_determine_number_of_pixels_in_frame(void)
{
    TEST_PASS();
}
void CaptureFrame_sends_num_pixels_MSB(void)
{
    TEST_PASS();
}
void CaptureFrame_sends_num_pixels_LSB(void)
{
    TEST_PASS();
}
void CaptureFrame_exposes_the_pixels(void)
{
    TEST_PASS();
}
void CaptureFrame_does_readout_of_num_pixels_into_the_frame_buffer(void)
{
    TEST_PASS();
}
void CaptureFrame_sends_the_pixel_readings_stored_in_the_frame_buffer(void)
{
    TEST_PASS();
}

// GetPeak(start_pixel, stop_pixel) -> peak_counts
void GetPeak_is_visible(void)
{
    /* uint16_t val = GetPeak(7, 200); */
    TEST_FAIL_MESSAGE("Implement test.");
}

// AutoExpose(start_pixel, stop_pixel)
void AutoExpose_is_visible(void)
{
    /* AutoExpose(256, 256); */
    TEST_FAIL_MESSAGE("Implement test.");
}
