#ifndef _SPISLAVE_H
#define _SPISLAVE_H
#include <stdint.h>
#include <stdbool.h>
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
extern spi_bit Spi_InterruptFlag;

#ifdef USE_FAKES
#include "SpiSlave_faked.h" // declare fakes
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
inline void _SignalDataReady(void)
{
    /** _SignalDataReady behavior:\n 
      * - drives DataReady LOW\n 
      * */
    /** SPI Slave outputs signal **Data Ready**.\n 
      * - **Data Ready** LOW signals next byte of data is
      *   ready.\n
      * - SPI Master waits for **Data Ready** LOW before reading
      *   a byte from the SPI Slave.\n
      * - SPI Slave drives **Data Ready** HIGH after each byte of
      *   SPI transfer.
      * */
    ClearBit(Spi_port, Spi_DataReady);
    // ---Expected Assembly---
    //  cbi	0x05, 1	; 5
}
inline void _SignalDataNotReady(void)
{
    //! Drive **Data Ready** HIGH.
    SetBit(Spi_port, Spi_DataReady);
    // ---Expected Assembly---
    //  sbi	0x05, 1	; 5
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
 * void DisableSpiInterrupt(void);\n 
 * uint8_t ReadSpiStatusRegister(void);\n 
 * uint8_t ReadSpiDataRegister(void);\n 
 * void ClearSpiInterruptFlag(void);\n 
 * void EnableSpiInterrupt(void);\n 
 * void SpiSlaveInit(void);\n 
 * void SpiSlaveTxByte(uint8_t input_byte);\n 
 * void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes);\n 
 * */
inline void DisableSpiInterrupt(void)
{
    //! Clear `SPIE` bit in `SPCR` to disable the SPI ISR.
    /** DisableSpiInterrupt behavior:\n 
      * - clears the SPI Interrupt Enable bit\n 
      * */
    /** Note:\n 
      * - Even with the SPI ISR disabled, interrupt flag bit
      * `SPIF` in register `SPSR` is still set when a serial
      * transfer completes.\n 
      * - Check the flag by calling `_SpiTransferIsDone()`\n 
      * - Clear `SPIF` manually by calling
      *   `ClearSpiInterruptFlag()`\n 
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
    //! Manually clear SPI interrupt flag.
    /** ClearSpiInterruptFlag behavior:\n 
      * - first reads SPI status register\n 
      * - then reads SPI data register\n 
      * */

    ReadSpiStatusRegister(); // in	r24, 0x2d
    ReadSpiDataRegister(); // in	r24, 0x2e
}
// ---API functions that call fakes when testing---
//
#ifdef USE_FAKES
#define ClearSpiInterruptFlag ClearSpiInterruptFlag_fake
#endif
inline void EnableSpiInterrupt(void)
{
    /** EnableSpiInterrupt behavior:\n 
      * - clears SPI interrupt flag\n 
      * - enables SPI transfer complete interrupt\n 
      * - consumes 6 cycles\n 
      * */
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
#ifdef USE_FAKES
#undef ClearSpiInterruptFlag
#endif

#ifdef USE_FAKES
// Call fakes by renaming faked calls with _fake suffix.
#define EnableSpiInterrupt EnableSpiInterrupt_fake
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
#ifdef USE_FAKES
// Remove `_fake` suffix from function names.
#undef EnableSpiInterrupt
#endif

#ifdef USE_FAKES
#define _SpiTransferIsDone _SpiTransferIsDone_fake
#define _SignalDataReady _SignalDataReady_fake
#define DisableSpiInterrupt DisableSpiInterrupt_fake
#endif
inline void SpiSlaveTxByte(uint8_t input_byte)
{
    //! Transmit a byte and ignore byte received.
    /** SpiSlaveTxByte behavior:\n 
      * - loads SPI data register with input byte\n 
      * - disables SPI ISR before signaling data ready\n 
      * - drives DataReady LOW to signal data is ready\n 
      * - waits until SPI transfer is done\n 
      * - drives DataReady HIGH immediately after SPI transfer
      *   finishes\n 
      * - enables SPI ISR after transfer\n 
      * */
    *Spi_SPDR = input_byte;
    // ---Expected Assembly---
    // out	0x2e, r23	; 46
    DisableSpiInterrupt();
    // ---Expected Assembly---
    // in	r24, 0x2c	; 44
    // andi	r24, 0x7F	; 127
    // out	0x2c, r24	; 44
    _SignalDataReady();
    // ---Expected Assembly---
    // cbi	0x05, 1	; 5
    // Wait for a byte from the SPI Master.
    while ( !_SpiTransferIsDone() ); // Check SPI interrupt flag
    // ---Expected Assembly---
    // Total number of cycles: 5
    // in	r0, 0x2d
    // sbrs	r0, 7
    // rjmp	.-6
    _SignalDataNotReady();
    // Re-enable interrupt and reset (clear) SPI interrupt flag
    EnableSpiInterrupt();
    // ---Expected Assembly---
    // Total number of cycles: 7
    // cli
    // in	r24, 0x2d
    // in	r24, 0x2e
    // in	r24, 0x2c
    // ori	r24, 0x80
    // out	0x2c, r24
    // sei
}
#ifdef USE_FAKES
#undef _SpiTransferIsDone
#undef _SignalDataReady
#undef DisableSpiInterrupt
#endif

#ifdef USE_FAKES
#define SpiSlaveTxByte SpiSlaveTxByte_fake
#endif
inline void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes)
{
    /** SpiSlaveTx behavior:\n 
      * - sends nbytes of input buffer to SpiMaster\n 
      * */
    uint16_t byte_index;
    for (byte_index = 0; byte_index < nbytes; byte_index++)
    {
        SpiSlaveTxByte(input_buffer[byte_index]);
    }
}
#ifdef USE_FAKES
#undef SpiSlaveTxByte
#endif

#endif // _SPISLAVE_H
