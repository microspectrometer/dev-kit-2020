// ---API (Go to the Doxygen documentation of this file)---
/** \file UsbCmd.h
 * # API for usb-bridge.c
 * - NullCommand()
 * - GetBridgeLED()
 * - SetBridgeLED()
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

    // write response from sensor
    UsbWriteByte(status);
    UsbWriteByte(binning);
    UsbWriteByte(gain);
    UsbWriteByte(row_bitmap);
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
inline void GetExposure(void)
{
    /** GetExposure behavior:\n 
      * - sends command to sensor\n 
      * - writes OK to indicate it sent the command to the sensor\n 
      * - waits for sensor to signal STATUS data ready\n 
      * - reads status from sensor\n 
      * - waits for sensor to signal EXPOSURE MSB data ready\n 
      * - reads exposure MSB from sensor\n 
      * - waits for sensor to signal EXPOSURE LSB data ready\n 
      * - reads exposure LSB from sensor\n 
      * - writes sensor status\n 
      * - writes exposure MSB\n 
      * - writes exposure LSB\n 
      * */

    uint8_t const cmd = 9; // command is GetExposure

    // send command to sensor
    SpiMasterXfrByte(cmd);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send EXPOSURE_MSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read exposure_msb
    uint8_t exposure_msb = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send EXPOSURE_LSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read exposure_lsb
    uint8_t exposure_lsb = SpiMasterXfrByte(PADDING);

    // write response from sensor
    UsbWriteByte(status);
    UsbWriteByte(exposure_msb);
    UsbWriteByte(exposure_lsb);
}
inline void SetExposure(void)
{
    /** SetExposure behavior:\n 
      * - waits for byte exposure_MSB\n 
      * - reads byte exposure_MSB\n 
      * - waits for byte exposure_LSB\n 
      * - reads byte exposure_LSB\n 
      * - sends command to sensor\n 
      * - sends exposure_MSB to sensor\n 
      * - sends exposure_LSB to sensor\n 
      * - writes OK to indicate it sent the command to the sensor\n 
      * - waits for sensor to signal STATUS data ready\n 
      * - reads status from sensor\n 
      * - writes sensor status\n 
      * */

    uint8_t const cmd = 10; // command is SetExposure

    // loop until exposure_MSB received
    while (UsbRxbufferIsEmpty());
    
    // read exposure_MSB
    uint8_t exposure_MSB = 0xFF;
    UsbReadByte(&exposure_MSB);

    // loop until exposure_LSB received
    while (UsbRxbufferIsEmpty());
    
    // read exposure_LSB
    uint8_t exposure_LSB = 0xFF;
    UsbReadByte(&exposure_LSB);

    // send command to sensor
    SpiMasterXfrByte(cmd);
    SpiMasterXfrByte(exposure_MSB);
    SpiMasterXfrByte(exposure_LSB);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // write response
    UsbWriteByte(status);
}
inline void CaptureFrame(void)
{
    /** CaptureFrame behavior:\n 
      * - sends command to sensor\n 
      * - writes OK to indicate it sent the command to the sensor\n 
      * - waits for sensor to signal STATUS data ready\n 
      * - reads status from sensor\n 
      * - waits for sensor to signal NUM_PIXELS_MSB data ready\n 
      * - reads num_pixels_MSB from sensor\n 
      * - waits for sensor to signal NUM_PIXELS_LSB data ready\n 
      * - reads num_pixels_LSB from sensor\n 
      * - writes sensor status\n 
      * - writes num_pixels_MSB\n 
      * - writes num_pixels_LSB\n 
      * - returns if status is not OK\n 
      * - waits for the next byte of frame data\n 
      * - writes the next byte of frame data\n 
      * - loops wait for byte then write byte for a total nbytes of 2x num_pixels\n 
      * */

    uint8_t const cmd = 11; // command is CaptureFrame

    // send command to sensor
    SpiMasterXfrByte(cmd);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send num_pixels_MSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read num_pixels_MSB
    uint8_t num_pixels_MSB = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send num_pixels_LSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read num_pixels_LSB
    uint8_t num_pixels_LSB = SpiMasterXfrByte(PADDING);

    // write first three bytes of response from sensor
    UsbWriteByte(status);
    UsbWriteByte(num_pixels_MSB);
    UsbWriteByte(num_pixels_LSB);

    if (status == OK)
    {
        /* ------------------ */
        /* | READ THE FRAME | */
        /* ------------------ */

        uint16_t num_pixels = (num_pixels_MSB << 8) | num_pixels_LSB;
        uint16_t nbytes = 2*num_pixels;
        uint16_t nbytes_read = 0;
        while(nbytes_read++ < nbytes)
        {
            // wait for data ready LOW: sensor ready to send next byte
            while( BitIsSet(Spi_PortInput, Spi_DataReady));
            // read next byte of frame data and write to USB host
            UsbWriteByte( SpiMasterXfrByte(PADDING) );
        }
    }

}
inline void AutoExposure(void)
{
    uint8_t const cmd = 12; // command is GetAutoExposeConfig

    // indicate BUSY
    BiColorLedRed(status_led);

    // send command to sensor
    SpiMasterXfrByte(cmd);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send success
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read success true/false
    uint8_t success = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send iterations
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read number of iterations to hit AutoExpose target
    uint8_t iterations = SpiMasterXfrByte(PADDING);

    // write response from sensor
    UsbWriteByte(status);
    UsbWriteByte(success);
    UsbWriteByte(iterations);

    // indicate DONE
    BiColorLedGreen(status_led);
}
inline void GetAutoExposeConfig(void)
{
    uint8_t const cmd = 13; // command is GetAutoExposeConfig

    // send command to sensor
    SpiMasterXfrByte(cmd);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send MAX_TRIES
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read max_tries
    uint8_t max_tries = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send START_PIXEL_MSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read start_pixel_msb
    uint8_t start_pixel_msb = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send START_PIXEL_LSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read start_pixel_lsb
    uint8_t start_pixel_lsb = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send STOP_PIXEL_MSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read stop_pixel_msb
    uint8_t stop_pixel_msb = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send STOP_PIXEL_LSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read stop_pixel_lsb
    uint8_t stop_pixel_lsb = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send TARGET_MSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read target_msb
    uint8_t target_msb = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send TARGET_LSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read target_lsb
    uint8_t target_lsb = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send TARGET_TOLERANCE_MSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read target_tolerance_msb
    uint8_t target_tolerance_msb = SpiMasterXfrByte(PADDING);

    // wait for data ready LOW: sensor ready to send TARGET_LSB
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read target_tolerance_lsb
    uint8_t target_tolerance_lsb = SpiMasterXfrByte(PADDING);

    // write response from sensor
    UsbWriteByte(status);
    // default: 10 = 0x0a
    UsbWriteByte(max_tries);
    // default: 7 = 0x0007
    UsbWriteByte(start_pixel_msb); // 0
    UsbWriteByte(start_pixel_lsb); // 7
    // default: 392 = 0x0188
    UsbWriteByte(stop_pixel_msb); // 0x01 -- 1 (*256)
    UsbWriteByte(stop_pixel_lsb); // 0x88 -- 136
    // default: 46420 = 0xb554
    UsbWriteByte(target_msb); // 0xb5 -- 181 (*256)
    UsbWriteByte(target_lsb); // 0x54 -- 84
    // default: 3277 = 0x0ccd
    UsbWriteByte(target_tolerance_msb); // 0x0c -- 12 (*256)
    UsbWriteByte(target_tolerance_lsb); // 0xcd -- 205
}
inline void SetAutoExposeConfig(void)
{

    // indicate BUSY
    BiColorLedRed(status_led);
    uint8_t const cmd = 14; // command is SetAutoExposeConfig

    // loop until max_tries is received
    while (UsbRxbufferIsEmpty());
    // read max_tries
    uint8_t max_tries = 0xFF;
    UsbReadByte(&max_tries);

    // loop until start_pixel_msb is received
    while (UsbRxbufferIsEmpty());
    // read start_pixel_msb
    uint8_t start_pixel_msb = 0xFF;
    UsbReadByte(&start_pixel_msb);

    // loop until start_pixel_lsb is received
    while (UsbRxbufferIsEmpty());
    // read start_pixel_lsb
    uint8_t start_pixel_lsb = 0xFF;
    UsbReadByte(&start_pixel_lsb);

    // loop until stop_pixel_msb is received
    while (UsbRxbufferIsEmpty());
    // read stop_pixel_msb
    uint8_t stop_pixel_msb = 0xFF;
    UsbReadByte(&stop_pixel_msb);

    // loop until stop_pixel_lsb is received
    while (UsbRxbufferIsEmpty());
    // read stop_pixel_lsb
    uint8_t stop_pixel_lsb = 0xFF;
    UsbReadByte(&stop_pixel_lsb);

    // loop until target_msb is received
    while (UsbRxbufferIsEmpty());
    // read target_msb
    uint8_t target_msb = 0xFF;
    UsbReadByte(&target_msb);

    // loop until target_lsb is received
    while (UsbRxbufferIsEmpty());
    // read target_lsb
    uint8_t target_lsb = 0xFF;
    UsbReadByte(&target_lsb);

    // loop until target_tolerance_msb is received
    while (UsbRxbufferIsEmpty());
    // read target_tolerance_msb
    uint8_t target_tolerance_msb = 0xFF;
    UsbReadByte(&target_tolerance_msb);

    // loop until target_tolerance_lsb is received
    while (UsbRxbufferIsEmpty());
    // read target_tolerance_lsb
    uint8_t target_tolerance_lsb = 0xFF;
    UsbReadByte(&target_tolerance_lsb);

    // send command to sensor
    SpiMasterXfrByte(cmd);
    SpiMasterXfrByte(max_tries);
    SpiMasterXfrByte(start_pixel_msb);
    SpiMasterXfrByte(start_pixel_lsb);
    SpiMasterXfrByte(stop_pixel_msb);
    SpiMasterXfrByte(stop_pixel_lsb);
    SpiMasterXfrByte(target_msb);
    SpiMasterXfrByte(target_lsb);
    SpiMasterXfrByte(target_tolerance_msb);
    SpiMasterXfrByte(target_tolerance_lsb);

    // write OK to indicate command sent to sensor
    UsbWriteByte(OK);

    // wait for data ready LOW: sensor ready to send STATUS
    while( BitIsSet(Spi_PortInput, Spi_DataReady));
    // read status
    uint8_t status = SpiMasterXfrByte(PADDING);

    // write response
    UsbWriteByte(status);

    // indicate DONE
    BiColorLedGreen(status_led);
}

#endif // _USBCMD_H
