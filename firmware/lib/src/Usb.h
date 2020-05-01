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
  * - Control lines
  *   - `FtClock`
  *     - the clock idles `LOW`
  *     - the clock is only active during FT1248 communication
  * - Sending a single byte takes three clock cycles
  * - TODO: multi-byte communication where each additional byte only adds one
  *   clock cycle
  * - Databus pin **direction**
  *   - databus pins are `FtMiosio0` through `FtMiosio7`
  *   - the databus is bi-directional
  *     - `FtRead`: the *FT221X* drives the databus
  *     - `FtWrite` the *microcontroller* drives the databus
  *   - databus pins are usually microcontroller inputs
  *   - microcontroller **only outputs to the databus during an `FtWrite`**
  *     - the microcontroller pins are outputs for the interval
  *     - starting after a clock rising edge
  *     - ending before the next clock rising edge
  * - TODO: fill this in
  * */
#ifndef _USB_H
#define _USB_H
#include <stdint.h>
#include <stdbool.h>
#include "ReadWriteBits.h"
#ifdef USE_FAKES
#include "Usb_faked.h" // declare fakes
#endif
//---Hardware types: register addresses, pin numbers, bit numbers---
typedef uint8_t volatile * const usb_reg; // i/o reg address
typedef uint8_t const usb_pin; // bit index into i/o reg for an i/o pin

// Register address, pin number, and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/Lis-Hardware.h
// ---Registers---
// control lines
extern usb_reg FtCtrl_port;
extern usb_reg FtCtrl_pin;
// bi-directional data lines (driven by both master and slave)
extern usb_reg FtData_pin;
extern usb_reg FtData_ddr;
extern usb_reg FtData_port;
// ---Pins---
// control lines
extern usb_pin FtMiso;
extern usb_pin FtChipSelect;
extern usb_pin FtClock;
// bi-directional data lines (driven by both master and slave)
extern usb_pin FtMiosio0;
// FtMiosio0 is used on its own for flow control.
// FtMiosio[0:7] are used together as a single port for data.
// Write access: *FtData_port
// Read access: *FtData_pin

// TODO: change to variables in style of LisConfig and LisConfigs
enum ft_cmd {FtReadCmd=0xC6, FtWriteCmd=0x86};
enum ft_databus_pin_direction {FtIn=0x00, FtOut=0xFF};
enum ft_status {FtError=0, FtOk=1};
enum ft_clock_edge {FtSample=0, FtDrive=1};

// ---Private---
inline void _FtClockDatabus(uint8_t direction)
{ // TODO: check compiler eliminates the conditional
    /** FtClockDatabus behavior:\n 
      * - drives FtClock HIGH if direction is FtDrive\n 
      * - drives FtClock LOW if direction is FtSample\n 
      * */
    direction ?
        SetBit(FtCtrl_port, FtClock) :  // FtDrive==direction
        ClearBit(FtCtrl_port, FtClock); // FtSample==direction
}
inline void _FtReadDatabus(uint8_t * pbyte)
{
    /** FtReadDatabus behavior:\n 
      * - copies databus pin values to address pbyte\n 
      * */
    *pbyte = *FtData_pin;
}
inline void _FtWriteDatabus(uint8_t byte)
{
    /** FtWriteDatabus behavior:\n 
      * - outputs byte on databus pins\n 
      * */
    *FtData_port = byte;
}
inline void _FtDatabusPinDirection(uint8_t pin_direction)
{
    /** FtDatabusPinDirection behavior:\n 
      * - makes databus pins outputs if direction is FtOut\n 
      * - makes databus pins inputs if direction is FtIn\n 
      * */
    *FtData_ddr = pin_direction;
}

// ---Ft API used internally by Usb---
inline void FtSelectFT221X(void)
{
    /** FtSelectFT221X behavior:\n 
      * - drives FtChipSelect LOW\n 
      * */
    ClearBit(FtCtrl_port, FtChipSelect);
}
inline void FtUnselectFT221X(void)
{
    /** FtUnselectFT221X behavior:\n 
      * - drives FtChipSelect HIGH\n 
      * */
    SetBit(FtCtrl_port, FtChipSelect);
}

#ifdef USE_FAKES
#define _FtClockDatabus _FtClockDatabus_fake
#endif
inline void FtBusTurnaround(void)
{
    /** FtBusTurnaround behavior:\n 
      * - clocks one cycle to signal data drive then data sample\n 
      * */
    // Clock the FtStatus onto FtMiso
    _FtClockDatabus(FtDrive);  // FtClock rising edge
    _FtClockDatabus(FtSample); // FtClock falling edge
}
#ifdef USE_FAKES
#undef _FtClockDatabus
#endif

