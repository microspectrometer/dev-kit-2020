#include "VisCmd.h"
/* ------------------ */
/* | ---Commands--- | */
/* ------------------ */
void ReplyCommandInvalid(void);
void NullCommand(void);
void GetSensorLED(void);
void SetSensorLED(void);
void GetSensorConfig(void);
void SetSensorConfig(void);
void GetExposure(void);
void SetExposure(void);
void CaptureFrame(void);
void AutoExposure(void);
void GetAutoExposeConfig(void);
void SetAutoExposeConfig(void);

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
bool LedNumIsValid(bicolorled_num);
uint8_t ReadLedState(bicolorled_num);
bool AutoExposeConfigIsValid(uint8_t, uint16_t, uint16_t, uint16_t);

void LisReadout(uint16_t num_pixels)
{
    /** LisReadout behavior:\n 
      * - waits for Lis_Sync to go HIGH then go LOW\n 
      * - reads one pixel on each rising edge of Lis_Clk\n 
      * - LOOP: wait for the rising edge of Lis_Clk\n 
      * - LOOP: start the ADC conversion\n 
      * - LOOP: wait for 45 cycles of 10MHz clock\n 
      * - LOOP: start ADC readout\n 
      * - LOOP: wait for most significant byte of ADC readout\n 
      * - LOOP: save MSB to frame buffer\n 
      * - LOOP: wait for least significant byte of ADC readout\n 
      * - LOOP: save LSB to frame buffer\n 
      * */

    // wait for SYNC pulse to signify readout starts
    while( BitIsClear(Lis_pin1, Lis_Sync) ); // wait for RISING
    while(   BitIsSet(Lis_pin1, Lis_Sync) ); // wait for FALLING

    /* --------------------------------------------------------- */
    /* | LOOP: read one pixel on each rising edge of Lis clock | */
    /* --------------------------------------------------------- */
    uint16_t pixel_count = 0;
    uint8_t *pframe = frame;
    while( pixel_count++ < num_pixels)
    {
        // wait for rising edge of Lis clock
        _WaitForLisClkHigh();

        // start ADC conversion
        StartAdcConversion();

        /* --------------------------------------------------- */
        /* | wait at least 4.66µs for conversion to complete | */
        /* --------------------------------------------------- */
        // use _delay_loop_1 to count 45 ticks of the 10MHz osc
        // each loop iteration is 3 ticks
#ifndef USE_FAKES
        _delay_loop_1(15); // 15 * 3 = 45 -> 4.5µs plus overhead
#endif

        // start 16-bit ADC readout
        StartAdcReadout();

        // wait for MSB of ADC readout
        while (BitIsClear(UartSpi_UCSR0A, UartSpi_RXC0));

        // save MSB to frame buffer
        *(pframe++) = *UartSpi_UDR0;

        // wait for LSB of ADC readout
        while (BitIsClear(UartSpi_UCSR0A, UartSpi_RXC0));

        // save LSB to frame buffer
        *(pframe++) = *UartSpi_UDR0;
    }
}

uint16_t GetPeak(uint16_t const _start_pixel, uint16_t const _stop_pixel)
{
    /** # Algorithm
     *
     * - `peak` starts at 0
     * - walk the global `frame` buffer
     *   - start at `_start_pixel`
     *   - stop at `_stop_pixel`
     * - for each pixel:
     *   - if `pixval` > peak
     *     - `peak` = `pixval`
     * - return `peak`
     * */

    // start with a peak value of 0
    uint16_t peak = 0;

    // do not look for peak if _stop_pixel < _start_pixel
    if (_stop_pixel < _start_pixel) return peak;

    // convert pixel number to index into global frame buffer
    /** byte_index example:
     *                          v------- if _start_pixel is 7
     * pixel: 1  2  3  4  5  6  7  8
     *  byte: 01 23 45 67 89 AB CD EF
     *                          ^------- then byte_index is 12
     * */
    uint16_t byte_index = (_start_pixel-1) * 2; // 2 bytes per pixel

    // point at _start_pixel in the global frame buffer
    uint8_t * pframe = frame + byte_index;

    // check every pixel until _stop_pixel
    for (uint16_t pixnum = _start_pixel; pixnum++ <= _stop_pixel;)
    {
        uint16_t pixval = (*(pframe++)) << 8;
        pixval |= (*(pframe++));
        if (pixval > peak) peak = pixval;
    }
    return peak;
}

