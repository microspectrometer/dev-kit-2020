/** \file
 * *See doc "USBProtocol": a table showing the expected payload
 * and responses (if any) for each command.*\n 
 * **byte 0** of the reply is a **status code**.\n 
 * The **Status code** describes the status of the communication
 * *from the point of view of the sender*.\n 
 *
 * The **USB board** (`usb-bridge`) and the **sensor board**
 * (e.g., `vis-spi-out`) reply with the same status codes.
 * */
#ifndef _STATUSCODE_H
#define _STATUSCODE_H
#include "stdint.h"

//! Status codes are type `status_code`.
typedef uint8_t status_code;

extern status_code INVALID_CMD;

#endif // _STATUSCODE_H
