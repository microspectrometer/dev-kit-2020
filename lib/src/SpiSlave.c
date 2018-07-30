#include "SpiSlave.h"
#include "ReadWriteBits.h"

void SpiSlaveInit(void)
{
    /* =====[ Configure user-defined pins in software. ]===== */
    // Make Spi_Miso an output pin.
    SetBit(Spi_ddr, Spi_Miso);

    // Enable SPI.
    SetBit(Spi_spcr, Spi_Enable);

    /* // Enable SPI interrupt. */
    /* SetBit(Spi_spcr, Spi_InterruptEnable); */
}