inline bool FtIsOk(void)
{
    /** The FT221X indicates data transfer status on Pin `MISO` while FT1248
      * communication is active.\n
      * `LOW` on pin `MISO` means *FT221X status is OK.*\n
      * - See the **8-bit write** and **8-bit read** timing diagrams:
      *   - [FT221X datasheet](https://www.ftdichip.com/Support/Documents/DataSheets/ICs/DS_FT221X.pdf)
      * - See ACK/NAK details in section **FT1248: NAK**:
      *   - [FTDI application note AN167](https://www.ftdichip.com/Support/Documents/AppNotes/AN_167_FT1248_Parallel_Serial_Interface_Basics.pdf)
      *
      * Timing diagram and ACK/NAK summary:\n
      * - During the `BUS TURNAROUND` after the `COMMAND PHASE`
      *   - if command is **8-bit read**
      *     - `LOW` indicates **receive buffer** *has data* to read
      *     - `HIGH` indicates **receive buffer** *is empty*
      *   - if command is **8-bit write**
      *     - `LOW` indicates **transmit buffer** *has room* to write
      *     - `HIGH` indicates **transmit buffer** *is full*
      * - During the `DATA` phase
      *     - after the clock *rising* edge
      *       - FT221X outputs an `ACK` or `NAK` on `MISO`
      *       - `ACK`:
      *         - `MISO` is `LOW`
      *         - indicates a *successful* data transfer
      *       - `NAK`:
      *         - `MISO` is `HIGH`
      *         - indicates a *failed* data transfer
      *     -  after clock *falling* edge:
      *       - Microcontroller reads `MISO`
      *         - `LOW` indicates `ACK`
      *         - `HIGH` indicates `NAK`
      * - Data transers **fail** because of either:
      *   - a *buffer full* condition on **writing data** to the FT221X
      *   - a *buffer empty* condition on **reading data** from the FT221X
      * */
    /** FtIsOk behavior:\n 
      * - returns true if FtMiso is LOW\n 
      * - returns false if FtMiso is HIGH\n 
      * */
    return BitIsClear(FtCtrl_pin, FtMiso);
}

#ifdef USE_FAKES
#define _FtClockDatabus _FtClockDatabus_fake
#define _FtReadDatabus _FtReadDatabus_fake
#define _FtDatabusPinDirection _FtDatabusPinDirection_fake
#define _FtWriteDatabus _FtWriteDatabus_fake
#endif
inline void FtRead(uint8_t * pbyte)
{
    /** FtRead behavior:\n 
      * - clocks one byte out of the FT221X\n 
      * - stores the byte at address pbyte\n 
      * */
    // Clock a byte out of the FT221X
    _FtClockDatabus(FtDrive); // FtClock rising edge
    _FtClockDatabus(FtSample); // FtClock falling edge
    // Read the byte into pbyte
    _FtReadDatabus(pbyte);
}
inline void FtWrite(uint8_t byte)
{
    /** FtWrite behavior:\n 
      * - signals to drive data onto the databus\n 
      * - sets microcontroller databus pins as outputs\n 
      * - outputs byte on databus pins\n 
      * - signals FT221X to sample the databus\n 
      * - sets microcontroller databus pins as inputs\n 
      * */
    // Set databus for driving data
    _FtClockDatabus(FtDrive); // FtClock rising edge
    // Take control of databus
    _FtDatabusPinDirection(FtOut);
    // Write the byte
    _FtWriteDatabus(byte);
    // Clock the byte into the FT221X
    _FtClockDatabus(FtSample); // FtClock falling edge
    // Relinquish databus to FT221X
    _FtDatabusPinDirection(FtIn);
}
#ifdef USE_FAKES
#undef _FtClockDatabus
#undef _FtReadDatabus
#undef _FtDatabusPinDirection
#undef _FtWriteDatabus
#endif

// ---API---
inline bool UsbRxbufferIsEmpty(void)
{
    /** UsbRxbufferIsEmpty behavior:\n 
      * - returns true if pin FT1248 MISO is HIGH\n 
      * - returns false if pin FT1248 MISO is LOW\n 
      * */
    return BitIsSet(FtCtrl_pin, FtMiso);
}
inline bool UsbTxbufferIsFull(void)
{
    /** UsbTxbufferIsFull behavior:\n 
      * - returns true if pin MIOSIO0 is HIGH\n 
      * - returns false if pin MIOSIO0 is LOW\n 
      * */
    return BitIsSet(FtData_pin, FtMiosio0);
}

#ifdef USE_FAKES
#define FtSelectFT221X      FtSelectFT221X_fake
#define FtBusTurnaround     FtBusTurnaround_fake
#define FtIsOk              FtIsOk_fake
#define FtRead              FtRead_fake
#define FtWrite             FtWrite_fake
#define FtUnselectFT221X    FtUnselectFT221X_fake
#endif
inline uint8_t UsbReadByte(uint8_t * pbyte)
{
    /** UsbReadByte behavior:\n 
      * - selects the FT221X\n 
      * - drives databus with read command\n 
      * - signals FT221X to sample the databus\n 
      * */
    uint8_t status = FtError;
    FtSelectFT221X(); // activates FT1248 interface
    FtWrite(FtReadCmd); // Clock FtReadCmd into FT221X
    FtBusTurnaround(); // Relinquish bus to FT221X
    if (FtIsOk())
    {
        status = FtOk; // Buffer is not empty, OK to read a byte
        FtRead(pbyte); // Clock byte out of FT221X UsbRx buffer
    }
    FtUnselectFT221X(); // deactivates FT1248 interface
    return status; // up to caller to check status
}
inline uint8_t UsbWriteByte(uint8_t byte)
{
    uint8_t status = FtError;
    FtSelectFT221X(); // activates FT1248 interface
    FtWrite(FtWriteCmd); // Clock FtWriteCmd into FT221X
    FtBusTurnaround(); // Relinquish bus to FT221X
    if (FtIsOk())
    {
        status = FtOk; // Buffer is not full, OK to write a byte
        FtWrite(byte); // Clock byte into FT221X UsbTx buffer
    }
    FtUnselectFT221X(); // deactivates FT1248 interface
    return status; // up to caller to check status
}
#ifdef USE_FAKES
#undef FtSelectFT221X
#undef FtBusTurnaround
#undef FtIsOk
#undef FtRead
#undef FtWrite
#undef FtUnselectFT221X
#endif

#endif // _USB_H
