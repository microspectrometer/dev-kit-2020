#include "SpiSlave.h"
// avr headers included by Makefile:
// <avr/interrupt.h> defines macros `sei()` and `cli()`

static uint8_t ReadSpiDataRegister(void) { return *Spi_spdr; }
static uint8_t ReadSpiStatusRegister(void) { return *Spi_spsr; }
void SpiSlaveInit(void)
{
    /** SpiSlaveInit behavior:\n 
      * - makes DataReady an output pin\n 
      * - idles DataReady high\n 
      * */
    // DataReady pin idle high
    SetBit(Spi_port, Spi_DataReady); // sbi	0x05, 1
    // Set DataReady as an output pin
    SetBit(Spi_ddr, Spi_DataReady); // sbi	0x04, 1
    // TODO: add unit tests from here down
    // Set Miso as an an output pin
    SetBit(Spi_ddr, Spi_Miso); // sbi	0x04, 4
    // Enable SPI
    SetBit(Spi_spcr, Spi_Enable); // in r24, 0x2c; ori r24, 0x40; out 0x2c, r24
    // Clear pending interrupt by reading status then data
    ReadSpiStatusRegister(); // in	r24, 0x2d
    ReadSpiDataRegister(); // in	r24, 0x2e
    // Enable interrupts for robust SPI communication
    // Global interrupt disable
    cli(); // cli
    // Clear pending interrupt by reading status then data
    ReadSpiStatusRegister(); // in	r24, 0x2d
    ReadSpiDataRegister(); // in	r24, 0x2e
    // Enable the "transfer complete" interrupt
    SetBit(Spi_spcr, Spi_InterruptEnable); // in r24, 0x2c; ori r24, 0x80; out 0x2c, r24
    // Global interrupt enable
    sei(); // sei
}
