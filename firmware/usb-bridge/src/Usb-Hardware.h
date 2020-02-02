/** \file
  * `usb-bridge` communicates over USB via FTDI USB Bridge IC
  * `FT221X`
  *
  * About the `FT221X`:
  *
  * - Provides an embedded interface to a **USB host**
  *   - embedded system **microcontroller** is the **FT1248 master**
  *   - **microcontroller** sees `FT221X` as **FT1248 slave**
  *   - **USB host** sees `FT221X` as a **USB device**
  *   - `USB` protocol is entirely handled by the `FT221X`
  * - Data:
  *   - USB 2.0 Full Speed capable
  *   - Data transfer rate up to 1MByte/s (TODO: measure)
  *   - 512-byte receive buffer
  *   - 512-byte transmit buffer
  * - USB Driver options for Windows, Mac OS X, and Linux:
  *   - Virtual Com Port (VCP)
  *     - this is what `pyserial` uses
  *     - Chromation uses `pyserial` for the Python API
  *   - D2XX
  *     - FTDI-specific extended functionality
  *     - write your own Python package to wrap D2XX API calls
  *       - examples available on the internet
  *   - Both types free to download from FTDI:
  *     - https://www.ftdichip.com/Drivers/VCP.htm
  *     - scroll down to your operating system
  *
  * `FT221X` usage in Chromation `usb-bridge`:
  * - `USB` to `FT1248`
  *   - `usb-bridge` uses `FT1248` mode with 8-bit-wide data bus
  *   - `USB` protocol is entirely handled by the `FT221X`
  *     - `usb-bridge` firmware implements an `FT1248` master
  *     - `usb-bridge` firmware is ignorant of `USB` specifics
  * - Program internal 2kB memory via FTDI GUI `FT_Prog`:
  *   - Chromation serial number: `CHROMATION123456`
  *     - each dev-kit has a unique six-digit serial number
  *   - *Ft1248 Settings*
  *     - *Clock Polarity High*: `uncheck`
  *     - *Bit Order LSB*: `check`
  *     - *Flow Ctrl not selected*: `check`
  *       - implements flow control when `FT221X` is not selected
  *   - *CBUS Signals*
  *     - *C3*: `GPIO`
  * - Flow control:
  *   - when `FT221X` pin `CS#` (**Not** Chip Select) is HIGH:
  *   - reflect status of receive buffer on `FT221X` pin `MISO`
  *     - receive buffer **has data**: `MISO` is **LOW**
  *     - receive buffer is empty: `MISO` is **HIGH**
  *     - *usage:* wait for `MISO` **LOW** before a read
  *   - reflect status of transmit buffer on `FT221X` pin
  *     `MIOSIO[0]`
  *     - transmit buffer **is full**: `MIOSIO[0]` is **LOW**
  *     - transmit buffer has room: `MIOSIO[0]` is **HIGH**
  *     - *usage:* wait for `MIOSIO[0]` **HIGH** before a write
  * - Dev-kit hard reset:
  *   - `FT221X` pin `CBUS3` connects to the microcontroller
  *     RESET pin
  *   - USB host drives CBUS3 LOW to **hard reset** the dev-kit
 * */
#ifndef _USB_HARDWARE_H
#define _USB_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "Usb.h"
// ---Registers---
// control lines
/* usb_reg FtCtrl_ddr = &DDRC; */
usb_reg FtCtrl_port = &PORTC;
usb_reg FtCtrl_pin  = &PINC;
// bi-directional data lines (driven by both master and slave)
usb_reg FtData_pin  = &PIND;
usb_reg FtData_ddr  = &DDRD;
usb_reg FtData_port = &PORTD;
// ---Pins---
// control lines
usb_pin FtMiso       = PC2; // input: !RxbufferEmpty signal
usb_pin FtChipSelect = PC0; // output: activate interface
usb_pin FtClock      = PC1; // output: clock edges signal drive/sample data on bus
// bi-directional data lines (driven by both master and slave)
usb_pin FtMiosio0 = PD0; // data bus bit 0
#endif // _USB_HARDWARE_H

