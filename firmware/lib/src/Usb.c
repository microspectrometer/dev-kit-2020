#include "Usb.h"

// ---Private---
void _FtClockDatabus(uint8_t direction);
void _FtReadDatabus(uint8_t * pbyte);
void _FtWriteDatabus(uint8_t byte);
void _FtDatabusPinDirection(uint8_t pin_direction);

// ---Ft API used internally by Usb---
void FtSelectFT221X(void);
void FtUnselectFT221X(void);
void FtBusTurnaround(void);
bool FtIsOk(void);
void FtRead(uint8_t * pbyte);
void FtWrite(uint8_t byte);

// ---API---
bool UsbRxbufferIsEmpty(void);
bool UsbTxbufferIsFull(void);
uint8_t UsbReadByte(uint8_t * pbyte);
uint8_t UsbWriteByte(uint8_t byte);
