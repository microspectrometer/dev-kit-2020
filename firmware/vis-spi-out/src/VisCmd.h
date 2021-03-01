// ---API (Go to the Doxygen documentation of this file)---
/** \file VisCmd.h
 * # API for vis-spi-out.c
 * - NullCommand()
 * - GetSensorLED()
 * - SetSensorLED()
 * - GetSensorConfig()
 * - SetSensorConfig()
 * - GetExposure()
 * - SetExposure()
 * - CaptureFrame()
 * - AutoExposure()
 * - GetAutoExposeConfig()
 * - SetAutoExposeConfig()
 * */
#ifndef _VISCMD_H
#define _VISCMD_H
#include <stdint.h>
#include <stdbool.h>
#include "SpiSlave.h"
#include "StatusCode.h"
#include "Lis.h"
#include "LisConfig.h"
#include "Queue.h"
#include "BiColorLed.h"
#include "UartSpi.h"
#include "AutoExpose.h"

/* ------------------------------------------------------- */
/* | Only use these headers when building for AVR target | */
/* ------------------------------------------------------- */
#ifndef USE_FAKES
#include <util/delay_basic.h> // use _delay_loop_1 to count µs
#endif

extern volatile Queue_s * SpiFifo; // definition in translation unit using SpiFifo

//! One frame of pixel data is, at most, 1568 bytes.
extern uint8_t frame[];

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

inline bool AutoExposeConfigIsValid(
        uint8_t new_max_tries,
        uint16_t new_start_pixel,
        uint16_t new_stop_pixel,
        uint16_t new_target
        )
{
    // Check max_tries is valid
    if (new_max_tries == 0) return false;

    // Check target is valid
    if (new_target < 4500) return false;

    // Check stop_pixel is not less than start_pixel
    if (new_stop_pixel < new_start_pixel) return false;

    // Check start_pixel and stop_pixel are in range
    if (binning == BINNING_ON)
    {
        if ((new_start_pixel < 7) || (new_start_pixel > 392)) return false;
        if ((new_stop_pixel < 7) || (new_stop_pixel > 392)) return false;
    }
    else // (binning == BINNING_OFF)
    {
        if ((new_start_pixel < 14) || (new_start_pixel > 784)) return false;
        if ((new_stop_pixel < 14) || (new_stop_pixel > 784)) return false;
    }

    // Config is valid
    return true;
}


// "static" because it calls static _delay_loop_1
static inline void LisReadout(uint16_t num_pixels)
{
    /** LisReadout behavior:\n 
      * - waits for Lis_Sync to go HIGH then go LOW\n 
      * - reads one pixel on each rising edge of Lis_Clk\n 
      * - LOOP: wait for the rising edge of Lis_Clk\n 
      * - LOOP: start the ADC conversion\n 
      * - LOOP: wait for 45 cycles of 10MHz clock\n 
      * - LOOP: start ADC readout\n 
      * - LOOP: wait for most significant byte of ADC readout\n 
      * - LOOP: save MSB to frame buffer\n 
      * - LOOP: wait for least significant byte of ADC readout\n 
      * - LOOP: save LSB to frame buffer\n 
      * */

    // wait for SYNC pulse to signify readout starts
    while( BitIsClear(Lis_pin1, Lis_Sync) ); // wait for SYNC HIGH
    while(   BitIsSet(Lis_pin1, Lis_Sync) ); // wait for SYNC LOW

    /* --------------------------------------------------------- */
    /* | LOOP: read one pixel on each rising edge of Lis clock | */
    /* --------------------------------------------------------- */
    uint16_t pixel_count = 0;
    uint8_t *pframe = frame;
    while( pixel_count++ < num_pixels)
    {
        // wait for rising edge of Lis clock
        _WaitForLisClkHigh();

        // start ADC conversion
        StartAdcConversion();

        /* --------------------------------------------------- */
        /* | wait at least 4.66µs for conversion to complete | */
        /* --------------------------------------------------- */
        // use _delay_loop_1 to count 45 ticks of the 10MHz osc
        // each loop iteration is 3 ticks
#ifndef USE_FAKES
        _delay_loop_1(15); // 15 * 3 = 45 -> 4.5µs plus overhead
#endif

        // start 16-bit ADC readout
        StartAdcReadout();

        // wait for MSB of ADC readout
        while (BitIsClear(UartSpi_UCSR0A, UartSpi_RXC0));

        // save MSB to frame buffer
        *(pframe++) = *UartSpi_UDR0;

        // wait for LSB of ADC readout
        while (BitIsClear(UartSpi_UCSR0A, UartSpi_RXC0));

        // save LSB to frame buffer
        *(pframe++) = *UartSpi_UDR0;
    }
}

uint16_t GetPeak(uint16_t const, uint16_t const);
uint16_t AutoExpose(void);

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

inline void GetExposure(void)
{
    /** GetExposure behavior:\n 
      * - sends OK\n 
      * - sends MSB of exposure ticks\n 
      * - sends LSB of exposure ticks\n 
      * */

    SpiSlaveTxByte(OK);
    SpiSlaveTxByte(MSB(exposure_ticks));
    SpiSlaveTxByte(LSB(exposure_ticks));
}

inline void SetExposure(void)
{
    /** SetExposure behavior:\n 
      * - waits for byte exposure MSB\n 
      * - reads byte exposure MSB\n 
      * - waits for byte exposure LSB\n 
      * - reads byte exposure LSB\n 
      * - updates global exposure ticks\n 
      * - sends OK\n 
      * */

    // wait for exposure_MSB
    while (QueueIsEmpty(SpiFifo));

    // read exposure_MSB
    uint8_t exposure_MSB = QueuePop(SpiFifo);

    // wait for exposure_LSB
    while (QueueIsEmpty(SpiFifo));

    // read exposure_LSB
    uint8_t exposure_LSB = QueuePop(SpiFifo);

    // update global exposure_ticks
    exposure_ticks = (exposure_MSB << 8) | exposure_LSB;

    // send OK
    SpiSlaveTxByte(OK);
}

