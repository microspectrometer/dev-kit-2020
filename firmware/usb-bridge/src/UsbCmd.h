// ---API (Go to the Doxygen documentation of this file)---
/** \file UsbCmd.h
 * # API
 * void NullCommand(void);\n 
 * */
#ifndef _USBCMD_H
#define _USBCMD_H
#include "StatusCode.h"
#include "ReadWriteBits.h"
#include "BiColorLed.h"
#include "Usb.h"

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

    //! LED 0, the status_led, is the only Bridge indicator LED.
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

#endif // _USBCMD_H
