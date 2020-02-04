#ifndef _USB_FAKED_H
#define _USB_FAKED_H
#include <stdio.h>
#include <stdbool.h>

// ---Private functions faked for testing---
void _FtClockDatabus_fake(uint8_t);
void _FtReadDatabus_fake(uint8_t * pbyte);
void _FtWriteDatabus_fake(uint8_t byte);
void _FtDatabusPinDirection_fake(uint8_t pin_direction);

// ---Ft API used internally by Usb---
void FtSelectFT221X_fake(void);
void FtBusTurnaround_fake(void);
bool FtIsOk_fake(void);
void FtRead_fake(uint8_t * pbyte);
void FtWrite_fake(uint8_t byte);
void FtUnselectFT221X_fake(void);

#endif // _USB_FAKED_H
