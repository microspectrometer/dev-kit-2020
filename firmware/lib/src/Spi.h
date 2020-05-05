/** \file
 * `Spi.h` declares SPI hardware types and variables common to
 * the *SPI Master* and *SPI Slave*.
 * - SPI Master: `usb-bridge`\n
 * - SPI Slave: `vis-spi-out`\n
 * .
 * **Definitions** for **I/O register** and **bit number** depend on the *compiler* and *target*:
 * - `lib/test/Spi-HardwareFake.h` defines i/o for **compiler** `gcc`, **target** `unit-test`
 * - `usb-bridge/src/Spi-Hardware.h` defines i/o for **compiler** `avr-gcc`, **target** `usb-bridge`
 * - `vis-spi-out/src/Spi-Hardware.h` defines i/o for **compiler** `avr-gcc`, **target** `vis-spi-out`
 * .
 * A SPI interface usually only has four SPI pins:
 * - **SS**: *Slave Select*
 * - **SCK**: *SPI Clock*
 * - **MOSI**: *Master* data *OUT*, *Slave* data *IN*
 * - **MISO**: *Master* data *IN*, *Slave* data *OUT*
 * .
 * The development kit uses a 5th SPI pin:
 * - **DR**: *Data Ready*
 * .
 * **Data Ready** signals when `vis-spi-out` is ready for a **SPI
 * transmission**:
 * - **Data Ready** is normally HIGH
 * - `vis-spi-out` **drives Data Ready LOW** to signal ready for transmission.
 * - `vis-spi-out` **drives Data Ready HIGH** when transmission is done.
 * - for *multi-byte transmissions*, **Data Ready** is driven **LOW before each byte** and **HIGH after each byte**.
 * */
#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>
#include "ReadWriteBits.h"

// ---Hardware i/o types---
typedef uint8_t volatile * const spi_reg; // i/o reg address
typedef uint8_t const spi_bit; // bit index into i/o reg

// ---i/o Registers---
//! Atmel DDR
extern spi_reg Spi_PortDirection;
//! Atmel PIN
extern spi_reg Spi_PortInput;
//! Atmel PORT
extern spi_reg Spi_PortOutput;
//! Atmel PORT alias
extern spi_reg Spi_PortPullup;
//! SPI Control Register
extern spi_reg Spi_SPCR;
//! SPI Status Register
extern spi_reg Spi_SPSR;
//! SPI Data Register
extern spi_reg Spi_SPDR;

// ---i/o Pins---
extern spi_bit Spi_DataReady;
extern spi_bit Spi_Ss;
extern spi_bit Spi_Mosi;
extern spi_bit Spi_Miso;
extern spi_bit Spi_Sck;

// ---i/o Bits---
extern spi_bit Spi_MasterSlaveSelect; // bit MSTR in SPCR (0x2C)
extern spi_bit Spi_ClockBit0;         // bit 0 in SPCR (0x2C)
extern spi_bit Spi_ClockBit1;         // bit 1 in SPCR (0x2C)
extern spi_bit Spi_DoubleClock;       // bit 0 in SPSR (0x2D)
extern spi_bit Spi_Enable;            // bit 6 in SPCR (0x2C)
extern spi_bit Spi_InterruptEnable;   // bit 7 in SPCR (0x2C)
extern spi_bit Spi_InterruptFlag;     // bit 7 in SPSR (0x2D)

#ifdef USE_FAKES
#include "Spi_faked.h" // declare fakes
#endif
// ---Private---
inline void _EnableSpiModule(void)
{
    /** Set SPE bit in SPCR (SPI Control Register) to enable SPI.
     * This bit must be set to enable any SPI operations.
     * */
    SetBit(Spi_SPCR, Spi_Enable);
    // ---Expected Assembly---
    // in r24, 0x2c;
    // ori r24, 0x40;
    // out 0x2c, r24
    // This is three instructions because SPCR is outside the
    // address range for using `sbi`.
}
inline bool _SpiTransferIsDone(void)
{
    /** _SpiTransferIsDone behavior:\n 
      * - returns true if the SPI Interrupt Flag is set\n 
      * - returns false if the SPI Interrupt Flag is clear\n 
      * */
    return BitIsSet(Spi_SPSR, Spi_InterruptFlag);
}

// ---API (Go to the Doxygen documentation of this file)---
/** \file SpiSlave.h
 * # API
 * uint8_t ReadSpiStatusRegister(void);\n 
 * uint8_t ReadSpiDataRegister(void);\n 
 * */
inline uint8_t ReadSpiStatusRegister(void)
{
    return *Spi_SPSR;
    // ---Expected Assembly---
    // in	r24, 0x2d	; 45
}
inline uint8_t ReadSpiDataRegister(void)
{
    return *Spi_SPDR;
    // ---Expected Assembly---
    // in	r24, 0x2e	; 46
}

#endif // _SPI_H
