#include "SpiMaster.h"
#include "ReadWriteBits.h"

void SpiMasterInit(void)
{
    /* =====[ Configure user-defined pins in software. ]===== */
    // Spi_Ss idles high.
    SetBit(Spi_port, Spi_Ss);
    // Make Spi_Ss an output pin.
    SetBit(Spi_ddr, Spi_Ss);
    //
    // Make Spi_Mosi an output pin.
    SetBit(Spi_ddr, Spi_Mosi);
    //
    // Make Spi_Sck an output pin.
    SetBit(Spi_ddr, Spi_Sck);

    // Make this the master.
    SetBit(Spi_spcr, Spi_MasterSlaveSelect);

    /* =====[ Hardcode the clock rate for this project. ]===== */
    // SPI master and slave are both an ATmega328P with a 10MHz oscillator.
    // Use SCK = fosc/8 = 1.25MHz.
    SetBit  (Spi_spcr, Spi_ClockRateBit0);
    ClearBit(Spi_spcr, Spi_ClockRateBit1);
    SetBit(Spi_spsr, Spi_DoubleClockRate);

    // Enable SPI.
    SetBit(Spi_spcr, Spi_Enable);
}
void SpiMasterOpenSpi(void)
{
    ClearBit(Spi_port, Spi_Ss);
}
void SpiMasterCloseSpi(void)
{
    SetBit(Spi_port, Spi_Ss);
}
