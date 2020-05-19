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
uint8_t max_tries;

//! AutoExpose() ignores pixels below `start_pixel`.
uint16_t start_pixel;

//! AutoExpose() ignores pixels above `stop_pixel`.
uint16_t stop_pixel;

//! target peak counts for AutoExpose().
uint16_t target;

//! `target` ± `target_tolerance` is the target peak counts range for AutoExpose().
uint16_t target_tolerance;

#endif // _AUTOEXPOSE_H
