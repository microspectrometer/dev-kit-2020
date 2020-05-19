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

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
bool LedNumIsValid(bicolorled_num);
uint8_t ReadLedState(bicolorled_num);
void LisReadout(uint16_t num_pixels);

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

void AutoExpose(void)
{
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

    // determine number of pixels
    uint16_t num_pixels;
    if (binning == BINNING_OFF) num_pixels = MAX_NUM_PIXELS;
    else num_pixels = MAX_NUM_PIXELS/2;

    /* -------------------- */
    /* | AutoExpose SETUP | */
    /* -------------------- */

    // give up after 10 tries
    uint8_t iterations = 0;

    // calculate gain only when signal is above max_dark
    uint16_t const max_dark = 4500;
    /* -------------------------------------------------------- */
    /* | TODO: move this to the SetAutoExposureConfig command | */
    /* -------------------------------------------------------- */
    // clamp target to not be below max_dark
    if (target < max_dark) target = max_dark;

    // clamp exposure time
    uint16_t const min_exposure = 1;
    uint16_t const max_exposure = 65535;

    // target range for peak values
    uint16_t min_peak = target - target_tolerance;
    uint16_t max_peak = target + target_tolerance;
    /* -------------------------------------------------------- */
    /* | TODO: move this to the SetAutoExposureConfig command | */
    /* -------------------------------------------------------- */
    // clamp min_peak to not go below max_dark
    if ( min_peak < max_dark) min_peak = max_dark;
    // clamp target range to guard against arithmetic wrap-around
    if ( min_peak > target) min_peak = max_dark;
    if ( max_peak < target) max_peak = 65535;

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
        uint16_t peak = GetPeak(start_pixel, stop_pixel);

        /* ------------------------ */
        /* | ADJUST EXPOSURE TIME | */
        /* ------------------------ */

        // one variable for all big calculation results
        uint32_t big_result;

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
                if (big_result > max_exposure) exposure_ticks = max_exposure;
                else exposure_ticks = big_result;
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
                big_result = target * exposure_ticks;
                big_result = big_result / peak;

                // clamp exposure at its maximum value
                if (big_result > max_exposure) exposure_ticks = max_exposure;
                else exposure_ticks = big_result;
            }
        }

        // peak is in the target range
        else done = true;

        // give up if peak fails to hit target range after many tries
        if (++iterations == max_tries) done = true;

        (void) peak;
    }
}


