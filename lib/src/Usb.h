#ifndef _USB_H
#define _USB_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t

bool UsbRead(uint8_t *read_buffer_address);

#endif // _USB_H
