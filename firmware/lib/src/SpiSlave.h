#ifndef _SPISLAVE_H
#define _SPISLAVE_H
#include <stdint.h>
#include <stdbool.h>
#include "ReadWriteBits.h"
#include "Spi.h"

#ifdef USE_FAKES
#include "Spi_faked.h"
#include "SpiSlave_faked.h" // declare fakes
#endif

// ---Private---
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
    ClearBit(Spi_PortOutput, Spi_DataReady);
    // ---Expected Assembly---
    //  cbi	0x05, 1	; 5
}
inline void _SignalDataNotReady(void)
{
    //! Drive **Data Ready** HIGH.
    SetBit(Spi_PortOutput, Spi_DataReady);
    // ---Expected Assembly---
    //  sbi	0x05, 1	; 5
}

// ---API (Go to the Doxygen documentation of this file)---
/** \file SpiSlave.h
 * # API
 * void DisableSpiInterrupt(void);\n 
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
    SetBit(Spi_PortOutput, Spi_DataReady); // sbi	0x05, 1
    // Set DataReady as an output pin
    SetBit(Spi_PortDirection, Spi_DataReady); // sbi	0x04, 1

    // Set Miso as an an output pin
    SetBit(Spi_PortDirection, Spi_Miso); // sbi	0x04, 4

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
