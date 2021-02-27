/** \file AutoExpose.h
 * # Global AutoExpose config
 *
 * AutoExpose maximum number of tries
 * ----------------------------------
 * AutoExpose gives up if it cannot hit the target peak counts
 * range after `max_tries` attempts. Recommended value is **10**.
 * Use smaller numbers to speed up AutoExpose. Set the value to
 * **1** to manually trigger each iteration and get a sense of
 * how many tries are necessary to hit the target range. The
 * range of allowed values is 1 to 255.
 *
 * AutoExpose pixel range
 * ----------------------
 * Recommended values for `start_pixel` and `stop_pixel` depend
 * on the spectrometer pixel-to-wavelength map file:
 * - `start_pixel`: smallest pixel number in the map file
 * - `stop_pixel`: largest pixel number
 * .
 * The default pixel range on power-up is:
 * - `start_pixel = 7`
 * - `stop_pixel = 392`
 *
 * With binning on, the default pixel range is also the allowed
 * range of values. Attempting to exceed this range, it is
 * clamped at these boundaries. With binning off, the allowed
 * range is `start_pixel = 14`, `stop_pixel = 784`.
 *
 * AutoExpose target peak counts range
 * -----------------------------------
 * Recommended values for `target` and `target_tolerance` are:
 * - `target`: **46420** counts
 * - `target_tolerance`: **3277** counts
 * .
 * See details in the Doxygen for AutoExpose(). Allowed values
 * for `target` are in the range 4500 counts to 65535 counts.
 * Attempting to set a `target` less than **4500** counts,
 * `target` is clamped at **4500** counts. If the combination of
 * `target` and `target_tolerance` results in target ranges
 * extending below **4500** counts or above **65535** counts, the
 * target tolerance is ignored and the target range is clamped at
 * these boundaries.
 * */
#ifndef _AUTOEXPOSE_H
#define _AUTOEXPOSE_H

#include <stdint.h>
#include <stdbool.h>

//! maximum number of tries before AutoExpose() gives up
extern uint8_t max_tries;

//! AutoExpose() ignores pixels below `start_pixel`.
extern uint16_t start_pixel;

//! AutoExpose() ignores pixels above `stop_pixel`.
extern uint16_t stop_pixel;

//! target peak counts for AutoExpose().
extern uint16_t target;

//! `target` ± `target_tolerance` is the target peak counts range for AutoExpose().
extern uint16_t target_tolerance;

//! `max_dark` is a conservative estimate on the largest dark offset
extern uint16_t max_dark;

/** `min_exposure` is 100 microseconds (five 20µs-cycles)\n 
 * This is a safe lower limit to avoid dead frames.
 * */
extern uint16_t min_exposure;

/** `max_exposure` is 1.31 seconds (65535 20µs-cycles)\n 
 * This is the 16-bit limit, UINT16_MAX, but it is also a
 * practical limit on waiting time for one frame.\n 
 * For longer exposure times:
 * - change the datatype in firmware to uint32_t
 * - revise all transmissions of exposure_ticks in VisCmd.h and
 *   UsbCmd.h to send/receive four bytes
 * - update the Python API by changing all occurrences of
 *   `cycles` in the JSON file from size:2 to size:4.
 * .
 * */
extern uint16_t max_exposure;

inline uint16_t _MinPeak(uint16_t target, uint16_t target_tolerance)
{
    uint16_t min_peak = target-target_tolerance;

    /* ------------------------------ */
    /* | clamp min_peak at max_dark | */
    /* ------------------------------ */

    // guard against arithmetic wrap around where target - tol > target
    if ( min_peak > target) min_peak = max_dark;
    // guard against large tolerance where target - tol < max_dark
    else if ( min_peak < max_dark) min_peak = max_dark;
    return min_peak;
}

// ---------------------------------------------------------
// |                       stdint.h                        |
// |                defines UINT16_MAX as 65535            |
// | (both /usr/include/stdint.h and avr/include/stdint.h) |
// ---------------------------------------------------------

inline uint16_t _MaxPeak(uint16_t target, uint16_t target_tolerance)
{
    uint16_t max_peak = target + target_tolerance;

    /* --------------------------- */
    /* | clamp max_peak at 65535 | */
    /* --------------------------- */
    // guard against arithmetic wrap around where target + tol < target
    if ( max_peak < target) max_peak = UINT16_MAX;
    return max_peak;
}

#endif // _AUTOEXPOSE_H
