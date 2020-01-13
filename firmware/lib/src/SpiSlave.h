#ifndef _SPISLAVE_H
#define _SPISLAVE_H
#include <stdint.h>
#include <stdbool.h>
#include "ReadWriteBits.h"
#include "Flag.h"
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

#ifdef SPISLAVE_FAKED
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
    //! Drive **Data Ready** HIGH
    SetBit(Spi_port, Spi_DataReady);
    // ---Expected Assembly---
    //  sbi	0x05, 1	; 5
}
inline void _SpiSlave_StopRxQueue(void)
{
    //! Clear `Flag_SlaveRx`
    /** SPI ISR checks `Flag_SlaveRx` to decide whether to Queue
     *  the byte received from the SPI master:\n 
     *  - flag is **set**: store SPI data register in the Queue\n
     *  - flag is **clear**: do not store SPI data register in
     *  the Queue
     * */
    ClearBit(Flag_SpiFlags, Flag_SlaveRx);
    // ---Expected Assembly---
    //  cbi	0x1e, 0	; 30
}
inline bool _TransferIsDone(void)
{
    /** TransferIsDone behavior:\n 
      * - returns true when ISR sets Flag TransferIsDone\n 
      * - returns false until ISR sets Flag TransferIsDone\n 
      * */
    return BitIsSet(Flag_SpiFlags, Flag_TransferDone);
    // ---Expected Assembly---
    // Used as the condition in a blocking while loop:
    //      while (!_TransferIsDone());
    // Expected assembly for the while loop is:
    //  sbis	0x1e, 1	; 30
    //  rjmp	.-4      	; 0x206 <main+0x160>
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
#ifdef SPISLAVE_FAKED
#define _TransferIsDone _TransferIsDone_fake
#define _SignalDataReady _SignalDataReady_fake
#endif
inline void SpiSlaveTxByte(uint8_t input_byte)
{
    //! Transmit a byte and ignore byte received.
    /** SpiSlaveTxByte behavior:\n 
      * - loads SPI data register with input byte\n 
      * - tells SPI ISR to ignore rx byte\n 
      * - drives DataReady LOW to signal data is ready\n 
      * - waits until SPI transfer is done\n 
      * - drives DataReady HIGH to sync with Master\n 
      * */
    *Spi_SPDR = input_byte;
    // ---Expected Assembly---
    // out	0x2e, r23	; 46
    _SpiSlave_StopRxQueue();
    _SignalDataReady();
    // ---Expected Assembly---
    // cbi	0x05, 1	; 5
    // TODO: ClearBit(Flag_SpiFlags, Flag_TransferDone);
    while (!_TransferIsDone()); // Flag set in ISR
    // ---Expected Assembly---
    //  206:	sbis	0x1e, 1	; 30
    //  208:	rjmp	.-4      	; 0x206 <main+0x160>
    _SignalDataNotReady();
    // TODO: SetBit(Flag_SpiFlags, Flag_SlaveRx);
}
#ifdef SPISLAVE_FAKED
#undef _TransferIsDone
#undef _SignalDataReady
#endif
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
#ifdef SPISLAVE_FAKED
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
#ifdef SPISLAVE_FAKED
#undef ClearSpiInterruptFlag
#endif


// ---API functions that call fakes when testing---
#ifdef SPISLAVE_FAKED
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
#ifdef SPISLAVE_FAKED
// Remove `_fake` suffix from function names.
#undef EnableSpiInterrupt
#endif
#ifdef SPISLAVE_FAKED
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
#ifdef SPISLAVE_FAKED
// Remove `_fake` suffix from function names.
#undef SpiSlaveTxByte
#endif

#endif // _SPISLAVE_H
