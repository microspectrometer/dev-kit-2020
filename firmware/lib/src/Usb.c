#include "Usb.h"
// ---Private---
void _FtDriveDatabus(void);
void _FtSampleDatabus(void);
void _FtWriteDatabus(uint8_t data);
// ---API---
bool UsbRxbufferIsEmpty(void);
bool UsbRxbufferIsFull(void);
uint8_t UsbRxbufferPop(void);
