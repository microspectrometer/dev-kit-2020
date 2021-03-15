#include "unity.h"
#include "Mock.h"
#include "test_AutoExpose.h"
#include "AutoExpose.h"
#include "VisCmd.h" // AutoExpose() defined here

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

/* =====[ AutoExpose ]===== */
void AutoExpose_turns_led1_red_to_indicate_starting(void)
{
    /* =====[ Operate ]===== */
    AutoExpose();

    /* =====[ Test ]===== */
    TEST_PASS();
}
void AutoExpose_sets_min_peak_at_target_minus_tolerance(void)
{
    /* =====[ Setup ]===== */
    target = 46420;
    target_tolerance = 3277;
    /* =====[ Operate ]===== */
    uint16_t min_peak = _MinPeak(target, target_tolerance);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(target-target_tolerance, min_peak);
}
void AutoExpose_clamps_min_peak_at_max_dark_if_target_minus_tolerance_is_GREATER_THAN_target(void)
{
    /* =====[ Setup ]===== */
    max_dark = 4500;
    target = 46420;
    target_tolerance = 46421;
    // greater than test of (a,b) tests for b>a
    TEST_ASSERT_GREATER_THAN_UINT16(1,2); // example
    TEST_ASSERT_GREATER_THAN_UINT16(target,target-target_tolerance);
    /* =====[ Operate ]===== */
    uint16_t min_peak = _MinPeak(target, target_tolerance);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(max_dark, min_peak);
}
void AutoExpose_clamps_min_peak_at_max_dark_if_target_minus_tolerance_is_LESS_THAN_max_dark(void)
{
    /* =====[ Setup ]===== */
    max_dark = 4500;
    target = 46420;
    target_tolerance = 46400;
    // less than test of (a,b) tests for b<a
    TEST_ASSERT_LESS_THAN_UINT16(2,1); // example
    TEST_ASSERT_LESS_THAN_UINT16(max_dark,target-target_tolerance);
    /* =====[ Operate ]===== */
    uint16_t min_peak = _MinPeak(target, target_tolerance);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(max_dark, min_peak);
}
void AutoExpose_sets_max_peak_at_target_plus_tolerance(void)
{
    /* =====[ Setup ]===== */
    target = 46420;
    target_tolerance = 3277;
    /* =====[ Operate ]===== */
    uint16_t max_peak = _MaxPeak(target, target_tolerance);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(target+target_tolerance, max_peak);
}
void AutoExpose_clamps_max_peak_at_65535_counts_if_target_plus_tolerance_is_LESS_THAN_target(void)
{
    /* =====[ Setup ]===== */
    target = 46420;
    target_tolerance = 20000;
    TEST_ASSERT_LESS_THAN_UINT16(2,1); // example
    TEST_ASSERT_LESS_THAN_UINT16(
        target,
        target+target_tolerance
        );
    /* =====[ Operate ]===== */
    uint16_t max_peak = _MaxPeak(target, target_tolerance);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(UINT16_MAX, max_peak);
}
void AutoExpose_loops_until_done(void)
{
    TEST_PASS();
}
void AutoExpose_exposes_the_pixels(void)
{
    /* =====[ Operate ]===== */
    AutoExpose();
    /* =====[ Test ]===== */
#ifdef LIS
    _AssertCall(1, "LisExpose");
#endif
#ifdef S13131
    _AssertCall(1, "S13131Expose");
#endif
}
void AutoExpose_reads_pixel_counts_into_global_frame_buffer(void)
{
    /* =====[ Operate ]===== */
    AutoExpose();
    /* =====[ Test ]===== */
#ifdef LIS
    _AssertCall(2, "LisReadout");
#endif
#ifdef S13131
    _AssertCall(2, "S13131Readout");
#endif
}

