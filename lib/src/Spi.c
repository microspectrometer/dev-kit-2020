#include "Spi.h"
#include "ReadWriteBits.h"

//
/* =====[ Spi Master ]===== */
//
//
// TODO: remove Open and Close from the public API
//
static void SpiMasterOpenSpi_Implementation(void)
{
    ClearBit(Spi_port, Spi_Ss);
}
void (*SpiMasterOpenSpi)(void) = SpiMasterOpenSpi_Implementation;
//
static void SpiMasterCloseSpi_Implementation(void)
{
    SetBit(Spi_port, Spi_Ss);
}
void (*SpiMasterCloseSpi)(void) = SpiMasterCloseSpi_Implementation;
//
static bool SpiTransferIsDone_Implementation(void)
{
    return BitIsClear(Spi_spsr, Spi_InterruptFlag);
}
bool (*SpiTransferIsDone)(void) = SpiTransferIsDone_Implementation;

static void (*SlaveSelectIdleHigh)(void) = SpiMasterCloseSpi_Implementation;
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
static void SetClockRateToFoscDividedBy8(void)
{
    // SPI master and slave are both an ATmega328P with a 10MHz oscillator.
    // Use SCK = fosc/8 = 1.25MHz.
    SetBit  (Spi_spcr, Spi_ClockRateBit0);
    ClearBit(Spi_spcr, Spi_ClockRateBit1);
    SetBit(Spi_spsr, Spi_DoubleClockRate);
}
static void EnableSpi(void)
{
    SetBit(Spi_spcr, Spi_Enable);
}
void SpiMasterInit(void)
{
    SlaveSelectIdleHigh();
    SetSlaveSelectAsOutput();  // pin-direction is user-defined
    SetMosiAsOutput();         // pin-direction is user-defined
    SetSckAsOutput();          // pin-direction is user-defined
    MakeMeTheMaster();
    SetClockRateToFoscDividedBy8();  // hardcode the SPI clock rate at 1.25MHz
    EnableSpi();
}
void SpiMasterWrite(uint8_t byte_to_send)
{
    SpiMasterOpenSpi();
    *Spi_spdr = byte_to_send;  // load tx buffer and start SPI transmission
    while (!SpiTransferIsDone()) ;
    SpiMasterCloseSpi();
}

//
/* =====[ Spi Slave ]===== */
//
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

