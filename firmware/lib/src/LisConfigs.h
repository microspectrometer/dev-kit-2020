/** \file
 * See LisConfig.h for context.\n
 *\n
 * Also see `cfg/microspec.json` in the Python API repository.\n
 * - Section `"global"` defines the **LIS-770i cfg byte codes.**\n
 * - The **LIS-770i cfg byte codes** definitions are duplicated
 *   in this header file.\n
 * */
#ifndef _LISCONFIGS_H
#define _LISCONFIGS_H
#include "LisConfig.h"

lis_cfg binning;
lis_cfg gain;
lis_cfg active_rows;
lis_cfg_val BINNING_OFF = 0x00;
lis_cfg_val BINNING_ON = 0x01;
lis_cfg_val GAIN_1X = 0x01;
lis_cfg_val GAIN_2X5 = 0x25;
lis_cfg_val GAIN_4X = 0x04;
lis_cfg_val GAIN_5X = 0x05;
lis_cfg_val ALL_ROWS_ACTIVE = 0x1F;   // 0b00011111 : all five rows
lis_cfg_val ONLY_ROW_1_ACTIVE = 0x01; // 0b00000001
lis_cfg_val ONLY_ROW_2_ACTIVE = 0x02; // 0b00000010
lis_cfg_val ONLY_ROW_3_ACTIVE = 0x04; // 0b00000100
lis_cfg_val ONLY_ROW_4_ACTIVE = 0x08; // 0b00001000
lis_cfg_val ONLY_ROW_5_ACTIVE = 0x10; // 0b00010000

#endif // _LISCONFIGS_H
