/** \file
 * See StatusCode.h for context.\n
 *\n
 * Also see `cfg/chromaspec.json` in the Python API repository.\n
 * - Section `"global"` defines the **protocol byte codes**.\n
 * - The **protocol byte code** definitions are duplicated in
 *   this header file.\n
 * */
#ifndef _STATUSCODES_H
#define _STATUSCODES_H
#include "StatusCode.h"

status_code OK = 0x00;
status_code ERROR = 0x01;
status_code INVALID_CMD = 0x02;

led_state OFF = 0x00;
led_state GREEN = 0x01;
led_state RED = 0x02;

uint8_t PADDING = 0xFF;

#endif // _STATUSCODES_H
