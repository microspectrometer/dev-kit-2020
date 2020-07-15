/** \file
 * *See `cfg/microspec.json` in the Python API repository.*
 * - Section `"global"` defines the **protocol byte codes**.
 *   - Firmware header file StatusCodes.h duplicates these `"global"`
 *   definitions.
 * - Section `"protocol":"command"` defines the commands and
 *   command parameters **sent by the USB host.**
 * - Section `"protocol":"serial"` defines the **expected
 *   response** from the **USB board** `usb-bridge`.
 * - Section `"protocol":"sensor"` defines the **expected
 *   response** from the **sensor board** `vis-spi-out`.
 * .
 * **STATUS CODES**
 * - JSON section `"globals"` defines the **status codes**: `OK`,
 *   `ERROR`, `INVALID_CMD`
 * - the **status code** is **byte 0** of the *response* from the
 *   **USB board** or **sensor board**
 * - the **USB board** and the **sensor board** reply with the
 *   same status codes
 * - the **status code** describes the status of the
 *   communication *from the point of view of the sender*:
 *   - `OK`: the sender understood the message it just received
 *   *and* it successfully performed the task
 *   - `ERROR`: the sender understood the message but could not
 *   perform the task
 *   - `INVALID_CMD`: the sender received an unrecognized command
 * .
 * **INDICATOR LED STATES**
 * - Section `"globals"` defines the **LED states**: `OFF`, `GREEN`, or `RED`
 * - **LED state** is:
 *   - **byte 2** of the command to `set` the indicator LED
 *   - **byte 1** of the response to the command to `get` the indicator LED
 * .
 * */
#ifndef _STATUSCODE_H
#define _STATUSCODE_H
#include "stdint.h"
#include "stdbool.h"

//! Status codes are type `status_code`.
typedef uint8_t const status_code;

extern status_code OK;
extern status_code ERROR;
extern status_code INVALID_CMD;

//! Indicator LEDs have three possible states: `OFF`, `GREEN`, and `RED`.
typedef uint8_t const led_state;
extern led_state OFF;
extern led_state GREEN;
extern led_state RED;

inline bool led_setting_is_valid(led_state setting)
{
    /** led_setting_is_valid behavior:\n 
      * - returns TRUE if setting is OFF\n 
      * - returns TRUE if setting is GREEN\n 
      * - returns TRUE if setting is RED\n 
      * - returns FALSE if setting is any other value\n 
      * */
    return ( (setting==OFF) || (setting==GREEN) || (setting==RED) );
}

//! When status_code is ERROR, pad responses to send expected number of bytes.
extern uint8_t PADDING;

#endif // _STATUSCODE_H
