// ---API (Go to the Doxygen documentation of this file)---
/** \file VisCmd.h
 * # API
 * void NullCommand(void);\n 
 * void ReplyCommandInvalid(void);\n 
 * void SetSensorConfig(void);\n 
 * */
#ifndef _VISCMD_H
#define _VISCMD_H
#include "SpiSlave.h"
#include "StatusCode.h"
#include "LisConfig.h"
#include "Queue.h"

extern volatile Queue_s * SpiFifo; // definiton in `main()` translation unit

inline void NullCommand(void)
{ //! Do nothing.
}

#ifdef USE_FAKES
#include "SpiSlave_faked.h" // declare fakes
#endif

#ifdef USE_FAKES
#define SpiSlaveTxByte SpiSlaveTxByte_fake
#endif
inline void ReplyCommandInvalid(void)
{
    /** ReplyCommandInvalid behavior:\n 
      * - transmits one byte over SPI\n 
      * - sends byte INVALID CMD\n 
      * */
    SpiSlaveTxByte(INVALID_CMD);
}
#ifdef USE_FAKES
#undef SpiSlaveTxByte
#endif

#ifdef USE_FAKES
#define SpiSlaveTxByte SpiSlaveTxByte_fake
#endif
inline void SetSensorConfig(void)
{
    /** SetSensorConfig behavior:\n 
      * - receives three bytes of config from Bridge\n 
      * - replies ERROR if binning is invalid\n 
      * - replies ERROR if gain is invalid\n 
      * - replies ERROR if active rows is invalid\n 
      * - replies OK if all config values are valid\n 
      * */
    // Get config values
    while (QueueIsEmpty(SpiFifo)); // 5 cycles
    binning = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo)); // 5 cycles
    gain = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo)); // 5 cycles
    active_rows = QueuePop(SpiFifo);
    // Reply with error if any config value is invalid
    // TODO: replace with LisConfigIsValid(binning, gain, active_rows)
    if (
            ((binning != BINNING_OFF) && (binning != BINNING_ON))
        ||  ((gain != GAIN_1X) && (gain != GAIN_2X5) && (gain != GAIN_4X) && (gain != GAIN_5X))
        ||  ((active_rows & 0xE0) != 0x00)
       )
    {
        SpiSlaveTxByte(ERROR);
        return;
    }
    /* LisWriteConfig(binning, gain, active_rows); */
    SpiSlaveTxByte(OK);
}
#ifdef USE_FAKES
#undef SpiSlaveTxByte
#endif

#endif // _VISCMD_H