#ifdef USE_FAKES
#define LisExpose LisExpose_fake
#define LisReadout LisReadout_fake
#endif
uint16_t AutoExpose(void)
{
    /** AutoExpose behavior:\n 
      * - turns led1 red to indicate starting\n 
      * - sets max dark at 4500 counts\n 
      * - sets min exposure at 5 cycles\n 
      * - sets max exposure at 65535 cycles\n 
      * - sets min peak at target minus tolerance\n 
      * - clamps min peak at max dark if target minus tolerance is GREATER THAN target\n 
      * - clamps min peak at max dark if target minus tolerance is LESS THAN max dark\n 
      * - sets max peak at target plus tolerance\n 
      * - clamps max peak at 65535 counts if target plus tolerance is LESS THAN target\n 
      * - loops until done\n 
      * - exposes the LIS 770i pixels\n 
      * - reads pixel counts into global frame buffer\n 
      * - finds frame peak in range start pixel to stop pixel\n 
      * - is done if peak less than max dark AND exposure at max\n 
      * - scales exposure by 10 if peak less than max dark\n 
      * - clamps exposure at max exposure if 10 x exposure is GREATER THAN max exposure\n 
      * - scales exposure by half if peak ABOVE max peak\n 
      * - clamps exposure at min exposure if half exposure is LESS THAN min exposure\n 
      * - is done if peak BELOW min peak and exposure at max exposure\n 
      * - scales exposure by target div peak if peak BELOW min peak and exposure not at max\n 
      * - clamps exposure at max exposure if gain is GREATER THAN max exposure\n 
      * - is done if peak is in the target range\n 
      * - turns led1 green to indicate it hit the target range\n 
      * - gives up if it iterates for max tries\n 
      * */
    /** # AutoExpose uses a "Take Back Half" algorithm
      *
      * Find an exposure time that gets a peak within +/-5% of
      * the target peak value. On each iteration, multiply
      * exposure time, either by:
      * - `10`, if peak is *below* the **dark background**
      * - `1/2`, if peak is *above* the **target range**
      * - `gain`, if peak is *visible* but *below* the **target
      *   range**
      *   - `gain` = `target` **÷** `peak`
      *     - note `gain` is **always > 1**:
      *       - `target` is the *center* of the target range
      *       - `peak` is *below* the `target` range, so `peak` <
      *       `target`
      *       - then `target` **÷** `peak` must be > 1
      *     - for **precise** *division* with **integers**:
      *       - multiply exposure by `target` *BEFORE* dividing
      *       by `peak`:
      *         - ( `exposure_ticks` **x** `target` ) **÷** `peak`
      * .
      * The algorithm **gives up** if:
      *  - exposure time hits a limit
      *  - or the peak fails to land in the ±5% tolerance after
      *  10 iterations.
      *
      * The **large** tolerance, **±5%**, makes the algorithm
      * fast. It usually settles within a few iterations.
      */
    /** target range
      * ------------
      * The range of target values is configured by setting
      * `target` and `target_tolerance`. The recommended values
      * are:
      * - `target`: **46420** counts
      * - `target_tolerance`: **3277** counts
      * .
      * AutoExpose tries to get the peak counts between `min_peak` and
      * `max_peak`.
      * - `target - target_tolerance` is the `min_peak`
      * - `target + target_tolerance` is the `max_peak`
      * .
      * Using the recommended values, this results in:
      * - `min_peak`: **43143** counts
      * - `max_peak`: **49697** counts
      * .
      * **49697** counts is the *top* of the guaranteed **linear**
      * range of output values for the LIS-770i
      * - this value is based on the dev-kit design:
      *   - **16-bit** ADC
      *   - **1.8V** ADC voltage reference
      *   - LIS-770i power supply: **3.3V**
      * - and based on the LIS-770i programming configuration:
      *   - binning **on**
      *   - gain **1x**
      *   - **all rows** active
      * .
      * This target range, **[ 43143 : 49697 ]**, represents 10% of
      * the full-scale reading, **65535**.
      */
    /** max_dark
      * --------
      * Signal below `max_dark` is considered **no signal**
      * because it is *indistinguishable* from the dark
      * background.
      * - use **4500** counts for `max_dark`
      *   - **1000** counts is the **actual** approximate peak in
      *   the dark background when Chromation ships the dev-kit
      *   - **4500** counts is **well above** the *actual* peak
      *   dark background
      *     - setting `max_dark` to **4500** ensures the
      *     algorithm calculates a gain **only when necessary**
      *     - for signal below **4500** counts, the 10x gain is
      *     sufficient
      * - `max_dark` depends on:
      *   - exposure time
      *     - at longer exposures, peaks in the dark background
      *     are reduced because they are closer to the mean dark
      *     value
      *   - trim pot setting
      *     - trims the output offset voltage of the LIS-770i
      */
    /** # Algorithm Setup
      *
      * Initial state
      * -------------
      * AutoExpose is not done
      *
      * Method to get peak value
      * ------------------------
      * - expose the LIS-770i and readout one frame of data
      * - return the peak value in the frame
      * .
      * Method to clamp exposure time
      * -----------------------------
      * Exposure time is stored in 16-bit global `exposure_ticks`.
      *
      * Clamp adjustments to `exposure_ticks` between **1** and
      * **65535**.
      *
      * When multiplying `exposure_ticks` by:
      * - `10` or `gain`:
      *   - if result > 65535, clamp `exposure_ticks` at 65535
      * - `1/2`:
      *   - **÷2** is a bit-shift, so result is **integer
      *   truncated**
      *   - if result < 1, clamp `exposure_ticks` at 1
      */
    /** # Algorithm
      *
      * while **AutoExpose is not done**:
      *   - expose LIS-770i
      *   - readout LIS-770i, store in global `frame` buffer
      *   - get `peak` from global `frame` buffer
      *   - if there is no signal: `// peak <= max_dark`
      *     - if at max exposure time:
      *       - **AutoExpose is done**
      *     - else:
      *       - **10x** exposure time
      *   - elif `peak` is ABOVE target range: `// peak is >
      *   max_peak`
      *     - **TAKE BACK HALF** `// exposure time = 1/2 exposure time`
      *   - elif `peak` is BELOW target range: `// peak is <
      *   min_peak`
      *     - if at max exposure time:
      *       - **AutoExpose is done**
      *     - else:
      *       - **gain** = `target` **÷** `peak`
      *       - scale exposure time by **gain** `// exposure time
      *       = ( target x exposure time ) ÷ peak`
      *     - else: `// signal in target range: min_peak <= peak
      *     <= max_peak`
      *       - **AutoExpose is done**
      *     - increment iterations
      *     - if iterations is at max:
      *       - **AutoExpose is done**
      */
    
    // red means AutoExpose started
    BiColorLedRed(led_1);

    // report AutoExpose success/failure
    bool success = false;

    // return final peak value for inspecting algorithm behavior
    uint16_t peak=0;

    // determine number of pixels to readout
    uint16_t num_pixels;
    if (binning == BINNING_OFF) num_pixels = MAX_NUM_PIXELS;
    else num_pixels = MAX_NUM_PIXELS/2;

    /* -------------------- */
    /* | AutoExpose SETUP | */
    /* -------------------- */

    // give up after 10 tries
    // SetAutoExposeConfig guarantees at least one try.
    uint8_t iterations = 0;

    // calculate target range for peak values
    uint16_t min_peak = _MinPeak(target, target_tolerance);
    uint16_t max_peak = _MaxPeak(target, target_tolerance);

    /* ------------------- */
    /* | AutoExpose LOOP | */
    /* ------------------- */

    bool done = false;
    while (!done)
    {
        /* -------------- */
        /* | READ FRAME | */
        /* -------------- */

        // expose the LIS-770i pixels
        LisExpose();

        // readout the LIS-770i pixels into global frame buffer
        LisReadout(num_pixels);

        // find peak in range start_pixel : stop_pixel
        peak = GetPeak(start_pixel, stop_pixel);

        /* ------------------------ */
        /* | ADJUST EXPOSURE TIME | */
        /* ------------------------ */

        // one variable for all big calculation results
        uint32_t big_result; // stores up to 4294967295

        // any signal?
        if (peak < max_dark)
        {
            // stop if already at maximum exposure
            if (exposure_ticks == max_exposure) done = true;

            // otherwise, 10x exposure
            else
            {
                big_result = 10 * exposure_ticks;

                // clamp exposure at its maximum value
                if (big_result > max_exposure) big_result = max_exposure;
                exposure_ticks = big_result;
            }
        }

        // peak ABOVE target range?
        else if (peak > max_peak)
        {
            // TAKE BACK HALF
            exposure_ticks = exposure_ticks >> 1;

            // clamp exposure at its minimum value
            if (exposure_ticks < min_exposure) exposure_ticks = min_exposure;
        }

        // peak BELOW target range?
        else if (peak < min_peak)
        {
            // stop if already at maximum exposure
            if (exposure_ticks == max_exposure) done = true;

            // otherwise, scale exposure by gain
            else
            {
                // -------------------------------------------------
                // | AT LEAST ONE OPERAND MUST BE CAST AS uint32_t |
                // | OTHERWISE THE OPERATION IS TREATED AS 16-bit  |
                // -------------------------------------------------
                big_result = (uint32_t)target * exposure_ticks;
                big_result = big_result / peak;

                // clamp exposure at its maximum value
                if (big_result > max_exposure) big_result = max_exposure;
                exposure_ticks = big_result;
            }
        }

        // peak is in the target range
        else
        {
            done = true;
            success = true;
            // green means target hit
            BiColorLedGreen(led_1);
        }

        // give up if peak fails to hit target range after many tries
        if (++iterations == max_tries) done = true;
    }
    // return success as MSB and iterations as LSB
    uint16_t result = (uint16_t)(success << 8) | iterations;
    return result;
}
#ifdef USE_FAKES
#undef LisExpose
#undef LisReadout
#endif


