/** \file
  * `usb-bridge` communicates over USB via FTDI USB Bridge IC
  * `FT221X`
  *
  * - USB host sees `FT221X` as a **USB device**
  * - Two options for host to communicate with this USB device:
  *   1. use generic USB serial device driver, e.g., `pyserial`
  *   2. use the `D2XX` driver to access the expanded
  *     functionality of the FTDI USB API
  *
  * - The `usb-bridge` microcontroller sees `FT221X` as an
  *   **FT1248 slave**
  * 
  * Flow from USB host to `usb-bridge` microcontroller:\n 
  * - USB host writes to `FT221X`
  * - now `FT221X` has data in its receive buffer:
  *   - pin `FT1248 MISO` asserts LOW
  *   - this signals to the `usb-bridge` microcontroller that
  *     data is available
  * - `usb-bridge` microcontroller reads the data
  *
  * Flow from `usb-bridge` microcontroller to USB host:\n 
  * - TODO: fill this in
  * */
#ifndef _USB_H
#define _USB_H
#include <stdint.h>
#include <stdbool.h>
#include "ReadWriteBits.h"
//---Hardware types: register addresses, pin numbers, bit numbers---
typedef uint8_t volatile * const usb_reg; // i/o reg address
typedef uint8_t const usb_pin; // bit index into i/o reg for an i/o pin

// Register address, pin number, and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/Lis-Hardware.h
// ---Registers---
extern usb_reg FtCtrl_port;
extern usb_reg FtCtrl_pin;
extern usb_reg FtData_pin;
// ---Pins---
extern usb_pin FtMiso;
extern usb_pin FtMiosio0;

// ---API---
inline bool UsbRxbufferIsEmpty(void)
{
    /** UsbRxbufferIsEmpty behavior:\n 
      * - returns true if pin FT1248 MISO is HIGH\n 
      * - returns false if pin FT1248 MISO is LOW\n 
      * */
    return BitIsSet(FtCtrl_pin, FtMiso);
}
inline bool UsbRxbufferIsFull(void)
{
    return BitIsClear(FtData_pin, FtMiosio0);
}
#endif // _USB_H
