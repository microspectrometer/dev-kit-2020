#ifndef _USB_H
#define _USB_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t

uint16_t UsbRead(uint8_t *read_buffer);
uint16_t UsbWrite(uint8_t *write_buffer, uint16_t nbytes);

#endif // _USB_H
