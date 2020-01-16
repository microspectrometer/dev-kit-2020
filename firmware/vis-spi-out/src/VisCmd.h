// ---API (Go to the Doxygen documentation of this file)---
/** \file VisCmd.h
 * # API
 * void ReplyCommandInvalid(void);\n 
 * */
#ifndef _VISCMD_H
#define _VISCMD_H
#include "SpiSlave.h"
#include "StatusCode.h"
#include "LisConfig.h"

#ifdef USE_FAKES
#include "SpiSlave_faked.h" // declare fakes
#endif

#ifdef USE_FAKES
#define SpiSlaveTxByte SpiSlaveTxByte_fake
#endif
inline void ReplyCommandInvalid(void)
{
    SpiSlaveTxByte(INVALID_CMD);
}
#ifdef USE_FAKES
#undef SpiSlaveTxByte
#endif
inline void NullCommand(void){}
inline void SetSensorConfig(void)
{
    /** SetSensorConfig behavior:\n 
      * - receives three bytes of config from Bridge\n 
      * */
    binning = BINNING_OFF;
    gain = GAIN_5X;
    active_rows = ALL_ROWS_ACTIVE;
}

#endif // _VISCMD_H