void AutoExpose_finds_frame_peak_in_range_start_pixel_to_stop_pixel(void)
{
    /* =====[ Setup ]===== */
    // mock a frame with a peak
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0xABCD at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0xAB;
    frame[byte_index] = 0xCD;
    uint16_t start_pixel = 10;
    uint16_t stop_pixel = 200;
    TEST_ASSERT_LESS_THAN_UINT16(2,1); // example
    TEST_ASSERT_LESS_THAN_UINT16(pixnum,start_pixel); // example
    TEST_ASSERT_GREATER_THAN_UINT16(1,2); // example
    TEST_ASSERT_GREATER_THAN_UINT16(pixnum,stop_pixel); // example
    /* =====[ Operate ]===== */
    uint16_t peak = GetPeak(start_pixel, stop_pixel);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT16(0xABCD, peak);
}
void AutoExpose_is_done_if_peak_less_than_max_dark_AND_exposure_at_max(void)
{
    /* =====[ Setup ]===== */
    // Expect algorithm to stop after first iteration.
    // Set max_tries above expected number of iterations.
    max_tries = 2;
    // Set target well above max_dark
    target = 46420; target_tolerance = 3277;
    // Set typical max_dark
    max_dark = 0x1194; // 4500 in hexadecimal
    TEST_ASSERT_EQUAL_UINT16(max_dark,4500);
    // fake frame of data with peak less than max_dark
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0x1193 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0x11;
    frame[byte_index] = 0x93;
    // autoexpose pixel range includes pixel with 0x1193 counts
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    min_exposure = 5;
    max_exposure = UINT16_MAX;
    // Put exposure at max
    /* ------------------------------- */
    /* | THIS IS WHAT'S BEING TESTED | */
    /* ------------------------------- */
    exposure_ticks = UINT16_MAX;
    AutoExpose();
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // expect algorithm to finsh on first try
    TEST_ASSERT_EQUAL_UINT8(1, iterations);
}
void AutoExpose_scales_exposure_by_10_if_peak_less_than_max_dark(void)
{
    /* =====[ Setup ]===== */
    // Set target well above max_dark
    target = 46420; target_tolerance = 3277;
    // Set typical max_dark
    max_dark = 0x1194; // 4500 in hexadecimal
    TEST_ASSERT_EQUAL_UINT16(max_dark,4500);
    // fake frame of data with peak less than max_dark
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0x1193 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0x11;
    frame[byte_index] = 0x93;
    // autoexpose pixel range includes pixel with 0x1193 counts
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    // Put exposure below 65535/10
    /* ------------------------------- */
    /* | THIS IS WHAT'S BEING TESTED | */
    /* ------------------------------- */
    min_exposure = 5;
    max_exposure = UINT16_MAX;
    uint16_t initial_exposure = 50;
    exposure_ticks = initial_exposure;
    // ---------------------------------------------------------
    // | This must run only one iteration to inspect exposure. |
    // ---------------------------------------------------------
    max_tries = 1;
    AutoExpose();
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // make sure only one iteration was performed
    TEST_ASSERT_EQUAL_UINT8(1, iterations);
    // The test: expect exposure is multiplied by 10
    TEST_ASSERT_EQUAL_UINT16(initial_exposure*10, exposure_ticks);
}
void AutoExpose_clamps_exposure_at_max_exposure_if_10_x_exposure_is_GREATER_THAN_max_exposure(void)
{
    /* =====[ Setup ]===== */
    // Set target well above max_dark
    target = 46420; target_tolerance = 3277;
    // Set typical max_dark
    max_dark = 0x1194; // 4500 in hexadecimal
    TEST_ASSERT_EQUAL_UINT16(max_dark,4500);
    // fake frame of data with peak less than max_dark
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0x1193 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0x11;
    frame[byte_index] = 0x93;
    // autoexpose pixel range includes pixel with 0x1193 counts
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    min_exposure = 5;
    max_exposure = UINT16_MAX;
    // Put exposure above 65535/10
    /* ------------------------------- */
    /* | THIS IS WHAT'S BEING TESTED | */
    /* ------------------------------- */
    uint16_t initial_exposure = 10000;
    exposure_ticks = initial_exposure;
    // ---------------------------------------------------------
    // | This must run only one iteration to inspect exposure. |
    // ---------------------------------------------------------
    max_tries = 1;
    AutoExpose();
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // make sure only one iteration was performed
    TEST_ASSERT_EQUAL_UINT8(1, iterations);
    // The test: expect exposure clamps at 65535
    TEST_ASSERT_EQUAL_UINT16(UINT16_MAX, exposure_ticks);
}
void AutoExpose_scales_exposure_by_half_if_peak_ABOVE_max_peak(void)
{
    /* =====[ Setup ]===== */
    // Set target well above max_dark
    target = 46420; target_tolerance = 3277;
    uint16_t max_peak = target+target_tolerance;
    uint16_t peak = 0xC222;
    TEST_ASSERT_GREATER_THAN_UINT16(1,2); // example
    TEST_ASSERT_GREATER_THAN_UINT16(max_peak,peak);
    // Set typical max_dark
    max_dark = 4500;
    // fake frame of data with peak ABOVE max_peak
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0xC222 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0xC2;
    frame[byte_index] = 0x22;
    // autoexpose pixel range includes peak pixel
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    min_exposure = 5;
    max_exposure = UINT16_MAX;
    // Put exposure above 11 (so that half exposure > 5)
    /* ------------------------------- */
    /* | THIS IS WHAT'S BEING TESTED | */
    /* ------------------------------- */
    uint16_t initial_exposure = 500; // PASS for: 10:65535
    exposure_ticks = initial_exposure;
    // ---------------------------------------------------------
    // | This must run only one iteration to inspect exposure. |
    // ---------------------------------------------------------
    max_tries = 1;
    AutoExpose();
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // make sure only one iteration was performed
    TEST_ASSERT_EQUAL_UINT8(1, iterations);
    // The test: expect exposure is scaled by half
    TEST_ASSERT_EQUAL_UINT16(initial_exposure/2, exposure_ticks);
    // >>1 is the same as dividing by 2
    TEST_ASSERT_EQUAL_UINT16(initial_exposure>>1, exposure_ticks);
}
void AutoExpose_clamps_exposure_at_min_exposure_if_half_exposure_is_LESS_THAN_min_exposure(void)
{
    /* =====[ Setup ]===== */
    // Set target well above max_dark
    target = 46420; target_tolerance = 3277;
    uint16_t max_peak = target+target_tolerance;
    uint16_t peak = 0xC222;
    TEST_ASSERT_GREATER_THAN_UINT16(1,2); // example
    TEST_ASSERT_GREATER_THAN_UINT16(max_peak,peak);
    // Set typical max_dark
    max_dark = 4500;
    // fake frame of data with peak
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0xC222 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0xC2;
    frame[byte_index] = 0x22;
    // autoexpose pixel range includes peak pixel
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    max_exposure = UINT16_MAX;
    /* ------------------------------- */
    /* | THIS IS WHAT'S BEING TESTED | */
    /* ------------------------------- */
    min_exposure = 5;
    // Put exposure below 10 (so that half exposure < 5)
    exposure_ticks = 9;
    // ---------------------------------------------------------
    // | This must run only one iteration to inspect exposure. |
    // ---------------------------------------------------------
    max_tries = 1;
    AutoExpose();
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // make sure only one iteration was performed
    TEST_ASSERT_EQUAL_UINT8(1, iterations);
    // The test: expect exposure clamps at min_exposure
    TEST_ASSERT_EQUAL_UINT16(min_exposure, exposure_ticks);
}
void AutoExpose_is_done_if_peak_BELOW_min_peak_and_exposure_at_max_exposure(void)
{
    /* =====[ Setup ]===== */
    // Expect algorithm to stop after first iteration.
    // Set max_tries above expected number of iterations.
    max_tries = 2;
    // Set target below min_peak
    target = 46420; target_tolerance = 3277;
    uint16_t min_peak = target - target_tolerance;
    uint16_t peak = 0xA886;
    TEST_ASSERT_LESS_THAN_UINT16(2,1); // example
    TEST_ASSERT_LESS_THAN_UINT16(min_peak,peak); // example

    // Set typical max_dark
    max_dark = 0x1194; // 4500 in hexadecimal
    TEST_ASSERT_EQUAL_UINT16(max_dark,4500);
    // fake frame of data with peak
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0xA886 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0xA8;
    frame[byte_index] = 0x86;
    // autoexpose pixel range includes peak pixel
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    min_exposure = 5;
    max_exposure = UINT16_MAX;
    // Put exposure at max
    /* ------------------------------- */
    /* | THIS IS WHAT'S BEING TESTED | */
    /* ------------------------------- */
    exposure_ticks = UINT16_MAX;
    AutoExpose();
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // expect algorithm to finsh on first try
    TEST_ASSERT_EQUAL_UINT8(1, iterations);
}
void AutoExpose_scales_exposure_by_target_div_peak_if_peak_BELOW_min_peak_and_exposure_not_at_max(void)
{
    /* =====[ Setup ]===== */
    target = 46420; target_tolerance = 3277;
    uint16_t min_peak = target - target_tolerance;
    // Set peak below min_peak
    uint16_t peak = 0xA886;
    TEST_ASSERT_LESS_THAN_UINT16(2,1); // example
    TEST_ASSERT_LESS_THAN_UINT16(min_peak,peak); // example

    // Set typical max_dark
    max_dark = 0x1194; // 4500 in hexadecimal
    TEST_ASSERT_EQUAL_UINT16(max_dark,4500);
    // fake frame of data with peak
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0xA886 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0xA8;
    frame[byte_index] = 0x86;
    // autoexpose pixel range includes peak pixel
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    min_exposure = 5;
    max_exposure = UINT16_MAX;
    // Put exposure below max
    /* ------------------------------- */
    /* | THIS IS WHAT'S BEING TESTED | */
    /* ------------------------------- */
    uint16_t initial_exposure = 50;
    exposure_ticks = initial_exposure;
    // ---------------------------------------------------------
    // | This must run only one iteration to inspect exposure. |
    // ---------------------------------------------------------
    max_tries = 1;
    AutoExpose();
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // make sure only one iteration was performed
    TEST_ASSERT_EQUAL_UINT8(1, iterations);

    // The test: expect exposure scales by target/peak

    /* ---------------------------------------------------- */
    /* |       NOTE ON UNSIGNED INTEGER ARITHMETIC        | */
    /* | (target/peak)*exposure != (target*exposure)/peak | */
    /* | example: using test values, lhs == 50, rhs == 53 | */
    /* ---------------------------------------------------- */

    TEST_ASSERT_NOT_EQUAL((target/peak)*initial_exposure, exposure_ticks);
    TEST_ASSERT_EQUAL_UINT16((target*initial_exposure)/peak, exposure_ticks);
}
void AutoExpose_clamps_exposure_at_max_exposure_if_gain_is_GREATER_THAN_max_exposure(void)
{
    /* =====[ Setup ]===== */
    target = 46420; target_tolerance = 3277;

    // Set typical max_dark
    max_dark = 0x1194; // 4500 in hexadecimal
    TEST_ASSERT_EQUAL_UINT16(max_dark,4500);

    // ---------------------------------------------
    // | Biggest gain happens when peak = max_dark |
    // |     Using test values, gain = 10.3        |
    // ---------------------------------------------

    // fake frame of data with peak at max_dark
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0x1194 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0x11;
    frame[byte_index] = 0x94;
    // autoexpose pixel range includes peak pixel
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    min_exposure = 5;
    max_exposure = UINT16_MAX;
    // Put exposure below max
    /* ------------------------------- */
    /* | THIS IS WHAT'S BEING TESTED | */
    /* ------------------------------- */
    // set exposure above 6353 to test clamping action
    uint16_t initial_exposure = 6354; // PASS: 6354:65535
    exposure_ticks = initial_exposure;
    // ---------------------------------------------------------
    // | This must run only one iteration to inspect exposure. |
    // ---------------------------------------------------------
    max_tries = 1;
    AutoExpose();
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // make sure only one iteration was performed
    TEST_ASSERT_EQUAL_UINT8(1, iterations);

    // The test: expect exposure clamps at max_exposure
    TEST_ASSERT_EQUAL_UINT16(UINT16_MAX, exposure_ticks);
}
void AutoExpose_is_done_if_peak_is_in_the_target_range(void)
{
    /* =====[ Setup ]===== */
    // Expect algorithm to stop after first iteration.
    // Set max_tries above expected number of iterations.
    max_tries = 2;

    // Use the recommended values
    target = 46420; target_tolerance = 3277; max_dark = 4500;

    // put peak in the target range by making peak = target
    uint16_t peak = 0xB554;
    TEST_ASSERT_EQUAL_UINT16(target, peak);

    // fake frame of data with peak in target range
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value 0xB554 at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0xB5;
    frame[byte_index] = 0x54;
    // autoexpose pixel range includes peak pixel
    start_pixel = 10; stop_pixel = 200;
    /* =====[ Operate ]===== */
    min_exposure = 5;
    max_exposure = UINT16_MAX;
    uint16_t initial_exposure = 50;
    exposure_ticks = initial_exposure;
    AutoExpose();
    // make sure AutoExpose did not change exposure
    TEST_ASSERT_EQUAL_UINT16(initial_exposure, exposure_ticks);
    /* =====[ Test ]===== */
    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    // The test: only one iteration is performed
    TEST_ASSERT_EQUAL_UINT8(1, iterations);
}
void AutoExpose_turns_led1_green_to_indicate_it_hit_the_target_range(void)
{
    TEST_PASS();
}
void AutoExpose_gives_up_if_it_iterates_for_max_tries(void)
{
    /* =====[ Setup ]===== */
    // use typical values
    target = 46420; target_tolerance = 3277; max_dark = 4500;
    min_exposure = 5; max_exposure = UINT16_MAX;
    exposure_ticks = 50;

    // Set peak one count below min_peak to put it *just* outside
    // the target range. AutoExpose should initially follow path
    // to apply calculated gain to exposure. I'm trying to set up
    // a scenario where it takes a while for the exposure to max
    // out.

    uint16_t min_peak = target - target_tolerance;
    uint16_t peak = 0xA886;
    TEST_ASSERT_EQUAL_UINT16(min_peak-1, peak);

    // fake frame of data with peak
    // TODO(sustainablelab): Why /2? Should be *2.
    for (uint16_t byte_index; byte_index++ < MAX_NUM_PIXELS/2;)
    {
        frame[byte_index] = 0;
    }
    // stick peak counts value at pixel 100
    uint16_t const pixnum = 100;
    uint16_t byte_index = (pixnum-1)*2;
    frame[byte_index++] = 0xA8;
    frame[byte_index] = 0x86;
    // autoexpose pixel range includes peak pixel
    start_pixel = 10; stop_pixel = 200;

    /* =====[ Operate ]===== */
    // ---------------------------------
    // | This is the value under test. |
    // ---------------------------------
    // 101 tries was the best I could do.
    // In theory, some combo of values makes 255 possible.
    // 255 is the maximum number of tries because of the data type.
    // But 101 is good enough to show the loop working.
    //
    // Note that 0 does not work, but SetAutoExposeConfig returns
    // an ERROR if the user tries to set max_tries to 0.
    max_tries = 50; // PASS: 1:101
    AutoExpose();

    /* =====[ Test ]===== */
    // -------------------------------------------
    // | The peak does not change (it is faked). |
    // | But the exposure DOES change. So the    |
    // | loop does NOT always follows the same   |
    // | path. Expect loop runs for max_tries.   |
    // -------------------------------------------

    // two calls are faked, so divide NumberOfActualCalls by 2
    uint8_t iterations = NumberOfActualCalls(mock)/2;
    TEST_ASSERT_EQUAL_UINT8(max_tries, iterations);
}
