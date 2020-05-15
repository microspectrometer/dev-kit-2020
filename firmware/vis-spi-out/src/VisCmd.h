// ---API (Go to the Doxygen documentation of this file)---
/** \file VisCmd.h
 * # API for vis-spi-out.c
 * - void NullCommand(void)
 * - void GetSensorLED(void)
 * - void SetSensorLED(void)
 * - void ReplyCommandInvalid(void)
 * - void GetSensorConfig(void)
 * - void SetSensorConfig(void)
 * - void GetExposure(void)
 * - void SetExposure(void)
 * - void AutoExposure(void)
 * - void GetFrame(void)
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
inline bool LedNumIsValid(bicolorled_num led_num) // -> is_valid
{
    /** LedNumIsValid behavior:\n 
      * - returns TRUE if led num is 0\n 
      * - returns TRUE if led num is 1\n 
      * - returns FALSE if led num is not 0 or 1\n 
      * */

    return ( (led_num == 0) || (led_num == 1) );
}

/*inline bool LedSettingIsValid(led_state led_setting) // -> is_valid */
/*{ */
/*    /1** LedSettingIsValid behavior:\n */ 
/*      * - returns TRUE if led setting is OFF\n */ 
/*      * - returns TRUE if led setting is GREEN\n */ 
/*      * - returns TRUE if led setting is RED\n */ 
/*      * - returns FALSE if led setting is not 0 1 or 2\n */ 
/*      * *1/ */

/*    return ( */
/*        (led_setting == OFF)   || */
/*        (led_setting == GREEN) || */
/*        (led_setting == RED) */
/*        ); */
/*} */

inline uint8_t ReadLedState(bicolorled_num led_num) // -> led_state
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

/* ---------------------- */
/* | ---Commands :( --- | */ // (these NEED proper unit tests)
/* ---------------------- */
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

    if (LedNumIsValid(led_num))
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

inline void SetSensorLED(void)
{
    /** SetSensorLED behavior:\n 
      * - waits for byte led num\n 
      * - reads byte led num\n 
      * - waits for byte led setting\n 
      * - reads byte led setting\n 
      * - sends ERROR if led num is invalid\n 
      * - sends ERROR if led setting is invalid\n 
      * - applies LED setting if valid\n 
      * - sends OK if num and setting are valid\n 
      * */

    // wait for led_num
    while (QueueIsEmpty(SpiFifo));

    // read led_num
    uint8_t led_num = QueuePop(SpiFifo);

    // wait for led_setting
    while (QueueIsEmpty(SpiFifo));

    // read led_setting
    uint8_t led_setting = QueuePop(SpiFifo);

    if (!LedNumIsValid(led_num)) // led_num is invalid
    {
        // send ERROR
        SpiSlaveTxByte(ERROR);
    }
    else if (!led_setting_is_valid(led_setting)) // led_setting is invalid
    {
        // send ERROR
        SpiSlaveTxByte(ERROR);
    }
    else // led_num and led_setting are valid
    {
        // lookup led_num
        uint8_t led;
        switch (led_num)
        {
            case 0: led = led_0; break;
            case 1: led = led_1; break;
        }

        // apply led_setting
        switch (led_setting)
        {
            case 0: BiColorLedOff(led); break;
            case 1: BiColorLedOn(led); BiColorLedGreen(led); break;
            case 2: BiColorLedOn(led); BiColorLedRed(led); break;
        }

        // send OK
        SpiSlaveTxByte(OK);
    }
}

inline void GetSensorConfig(void)
{
    /** GetSensorConfig behavior:\n 
      * - sends OK\n 
      * - sends binning\n 
      * - sends gain\n 
      * - sends row bitmap\n 
      * */

    SpiSlaveTxByte(OK);
    SpiSlaveTxByte(binning);
    SpiSlaveTxByte(gain);
    SpiSlaveTxByte(active_rows);
}

/* ---------------------- */
/* | ---Commands :) --- | */ // (these HAVE proper unit tests)
/* ---------------------- */

#ifdef USE_FAKES
#include "SpiSlave_faked.h" // declare fakes
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

/* ------------------------------------ */
/* | ---When command is unknown...--- | */
/* ------------------------------------ */

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

#endif // _VISCMD_H
