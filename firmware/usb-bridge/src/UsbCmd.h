// ---API (Go to the Doxygen documentation of this file)---
/** \file UsbCmd.h
 * # API for usb-bridge.c
 * - void NullCommand(void)
 * - void GetBridgeLED(void)
 * - void SetBridgeLED(void)
 * - void GetSensorLED(void)
 * - void SetSensorLED(void)
 * - void GetSensorConfig(void)
 * - void SetSensorConfig(void)
 * - void GetExposure(void)
 * - void SetExposure(void)
 * - void AutoExposure(void)
 * - void GetFrame(void)
 * */
#ifndef _USBCMD_H
#define _USBCMD_H
#include "StatusCode.h"
#include "ReadWriteBits.h"
#include "BiColorLed.h"
#include "Usb.h"
#include "SpiMaster.h"

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
inline uint8_t ReadLedState(void) // -> led_state
{
    /** ReadLedState behavior:\n 
      * - returns OFF if LED is off\n 
      * - returns GREEN if LED is on and green\n 
      * - returns RED if LED is on and red\n 
      * - see led_state in StatusCodes.h
      * */

    //! *LED 0,* the **status_led**, *is the only indicator LED on the usb-bridge.*
    if (BitIsClear(BiColorLed_ddr, status_led))
        return OFF;
    else // LED is on
        return BitIsClear(BiColorLed_port, status_led)
            ? GREEN : RED;
}