// "static" because it calls static LisReadout
static inline void CaptureFrame(void)
{
    /** CaptureFrame behavior:\n 
      * - sends OK\n 
      * - checks binning to determine number of pixels in frame\n 
      * - sends num pixels MSB\n 
      * - sends num pixels LSB\n 
      * - exposes the pixels\n 
      * - does readout of num pixels into the frame buffer\n 
      * - sends the pixel readings stored in the frame buffer\n 
      * */

    // send OK
    SpiSlaveTxByte(OK);

    // determine number of pixels
    uint16_t num_pixels;
    if (binning == BINNING_OFF) num_pixels = MAX_NUM_PIXELS;
    else num_pixels = MAX_NUM_PIXELS/2;

    // send num_pixels
    SpiSlaveTxByte(MSB(num_pixels));
    SpiSlaveTxByte(LSB(num_pixels));

    /* -------------- */
    /* | READ FRAME | */
    /* -------------- */

    // expose the LIS-770i pixels
    LisExpose();

    // readout the LIS-770i pixels into global frame buffer
    LisReadout(num_pixels);

    /* --------------- */
    /* | WRITE FRAME | */
    /* --------------- */

    // disable SPI interrupt while writing the frame
    DisableSpiInterrupt();

    // start at MSB of first pixel
    uint8_t *pframe = frame;

    uint16_t byte_count = 0;
    uint16_t nbytes = 2*num_pixels;
    while( byte_count++ < nbytes )
    {
        // load the SPI data register with the next byte
        *Spi_SPDR = *(pframe++);

        _SignalDataReady();

        // Wait for a byte from the SPI Master.
        while ( !_SpiTransferIsDone() ); // Check SPI interrupt flag

        _SignalDataNotReady();
    }

    // Re-enable interrupt and reset (clear) SPI interrupt flag
    EnableSpiInterrupt();
}

inline void AutoExposure(void)
{

    // auto-expose, report result:
    uint16_t result = AutoExpose();

    // send OK
    SpiSlaveTxByte(OK);

    // send final_peak
    SpiSlaveTxByte(MSB(result)); // success true/false
    SpiSlaveTxByte(LSB(result)); // iterations to hit target
}

inline void GetAutoExposeConfig(void)
{
    SpiSlaveTxByte(OK);
    SpiSlaveTxByte(max_tries);
    SpiSlaveTxByte(MSB(start_pixel));
    SpiSlaveTxByte(LSB(start_pixel));
    SpiSlaveTxByte(MSB(stop_pixel));
    SpiSlaveTxByte(LSB(stop_pixel));
    SpiSlaveTxByte(MSB(target));
    SpiSlaveTxByte(LSB(target));
    SpiSlaveTxByte(MSB(target_tolerance));
    SpiSlaveTxByte(LSB(target_tolerance));
    SpiSlaveTxByte(MSB(max_exposure));
    SpiSlaveTxByte(LSB(max_exposure));
}

inline void SetAutoExposeConfig(void)
{

    /* ------------------------------------------- */
    /* | Get 6 config values, a total of 11 bytes | */
    /* ------------------------------------------- */

    // get max_tries
    while (QueueIsEmpty(SpiFifo));
    uint8_t new_max_tries = QueuePop(SpiFifo);

    // remaining 5 config values are each 16 bits, sent MSB first
    uint8_t msb; uint8_t lsb;

    // get start pixel
    while (QueueIsEmpty(SpiFifo));
    msb = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo));
    lsb = QueuePop(SpiFifo);
    uint16_t new_start_pixel = (uint16_t)(msb << 8) | lsb;

    // get stop pixel
    while (QueueIsEmpty(SpiFifo));
    msb = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo));
    lsb = QueuePop(SpiFifo);
    uint16_t new_stop_pixel = (uint16_t)(msb << 8) | lsb;

    // get target
    while (QueueIsEmpty(SpiFifo));
    msb = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo));
    lsb = QueuePop(SpiFifo);
    uint16_t new_target = (uint16_t)(msb << 8) | lsb;

    // get target_tolerance
    while (QueueIsEmpty(SpiFifo));
    msb = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo));
    lsb = QueuePop(SpiFifo);
    uint16_t new_target_tolerance = (uint16_t)(msb << 8) | lsb;

    // get max_exposure
    while (QueueIsEmpty(SpiFifo));
    msb = QueuePop(SpiFifo);
    while (QueueIsEmpty(SpiFifo));
    lsb = QueuePop(SpiFifo);
    uint16_t new_max_exposure = (uint16_t)(msb << 8) | lsb;

    /* ----------------------- */
    /* | Validate New Config | */
    /* ----------------------- */

    /* TODO: add check that new_max_exposure is valid */
    if ( !AutoExposeConfigIsValid(
            new_max_tries,
            new_start_pixel, new_stop_pixel,
            new_target
            ) ) // any new_target_tolerance is valid
    { // at least one value in new config is invalid
        // reply with ERROR
        SpiSlaveTxByte(ERROR);
    }
    else // new config is valid
    {
        // update global config
        max_tries = new_max_tries;
        start_pixel = new_start_pixel;
        stop_pixel = new_stop_pixel;
        target = new_target;
        target_tolerance = new_target_tolerance;
        max_exposure = new_max_exposure;

        // reply with OK
        SpiSlaveTxByte(OK);
    }
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

#endif // _VISCMD_H
