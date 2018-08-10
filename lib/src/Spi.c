#include "Spi.h"
#include "ReadWriteBits.h"

static void ClearPendingSpiInterrupt_Implementation(void)
{
    // Clear the SPI Interrupt Flag bit in the SPI Status Register.
    // Implementation:
    // Read registers SPSR and SPDR, in that order.
    //
    // Old implementation -- check how avr-gcc handles this:
    /* *Spi_spsr; */
    /* *Spi_spdr; */
    //
    // New implementation -- check how avr-gcc handles this:
    ReadSpiStatusRegister();// read and discard the value
    ReadSpiDataRegister();  // read and discard the value
}
void (*ClearPendingSpiInterrupt)(void) = ClearPendingSpiInterrupt_Implementation;
static uint8_t ReadSpiStatusRegister_Implementation(void)
{
    return *Spi_spsr;
}
uint8_t (*ReadSpiStatusRegister)(void) = ReadSpiStatusRegister_Implementation;
//
static void DisableSpi_Implementation(void)
{
    ClearBit(Spi_spcr, Spi_Enable);
}
void (*DisableSpi)(void) = DisableSpi_Implementation;
//
static void EnableSpi_Implementation(void)
{
    SetBit(Spi_spcr, Spi_Enable);
}
void (*EnableSpi)(void) = EnableSpi_Implementation;
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
    return BitIsSet(Spi_spsr, Spi_InterruptFlag);
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
//
// I cannot do this.
// I expected it to just enable the pull-up.
// Instead, it prevents the SPI master from ever receiving `DataIsReady`.
// It behaves like MISO is always high, but I have not measured it.
//
static void SetMisoAsPullupInput(void)
{
    ClearBit(Spi_ddr, Spi_Miso);    // make it an input
    SetBit(Spi_port, Spi_Miso);     // enable pull-up
}
void SpiMasterInit(void)
{
    SlaveSelectIdleHigh();
    SetMisoAsPullupInput(); // protect against false SpiResponseIsReady signals
    SetSlaveSelectAsOutput();  // pin-direction is user-defined
    SetMosiAsOutput();         // pin-direction is user-defined
    SetSckAsOutput();          // pin-direction is user-defined
    MakeMeTheMaster();
    SetClockRateToFoscDividedBy8();  // hardcode the SPI clock rate at 1.25MHz
    EnableSpi();
    ClearPendingSpiInterrupt();
}
void SpiMasterWrite(uint8_t byte_to_send)
{
    SpiMasterOpenSpi();
    *Spi_spdr = byte_to_send;  // load tx buffer and start SPI transmission
    while (!SpiTransferIsDone()) ;
    SpiMasterCloseSpi();
}
static uint8_t ReadSpiDataRegister_Implementation(void)
{
    return *Spi_spdr;
}
uint8_t (*ReadSpiDataRegister)(void) = ReadSpiDataRegister_Implementation;
uint8_t const slave_ignore     = 0xFF;  // slave ignores 0xFF from the master
uint8_t const test_unknown_cmd = 0xFE;  // guarantee 0xFE to be an unknown cmd
uint8_t SpiMasterRead(void)
{
    SpiMasterWrite(slave_ignore);
    return ReadSpiDataRegister();
}
static bool SpiResponseIsReady_Implementation(void)
{
    return BitIsClear(Spi_pin, Spi_Miso);
}
bool (*SpiResponseIsReady)(void) = SpiResponseIsReady_Implementation;
void SpiMasterWaitForResponse(void)
{
    while( !SpiResponseIsReady() );
}


//
/* =====[ Spi Slave ]===== */
//
static void SetMisoAsOutput(void)
{
    SetBit(Spi_ddr, Spi_Miso);
}
void SpiSlaveInit(void)
{
    // After SPI is enabled, the alternate port function (SPI MISO) takes
    // control of the PORT value.
    // MISO is only driven hard high or hard low when:
    // - a SPI tranfer is in progress
    // - SPI is disabled
    // When not driven hard, the SPI module makes MISO a pull-up.
    SetMisoAsOutput();         // pin-direction is user-defined
    EnableSpi();
    ClearPendingSpiInterrupt();
}
static void EnableTransferCompleteInterrupt(void)
{
    SetBit(Spi_spcr, Spi_InterruptEnable);
}
void SpiEnableInterrupt(void)
{
    GlobalInterruptDisable();
    ClearPendingSpiInterrupt();
    EnableTransferCompleteInterrupt();
    GlobalInterruptEnable();
}
void SpiSlaveSignalDataIsReady(void)
{
    ClearBit(Spi_port, Spi_Miso);
    DisableSpi();
    EnableSpi();
}
uint8_t SpiSlaveRead(void)
{
    while( !SpiTransferIsDone() );
    return ReadSpiDataRegister();
}
