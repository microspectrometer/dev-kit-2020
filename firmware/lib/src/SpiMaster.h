#ifndef _SPIMASTER_H
#define _SPIMASTER_H
#include <stdint.h>
#include "ReadWriteBits.h"
#include "Spi.h"

#ifdef USE_FAKES
#include "Spi_faked.h"
/* #include "SpiMaster_faked.h" // no need for this yet */
#endif

// ---Private---
//
// ---API (Go to the Doxygen documentation of this file)---
/** \file SpiMaster.h
 * # API
 * void SpiMasterInit(void);\n 
 * void SpiMasterXfrByte(uint8_t byte);\n 
 * */

inline uint8_t SpiMasterXfrByte(uint8_t byte)
{
    /** SpiMasterXfrByte behavior:\n 
      * - selects the SPI slave\n 
      * - loads SPI data reg with the byte to send\n 
      * - waits until the transfer is done by reading the SPI Interrupt Flag\n 
      * - unselects the SPI slave\n 
      * - clears the SPI Interrupt Flag by reading the SPI data reg\n 
      * - returns the byte in the SPI data reg\n 
      * */

    // select the SPI Slave
    ClearBit(Spi_PortOutput, Spi_Ss);

    // load data reg with byte to send
    *Spi_SPDR = byte;

    // wait for SPI transmission to complete
    while(BitIsClear(Spi_SPSR, Spi_InterruptFlag));

    // unselect the SPI Slave
    SetBit(Spi_PortOutput, Spi_Ss);

    // Clear the Spi_InterruptFlag by
    // reading Spi_InterruptFlag while it is set
    // then reading Spi_SPDR
    return *Spi_SPDR;
}

// ---API functions that call fakes when testing---
//
#ifdef USE_FAKES
#define ClearSpiInterruptFlag ClearSpiInterruptFlag_fake
#endif
inline void SpiMasterInit(void)
{
    /** SpiMasterInit behavior:\n 
      * - idles SlaveSelect high\n 
      * - makes SlaveSelect an output\n 
      * - makes Miso an input\n 
      * - enables pullup on Miso\n 
      * - makes DataReady an input\n 
      * - enables pullup on DataReady\n 
      * - makes Mosi an output\n 
      * - makes Sck an output\n 
      * - makes this MCU the SPI Master\n 
      * - sets SPI Clock to 10MHz ext osc divided by 8\n 
      * - enables the SPI hardware module\n 
      * - clears SPI interrupt flag\n 
      * */

    // Pin Ss: HIGH
    SetBit(Spi_PortOutput, Spi_Ss);
    // Pin Ss: output
    SetBit(Spi_PortDirection, Spi_Ss);

    // Pin Miso: input
    ClearBit(Spi_PortDirection, Spi_Miso);
    // Pin Miso: enable pullup
    SetBit(Spi_PortOutput, Spi_Miso);

    // Pin DataReady: input
    ClearBit(Spi_PortDirection, Spi_DataReady);
    // Pin DataReady: enable pullup
    SetBit(Spi_PortOutput, Spi_DataReady);

    // Pin Mosi: output
    SetBit(Spi_PortDirection, Spi_Mosi);

    // Pin Sck: output
    SetBit(Spi_PortDirection, Spi_Sck);

    // Be the SPI Master
    SetBit(Spi_SPCR, Spi_MasterSlaveSelect);

    // Set SPI clock to 1.25MHz
    SetBit  (Spi_SPCR, Spi_ClockBit0);
    ClearBit(Spi_SPCR, Spi_ClockBit1);
    SetBit  (Spi_SPSR, Spi_DoubleClock);

    _EnableSpiModule();

    ClearSpiInterruptFlag();
}
#ifdef USE_FAKES
#undef ClearSpiInterruptFlag
#endif

#endif // _SPIMASTER_H
