#include "SpiMaster.h"
#include "ReadWriteBits.h"

static void (*SlaveSelectIdleHigh)(void) = SpiMasterCloseSpi;
static void SetSlaveSelectAsOutput(void)
{
    SetBit(Spi_ddr, Spi_Ss);
}
static void SetMosiAsOutput(void)
{
    SetBit(Spi_ddr, Spi_Mosi);
}
static void SetSckAsOutput(void)
{
    SetBit(Spi_ddr, Spi_Sck);
}
static void MakeMeTheMaster(void)
{
    SetBit(Spi_spcr, Spi_MasterSlaveSelect);
}
void SpiMasterInit(void)
{
    SlaveSelectIdleHigh();
    SetSlaveSelectAsOutput();  // pin-direction is user-defined
    SetMosiAsOutput();         // pin-direction is user-defined
    SetSckAsOutput();          // pin-direction is user-defined
    MakeMeTheMaster();

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
