/** \file
 * *LIS-770i configuration*
 * sensor config is three bytes
 * TODO: write documentation on configuring the LIS-770i
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

