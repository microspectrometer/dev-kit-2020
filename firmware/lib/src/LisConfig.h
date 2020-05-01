/** \file
 * *LIS-770i configuration*
 * - sensor config is three bytes: `binning`, `gain`, and
 *   `active_rows`
 *   - `binning`: control LIS-770i pixel pitch
 *      - 0x00 (off) -- use 7.8µm pixel pitch
 *      - 0x01 (on) -- use 15.6µm pixel pitch
 *   - `gain`: there are four gain settings in the LIS-770i
 *      - 0x01 (1x)
 *      - 0x25 (2.5x) (0x25 is 37 in decimal)
 *      - 0x04 (4x)
 *      - 0x05 (5x)
 *   - `active_rows`: there are five rows in the LIS-770i
 *      - set the bit to turn the row on
 *      - clear the bit to turn the row off
 *      - the 5 least significant bits represent the 5 rows
 *      - bits [8..0]: xxx54321
 *        - `1` to `5` are the row numbers
 *        - `x` is don't care
 * .
 * *TODO: write documentation on configuring the LIS-770i*
 * */
#ifndef _LISCONFIG_H
#define _LISCONFIG_H
#include "stdint.h"

//! LIS-770i programmable parameters are type `lis_cfg`
typedef uint8_t lis_cfg;
extern lis_cfg binning;
extern lis_cfg gain;
extern lis_cfg active_rows;

//! LIS-770i parameter values are type `lis_cfg_val`
typedef uint8_t const lis_cfg_val;
extern lis_cfg_val BINNING_OFF;
extern lis_cfg_val BINNING_ON;
extern lis_cfg_val GAIN_1X;
extern lis_cfg_val GAIN_2X5;
extern lis_cfg_val GAIN_4X;
extern lis_cfg_val GAIN_5X;
extern lis_cfg_val ALL_ROWS_ACTIVE;
extern lis_cfg_val ONLY_ROW_1_ACTIVE;
extern lis_cfg_val ONLY_ROW_2_ACTIVE;
extern lis_cfg_val ONLY_ROW_3_ACTIVE;
extern lis_cfg_val ONLY_ROW_4_ACTIVE;
extern lis_cfg_val ONLY_ROW_5_ACTIVE;

#endif // _LISCONFIG_H

