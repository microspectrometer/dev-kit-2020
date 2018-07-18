#ifndef _TEST_USB_MOCKUPS_H
#define _TEST_USB_MOCKUPS_H

void SetUpMock_UsbRead(void);
void TearDownMock_UsbRead(void);
void SetUpMock_UsbWrite(void);
void TearDownMock_UsbWrite(void);
void SetUpMock_UsbHasDataToRead(void);
void TearDownMock_UsbHasDataToRead(void);
void SetUpMock_UsbHasRoomToWrite(void);
void TearDownMock_UsbHasRoomToWrite(void);

#endif // _TEST_USB_MOCKUPS_H