/* ------------------ */
/* | ---Commands--- | */
/* ------------------ */
inline void NullCommand(void)
{ //! Do nothing.
}
inline void GetBridgeLED(void)
{
    /** GetBridgeLED behavior:\n 
      * - waits for byte led num\n 
      * - reads byte led num\n 
      * - writes two bytes to USB host\n 
      * - writes OK and LED STATE if led num is valid\n 
      * - writes ERROR and pads second byte if led num is invalid\n 
      * */
    // loop until led_num received
    while (UsbRxbufferIsEmpty());

    // read led_num
    uint8_t led_num = 0xFF;
    UsbReadByte(&led_num);

    if (led_num == 0) // led_num is valid
    {
        // write OK and LED STATE
        UsbWriteByte(OK);
        UsbWriteByte(ReadLedState());
    }
    else // led_num is invalid
    {
        // write ERROR and pad second byte
        UsbWriteByte(ERROR);
        UsbWriteByte(PADDING);
    }
}
inline void SetBridgeLED(void)
{
    /** SetBridgeLED behavior:\n 
      * - waits for byte led num\n 
      * - reads byte led num\n 
      * - waits for byte led setting\n 
      * - reads byte led setting\n 
      * - applies led setting and writes OK if parameters are valid\n 
      * - writes ERROR if led num is invalid\n 
      * - writes ERROR if led setting is invalid\n 
      * */
    // loop until led_num received
    while (UsbRxbufferIsEmpty());

    // read led_num
    uint8_t led_num = 0xFF;
    UsbReadByte(&led_num);

    // loop until led_setting received
    while (UsbRxbufferIsEmpty());

    // read led_setting
    uint8_t led_setting = 0xFF;
    UsbReadByte(&led_setting);

    if ( (led_num == 0) && led_setting_is_valid(led_setting) ) // parameters are valid
    {
        // apply led_setting
        switch (led_setting)
        {
            case 0: BiColorLedOff(status_led); break;
            case 1: BiColorLedOn(status_led); BiColorLedGreen(status_led); break;
            case 2: BiColorLedOn(status_led); BiColorLedRed(status_led); break;
        }
        // write OK
        UsbWriteByte(OK);
    }
    else // at least one parameter is invalid
    {
        UsbWriteByte(ERROR);
    }
}
inline void GetSensorLED(void)
{
    /** Forward 2-byte indicator-LED command to sensor.\n
     * Respond with three bytes:
     * - usb-bridge **status**
     *   - send `OK` after sending all bytes to sensor
     * - vis-spi-out **status**
     * - **led_setting**
     */
    /** GetSensorLED behavior:\n 
      * - waits for byte led num\n 
      * - reads byte led num\n 
      * - sends command to sensor\n 
      * - sends led num to sensor\n 
      * - writes OK to indicate it sent the command to the sensor\n 
      * - waits for sensor to signal STATUS data ready\n 
      * - reads status from sensor\n 
      * - waits for sensor to signal LED SETTING data ready\n 
      * - reads led setting from sensor\n 
      * - writes sensor status\n 
      * - writes sensor led setting\n 
      * */
    uint8_t const cmd = 3; // command is GetSensorLED

    // loop until led_num received
    while (UsbRxbufferIsEmpty());

    // read led_num
    uint8_t led_num = 0xFF;
    UsbReadByte(&led_num);

    // send command to sensor
    SpiMasterXfrByte(cmd);
    SpiMasterXfrByte(led_num);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);
    // wait for data ready LOW: sensor ready to send LED_SETTING
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read led_setting
    uint8_t led_setting = SpiMasterXfrByte(PADDING);

    // write response
    UsbWriteByte(status);
    UsbWriteByte(led_setting);
}
inline void SetSensorLED(void)
{
    /** Forward 3-byte indicator-LED command to sensor.\n
     * Respond with two bytes:
     * - usb-bridge **status**
     *   - send `OK` after sending all bytes to sensor
     * - vis-spi-out **status**
     */
    /** SetSensorLED behavior:\n 
      * - waits for byte led num\n 
      * - reads byte led num\n 
      * - waits for byte led setting\n 
      * - reads byte led setting\n 
      * - sends command to sensor\n 
      * - sends led num to sensor\n 
      * - sends led setting to sensor\n 
      * - writes OK to indicate it sent the command to the sensor\n 
      * - waits for sensor to signal STATUS data ready\n 
      * - reads status from sensor\n 
      * - writes sensor status\n 
      * */

    uint8_t const cmd = 4; // command is SetSensorLED

    // loop until led_num received
    while (UsbRxbufferIsEmpty());

    // read led_num
    uint8_t led_num = 0xFF;
    UsbReadByte(&led_num);

    // loop until led_setting received
    while (UsbRxbufferIsEmpty());

    // read led_setting
    uint8_t led_setting = 0xFF;
    UsbReadByte(&led_setting);

    // send command to sensor
    SpiMasterXfrByte(cmd);
    SpiMasterXfrByte(led_num);
    SpiMasterXfrByte(led_setting);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // write response from sensor
    UsbWriteByte(status);
}
inline void SetSensorConfig(void)
{
    /** Forward 4-byte sensor config command to sensor.\n
     * Respond with two bytes:
     * - usb-bridge **status**
     *   - send `OK` after sending all bytes to sensor
     * - vis-spi-out **status**
     */
    /** SetSensorConfig behavior:\n 
      * - waits for byte binning\n 
      * - reads byte binning\n 
      * - waits for byte gain\n 
      * - reads byte gain\n 
      * - waits for byte row_bitmap\n 
      * - reads byte row_bitmap\n 
      * - sends command to sensor\n 
      * - sends binning to sensor\n 
      * - sends gain to sensor\n 
      * - sends row_bitmap to sensor\n 
      * - writes OK to indicate it sent the command to the sensor\n 
      * - waits for sensor to signal STATUS data ready\n 
      * - reads status from sensor\n 
      * - writes sensor status\n 
      * */

    uint8_t const cmd = 8; // command is SetSensorConfig

    // loop until binning received
    while (UsbRxbufferIsEmpty());

    // read binning
    uint8_t binning = 0xFF;
    UsbReadByte(&binning);

    // loop until gain received
    while (UsbRxbufferIsEmpty());

    // read gain
    uint8_t gain = 0xFF;
    UsbReadByte(&gain);

    // loop until row_bitmap received
    while (UsbRxbufferIsEmpty());

    // read row_bitmap
    uint8_t row_bitmap = 0xFF;
    UsbReadByte(&row_bitmap);

    // send command to sensor
    SpiMasterXfrByte(cmd);
    SpiMasterXfrByte(binning);
    SpiMasterXfrByte(gain);
    SpiMasterXfrByte(row_bitmap);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // write response
    UsbWriteByte(status);
}
inline void GetSensorConfig(void)
{
    /** Forward 1-byte sensor config command to sensor.\n
     * Respond with five bytes:
     * - usb-bridge **status**
     *   - send `OK` after sending all bytes to sensor
     * - vis-spi-out **status**
     * - **binning**
     * - **gain**
     * - **row_bitmap**
     */
    /** GetSensorConfig behavior:\n 
      * - sends command to sensor\n 
      * - writes OK to indicate it sent the command to the sensor\n 
      * - waits for sensor to signal STATUS data ready\n 
      * - reads status from sensor\n 
      * - waits for sensor to signal BINNING data ready\n 
      * - reads binning from sensor\n 
      * - waits for sensor to signal GAIN data ready\n 
      * - reads gain from sensor\n 
      * - waits for sensor to signal ROW_BITMAP data ready\n 
      * - reads row_bitmap from sensor\n 
      * - writes sensor status\n 
      * - writes sensor binning\n 
      * - writes sensor gain\n 
      * - writes sensor row_bitmap\n 
      * */

    uint8_t const cmd = 7; // command is GetSensorConfig

    // send command to sensor
    SpiMasterXfrByte(cmd);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send BINNING
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read binning
    uint8_t binning = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send GAIN
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read gain
    uint8_t gain = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send ROW_BITMAP
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read row_bitmap
    uint8_t row_bitmap = SpiMasterXfrByte(PADDING);

    (void)status;
    (void)binning;
    (void)gain;
    (void)row_bitmap;

    // write response from sensor
    UsbWriteByte(status);
    UsbWriteByte(binning);
    UsbWriteByte(gain);
    UsbWriteByte(row_bitmap);
}

#endif // _USBCMD_H
