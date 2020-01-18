/** \file
 * *See doc "USBProtocol": a table showing the expected payload
 * and responses (if any) for each command.*\n 
 * - Status codes:
 *   - `INVALID_CMD`: the sender received an unrecognized
 *   command\n 
 *   - `OK`: the sender understood the message it just received
 *   *and* it successfully performed the task
 *   - `ERROR`: the sender understood the message but could not
 *   perform the task
 *
 * The **USB board** (`usb-bridge`) and the **sensor board**
 * (e.g., `vis-spi-out`) reply with the same status codes.
 * */
#ifndef _STATUSCODES_H
#define _STATUSCODES_H
#include "StatusCode.h"

status_code OK = 0x00;
status_code ERROR = 0x01;
status_code INVALID_CMD = 0x02;

#endif // _STATUSCODES_H
