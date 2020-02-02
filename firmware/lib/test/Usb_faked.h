#ifndef _USB_FAKED_H
#define _USB_FAKED_H
#include <stdio.h>

// ---Private functions faked for testing---
void _FtDriveDatabus_fake(void);
void _FtSampleDatabus_fake(void);
void _FtWriteDatabus_fake(uint8_t data);

#endif // _USB_FAKED_H
