#ifndef _USB_H
#define _USB_H

#include <stdbool.h>    // bool, true, false
#include <stdint.h>     // uint8_t
/* TODO: move the jump-table functions to a new file so that lib Usb does not depend on Spi.h */
#include "Spi.h" // for `SpiMasterWriteByte` and definitions of `extern` /spi_.*_key

void UsbInit(void);
uint16_t UsbRead(uint8_t *read_buffer);
bool UsbHasDataToRead(void);
bool UsbHasRoomToWrite(void);


/* =====[ WIP: robust UsbRead functionality started 2019-03-01 ]===== */
uint8_t UsbReadOneByte(uint8_t *read_buffer);
// TODO: rename UsbRead to UsbReadAll then UsbReadN to UsbRead
// TODO: make UsbReadN a function pointer for faking.
/* uint16_t UsbReadN(uint8_t *read_buffer, uint16_t nbytes); */
extern uint16_t (*UsbReadN)(uint8_t *read_buffer, uint16_t nbytes);
/* uint16_t UsbWrite(uint8_t const *write_buffer, uint16_t nbytes); */
extern uint16_t (*UsbWrite)(uint8_t const *write_buffer, uint16_t nbytes);

// TODO: work on this to replace UsbReadN. UsbReadN does not wait for bytes
// ready.
uint16_t UsbReadBytes(uint8_t *read_buffer, uint16_t nbytes);

#endif // _USB_H
