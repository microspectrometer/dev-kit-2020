#ifndef _SPIMASTER_H
#define _SPIMASTER_H
#include <stdint.h>
#include "Spi.h"
#include "ReadWriteBits.h"

// ---API---
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
      * */

    // Pin Ss: output, high
    SetBit(Spi_PortOutput, Spi_Ss);
    SetBit(Spi_PortDirection, Spi_Ss);

    // Pin Miso: input, pullup
    ClearBit(Spi_PortDirection, Spi_Miso);
    SetBit(Spi_PortOutput, Spi_Miso);

    // Pin DataReady: input, pullup
    ClearBit(Spi_PortDirection, Spi_DataReady);
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

    // Enable SPI hardware module
    SetBit(Spi_SPCR, Spi_Enable);

    // Clear pending SPI interrupt
    /* ClearSpiInterruptFlag(); */
}

#endif // _SPIMASTER_H
