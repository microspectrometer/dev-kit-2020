#ifndef _SPISLAVE_H
#define _SPISLAVE_H
#include <stdint.h>
#include "ReadWriteBits.h"
//---Hardware types: register addresses, pin numbers, bit numbers---
typedef uint8_t volatile * const spi_ptr; // i/o reg address
typedef uint8_t const spi_pin; // bit index of i/o reg for i/o pin
typedef uint8_t const spi_bit; // bit index into i/o reg
// Register address, pin number, and bit definitions depend on
// compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/Hardware.h
// ---Registers---
extern spi_ptr Spi_ddr;
extern spi_ptr Spi_port;
extern spi_ptr Spi_SPCR;
extern spi_ptr Spi_SPDR;
extern spi_ptr Spi_SPSR;
// ---Pins---
extern spi_pin Spi_DataReady; // slave signal DR to master
extern spi_pin Spi_Miso; // master-in, slave-out
// ---Bits---
extern spi_bit Spi_Enable;
extern spi_bit Spi_InterruptEnable;

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
inline void _SignalDataReady(void)
{
    /** Pin **Data Ready** is normally high.
     *  Drive **Data Ready** LOW to signal to the SPI Master that
     *  next byte of data is ready.
     * */
    ClearBit(Spi_port, Spi_DataReady);
}

// ---API (Go to the Doxygen documentation of this file)---
/** \file SpiSlave.h
 * # API
 * void SpiSlaveInit(void);\n 
 * void SpiSlaveTxByte(uint8_t input_byte);\n 
 * void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes);\n 
 * uint8_t ReadSpiStatusRegister(void);\n 
 * uint8_t ReadSpiDataRegister(void);\n 
 * void ClearSpiInterruptFlag(void);\n 
 * void DisableSpiInterrupt(void);\n 
 * void EnableSpiInterrupt(void);\n 
 * */
inline void SpiSlaveTxByte(uint8_t input_byte)
{
    /** SpiSlaveTxByte behavior:\n 
      * - loads SPI data register with input byte\n 
      * - drives DataReady LOW to signal data is ready\n 
      * */
    *Spi_SPDR = input_byte;
    _SignalDataReady();
    // ---Expected Assembly---
    // out	0x2e, r23	; 46
    // cbi	0x05, 1	; 5
}
inline void DisableSpiInterrupt(void)
{
    /** Clear SPIE bit in SPCR to disable the SPI ISR:\n 
     * - bit SPIF in register SPSR is still set when a serial
     *   transfer completes\n 
     * - clear SPIF manually by calling ClearSpiInterruptFlag\n 
     *   - SPIF is cleared by first reading the SPI status
     *   register, then accessing the SPI data register\n 
     * */
    // Disable the "transfer complete" interrupt
    ClearBit(Spi_SPCR, Spi_InterruptEnable);
    // ---Expected Assembly---
    // in	r24, 0x2c	; 44
    // andi	r24, 0x7F	; 127
    // out	0x2c, r24	; 44
    // This is three instructions because SPCR is outside the
    // address range for using `cbi`.
}
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
inline void ClearSpiInterruptFlag(void)
{
    /** Manually clear SPI interrupt flag by first reading the
     * SPI status register, then reading the SPI data register.
     * */
    ReadSpiStatusRegister(); // in	r24, 0x2d
    ReadSpiDataRegister(); // in	r24, 0x2e
}
inline void EnableSpiInterrupt(void)
{
    /** Set SPIE bit in SPCR to execute the SPI ISR when:\n 
     * - the Global Interrupt Enable bit is set in SREG\n 
     * - bit SPIF in register SPSR is set\n 
     *   - bit SPIF in register SPSR is set when a serial
     *     transfer completes\n 
     *   - SPIF is cleared when executing the SPI ISR\n 
     *   - alternatively, SPIF is cleared by first reading the
     *     SPI status register, then accessing the SPI data
     *     register\n 
     * */
    // Global interrupt disable
    cli(); // cli
    // Clear SPI interrupt flag (SPIF) before enabling interrupt
    ClearSpiInterruptFlag();
    // Enable the "transfer complete" interrupt
    SetBit(Spi_SPCR, Spi_InterruptEnable);
    // ---Expected Assembly---
    // in r24, 0x2c; 
    // ori r24, 0x80;
    // out 0x2c, r24
    // This is three instructions because SPCR is outside the
    // address range for using `sbi`.
    //
    // Global interrupt enable
    sei(); // sei
}

// ---API functions that call fakes when testing---
#ifdef SPISLAVE_FAKED
#include "SpiSlave_faked.h" // declare fakes
// Call fakes by renaming faked calls with _fake suffix.
#define EnableSpiInterrupt EnableSpiInterrupt_fake
#define SpiSlaveTxByte SpiSlaveTxByte_fake
#endif
inline void SpiSlaveInit(void)
{
    /** SpiSlaveInit behavior:\n 
      * - makes DataReady an output pin\n 
      * - idles DataReady high\n 
      * - makes Miso an output pin\n 
      * - enables SPI\n 
      * - enables SPI interrupt\n 
      * */
    // DataReady pin idle high
    SetBit(Spi_port, Spi_DataReady); // sbi	0x05, 1
    // Set DataReady as an output pin
    SetBit(Spi_ddr, Spi_DataReady); // sbi	0x04, 1
    // Set Miso as an an output pin
    SetBit(Spi_ddr, Spi_Miso); // sbi	0x04, 4
    _EnableSpiModule();
    // Enable interrupts for robust SPI communication
    EnableSpiInterrupt();
}
inline void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes)
{
    /** SpiSlaveTx behavior:\n 
      * - loads SPI data register with bytes from input buffer\n 
      * */
    uint16_t byte_index;
    for (byte_index = 0; byte_index < nbytes; byte_index++)
    {
        *Spi_SPDR = input_buffer[byte_index];
    }
}
#ifdef SPISLAVE_FAKED
// Remove `_fake` suffix from function names.
#undef EnableSpiInterrupt
#undef SpiSlaveTxByte
#endif

#endif // _SPISLAVE_H
