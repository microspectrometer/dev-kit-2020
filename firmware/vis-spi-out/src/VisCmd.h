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
#include "Lis.h"
#include "LisConfig.h"
#include "Queue.h"
#include "BiColorLed.h"

extern volatile Queue_s * SpiFifo; // definiton in `main()` translation unit

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
inline uint8_t ReadLedState(uint8_t led_num) // -> led_state
{
    /** ReadLedState behavior:\n 
      * - returns OFF if LED is off\n 
      * - returns GREEN if LED is on and green\n 
      * - returns RED if LED is on and red\n 
      * - see led_state in StatusCodes.h
      * */

    if (BitIsClear(BiColorLed_ddr, led_num))
        return OFF;
    else // LED is on
        return BitIsClear(BiColorLed_port, led_num)
            ? GREEN : RED;
}

/* ------------------ */
/* | ---Commands--- | */
/* ------------------ */
inline void NullCommand(void)
{ //! Do nothing.
}

inline void GetSensorLED(void)
{
    /** GetSensorLED behavior:\n 
      * - waits for byte led num\n 
      * - reads byte led num\n 
      * - sends OK and LED SETTING if led num is valid\n 
      * - sends ERROR and pads second byte if led num is invalid\n 
      * */

    // wait for led_num
    while (QueueIsEmpty(SpiFifo));
    
    // read led_num
    uint8_t led_num = QueuePop(SpiFifo);

    if ((led_num == 0) || (led_num == 1)) // led_num is valid
    {
        // send OK and LED_SETTING
        SpiSlaveTxByte(OK);
        SpiSlaveTxByte(ReadLedState(led_num));
    }
    else // led_num is invalid
    {
        // send ERROR and pad second byte
        SpiSlaveTxByte(ERROR);
        SpiSlaveTxByte(PADDING);
    }
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
#define LisWriteConfig LisWriteConfig_fake
#endif
inline void SetSensorConfig(void)
{
    /** SetSensorConfig behavior:\n 
      * - receives three bytes of config from Bridge\n 
      * - does not update config globals if config is invalid\n 
      * - replies ERROR if binning is invalid\n 
      * - replies ERROR if gain is invalid\n 
      * - replies ERROR if active rows is invalid\n 
      * - does not program LIS 770i if config is invalid\n 
      * - writes valid config to LIS 770i programmable setup register\n 
      * - replies OK if all config values are valid\n 
      * - the OK is sent after LIS is programmed with new config\n 
      * */
    // Get config values
    while (QueueIsEmpty(SpiFifo)); // 5 cycles
    uint8_t new_binning = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo)); // 5 cycles
    uint8_t new_gain = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo)); // 5 cycles
    uint8_t new_active_rows = QueuePop(SpiFifo);
    if ( !LisConfigIsValid(new_binning, new_gain, new_active_rows) )
    {
        // Reply with error if any config value is invalid
        SpiSlaveTxByte(ERROR);
        return;
    }
    // New config is valid, update global config
    binning     = new_binning;
    gain        = new_gain;
    active_rows = new_active_rows;
    // Program the LIS-770i
    LisWriteConfig();
    // Report OK to SPI Master to indicate programming is done
    SpiSlaveTxByte(OK);
}
#ifdef USE_FAKES
#undef SpiSlaveTxByte
#undef LisWriteConfig
#endif

#endif // _VISCMD_H
