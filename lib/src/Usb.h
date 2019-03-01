#ifndef _USB_H
#define _USB_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t

void UsbInit(void);
uint16_t UsbRead(uint8_t *read_buffer);
uint16_t UsbWrite(uint8_t *write_buffer, uint16_t nbytes);
bool UsbHasDataToRead(void);
bool UsbHasRoomToWrite(void);

/* =====[ WIP: robust UsbRead functionality started 2019-03-01 ]===== */
uint8_t UsbReadOneByte(uint8_t *read_buffer);

#endif // _USB_H
