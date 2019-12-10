/** \file */
#ifdef SPISLAVE_FAKED
#include "SpiSlave_faked.h"
#endif
#include "SpiSlave.h"
// avr headers included by Makefile:
// <avr/interrupt.h> defines macros `sei()` and `cli()`

// ---Private Functions---
static uint8_t ReadSpiStatusRegister(void) { return *Spi_spsr; }
static uint8_t ReadSpiDataRegister(void) { return *Spi_spdr; }
static inline void ClearSpiInterruptFlag(void)
{
    /** Manually clear SPI interrupt flag by first reading the
     * SPI status register, then reading the SPI data register.
     * */
    ReadSpiStatusRegister(); // in	r24, 0x2d
    ReadSpiDataRegister(); // in	r24, 0x2e
}
static void EnableSpiModule(void)
{
    /** Set SPE bit in SPCR (SPI Control Register) to enable SPI.
     * This bit must be set to enable any SPI operations.
      * */
    SetBit(Spi_spcr, Spi_Enable);
    // ---Expected Assembly---
    // in r24, 0x2c;
    // ori r24, 0x40;
    // out 0x2c, r24
    // This is three instructions because SPCR is outside the
    // address range for using `sbi`.
}
// ---Private Functions with fakes for unit tests---
#ifndef SPISLAVE_FAKED
static void EnableSpiInterrupt(void)
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
    SetBit(Spi_spcr, Spi_InterruptEnable);
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
#else
void EnableSpiInterrupt(void);
#endif

// ---API---
void SpiSlaveInit(void)
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
    EnableSpiModule();
    // Enable interrupts for robust SPI communication
    EnableSpiInterrupt();
}
