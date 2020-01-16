/** \file
 * *LIS-770i configuration*
 * sensor config is three bytes
 * TODO: write documentation on configuring the LIS-770i
 * */
#ifndef _LISCONFIGS_H
#define _LISCONFIGS_H
#include "LisConfig.h"

lis_cfg binning;
lis_cfg gain;
lis_cfg active_rows;
lis_cfg_val BINNING_OFF = 0x02;
lis_cfg_val GAIN_1X = 0x01;
lis_cfg_val GAIN_2X5 = 0x25;
lis_cfg_val GAIN_4X = 0x04;
lis_cfg_val GAIN_5X = 0x05;
lis_cfg_val ALL_ROWS_ACTIVE = 0x1F; // 0b00011111 : all five rows


#endif // _LISCONFIGS_H
