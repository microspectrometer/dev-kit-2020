/** \file */
#include "Spi.h"
#include "ReadWriteBits.h"
/* #include "DebugLeds.h"          // controls the 4 debug LEDs */
/* #include "BiColorLed.h" */
#include "stdlib.h" // defines NULL

/* ---------------------------------------------------- */
/* | 2019-03-04 WIP: inline version of SpiMasterWrite | */
/* ---------------------------------------------------- */
void SpiMasterWriteN_NoInlineHelpers(uint8_t const * bytes, uint8_t const nbytes);
uint8_t SpiMasterReadByte(void); void SpiMasterWriteByte(uint8_t const byte);
/* =====[ Helpers for ReadByte and WriteByte ]===== */
void WaitUntilSpiWriteIsDone(void);
void OpenSpiSlave(void);
void CloseSpiSlave(void);
void SpiLaunchByte(uint8_t const byte);
/* ---------------------------------------------------- */

/* --------------------------------------------------------------------------------------- */
/* | 2019-03-03 WIP: New SpiSlave API functionality for robust multi-byte communication. | */
/* --------------------------------------------------------------------------------------- */
/* bool SpiSlaveRead_OneByte(uint8_t *read_buffer) */
/* { */
/*     bool have_byte = SpiTransferIsDone(); */
/*     return have_byte; */
/* } */
void SpiMasterWaitForSlaveReady(void);
/* =====[ Helpers to `SpiMasterWaitForSlaveReady` ]===== */
bool SpiSlaveShowsDataReady(void);
bool IsSpiSlaveReadyToSend(void);

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

void SpiClearFlagTransferIsDone(void)
{
    // Clear the Spi_Interrupt flag by
    // reading Spi_InterruptFlag while it is set
    // then reading SPDR
    *Spi_spdr;  // this serves as a throwaway access
}

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

// Enable MISO pull-up on the SpiMaster only!
// The pull-up is achieved by setting the port bit that matches the MISO pin.
// When the slave disables SPI to drive MISO, it needs this pin to go low.
//
/* TODO: rename this function to make it clear it is *only* for the SpiMaster. */
static void SetMisoAsPullupInput(void) // For SpiMaster only!
{
    ClearBit(Spi_ddr, Spi_Miso);    // make it an input
    SetBit(Spi_port, Spi_Miso);     // enable pull-up
}
void SetDataReadyAsPullupInput(void) // For SpiMaster only!
{
    ClearBit(Spi_ddr, Spi_DataReady); // make it an input
    SetBit(Spi_port, Spi_DataReady); // enable pull-up
}
void SetDataReadyAsOutputIdlesHigh(void) // For SpiSlave only!
{
    SetBit(Spi_port, Spi_DataReady); // idle HIGH
    SetBit(Spi_ddr, Spi_DataReady); // make it an output pin
}

void SpiMasterInit(void)
{
    SlaveSelectIdleHigh();
    SetMisoAsPullupInput(); // protect against false SpiResponseIsReady signals
    // DataReady LOW signals Master that Slave is ready to send a byte of data
    SetDataReadyAsPullupInput(); // Added 2019-10-18:
    SetSlaveSelectAsOutput();  // pin-direction is user-defined
    SetMosiAsOutput();         // pin-direction is user-defined
    SetSckAsOutput();          // pin-direction is user-defined
    MakeMeTheMaster();
    SetClockRateToFoscDividedBy8();  // hardcode the SPI clock rate at 1.25MHz
    EnableSpi();
    ClearPendingSpiInterrupt(); // access SPI status reg and SPI data reg
    // Master does *not* use interrupts to send and receive bytes over SPI.
}
void SpiMasterDisable(void)
{
    // Release control of all SPI lines to let another SPI master take control.
    /* Undo SlaveSelectIdleHigh(); */
    ClearBit(Spi_ddr, Spi_Ss); ClearBit(Spi_port, Spi_Ss);
    /* SetMisoAsPullupInput(); // protect against false SpiResponseIsReady signals */
    /* SetSlaveSelectAsOutput();  // pin-direction is user-defined */
    /* Undo SetMosiAsOutput();         // pin-direction is user-defined */
    ClearBit(Spi_ddr, Spi_Mosi);
    /* Undo SetSckAsOutput();          // pin-direction is user-defined */
    ClearBit(Spi_ddr, Spi_Sck);
    /* MakeMeTheMaster(); */
    /* SetClockRateToFoscDividedBy8();  // hardcode the SPI clock rate at 1.25MHz */
    /* Undo EnableSpi(); */
    DisableSpi();
    /* ClearPendingSpiInterrupt(); */
}

void SpiMasterWrite(uint8_t byte_to_send)
{
    SpiMasterOpenSpi();
    *Spi_spdr = byte_to_send;  // load tx buffer and start SPI transmission
    while (!SpiTransferIsDone()) ;
    // You need to now access SPDR to clear the flag!
    // I do not think the reads are destructive.
    // Add code here to clear the flag:
    SpiClearFlagTransferIsDone();  // TODO: add unit tests for this
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
    // Slave outputs LOW on DataReady to signal Master there is data to read.
    SetDataReadyAsOutputIdlesHigh();
    // TODO: do not drive MISO, that's why we have DataReady
    SetMisoAsOutput(); // pin-direction is user-defined
    EnableSpi();
    ClearPendingSpiInterrupt(); // access SPI status reg and SPI data reg
    // Slave uses interrupts to send and receive bytes over SPI.
    SpiEnableInterrupt();
}
/* void oldSpiSlaveInit(void) */
/* { */
/*     // After SPI is enabled, the alternate port function (SPI MISO) takes */
/*     // control of the PORT value. */
/*     // MISO is only driven hard high or hard low when: */
/*     // - a SPI tranfer is in progress */
/*     // - SPI is disabled */
/*     // When not driven hard, the SPI module makes MISO a pull-up. */
/*     SetMisoAsOutput();         // pin-direction is user-defined */
/*     EnableSpi(); */
/*     /1* ------------------------ *1/ */
/*     /1* | Added on 2019-03-04: | *1/ */
/*     /1* Drive Spi_Miso pin low when the SPI module is disabled. *1/ */
/*     /1* See FourWire_SpiSlaveSignalDataIsReady for explanation. *1/ */
/*     ClearBit(Spi_port, Spi_Miso); */
/*     /1* ------------------------ *1/ */
/*     /1* ClearPendingSpiInterrupt(); *1/ */
/*     // Slave uses interrupts to receive incoming bytes over SPI. */
/*     SpiEnableInterrupt(); */
/*     // Slave disables interrupts for transmitting bytes over SPI. */
/*     // In client code, enable interrupt with: */
/*     /1* SetBit(Spi_spcr, Spi_InterruptEnable); // Enable SPI interrupt *1/ */
/*     // and disable with: */
/*     /1* ClearBit(Spi_spcr, Spi_InterruptEnable); // Disable SPI interrupt *1/ */
/* } */
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
static void SpiSlaveSignalDataIsReady_Implementation(void)
{

}
/* static void FourWire_SpiSlaveSignalDataIsReady_Implementation(void) */
/* { */

/*     /1* Prepare to output a low. *1/ */
/*     ClearBit(Spi_port, Spi_Miso); */

/*     /1* Make Spi_Miso a general purpose I/O pin. MISO goes low. *1/ */
/*     DisableSpi(); */

/*     /1* Should I drive the pin high before re-enabling SPI? *1/ */
/*     /1* ? SetBit(Spi_port, Spi_Miso); *1/ */
/*     /1* If I do this, I think the SPI master must add a small delay *1/ */
/*     /1* after seeing MISO go high to be sure the SPI slave is ready. *1/ */

/*     /1* Make Spi_Miso the SPI Slave Output pin. MISO slowly rises up. *1/ */
/*     EnableSpi(); */
/* } */
    /** `SpiSlave` signals *Data Ready* by driving pin `Spi_Miso` low.
     *  Attention: this is not the usual step waveform. Instead, pin `Spi_Miso`
     *  idles high, spikes low, then slowly rises back.
     *
     *  Here is how the *Data Ready* signal is implemented:
     *
     * 1. Disable the SPI module to restore general purpose I/O functionality.
     *
     * 2. Pin `Spi_Miso` is driven low because the PORT register has a 0 in this
     * bit. This is a sudden voltage step from high to low.
     *
     * 3. Enable the SPI module in preparation to receive a transmission from
     * the SPI master. This is a slow voltage rise from low to high because the
     * the capacitance on the SPI bus is charged by the small current available
     * from the power supply via the pull-up resistor.
     */
void (*SpiSlaveSignalDataIsReady)(void) = SpiSlaveSignalDataIsReady_Implementation;

uint8_t SpiSlaveRead(void)
{
    while( !SpiTransferIsDone() );
    return ReadSpiDataRegister();
}
void SpiSlaveSendBytes(uint8_t const *bytes, uint16_t const nbytes)
{
    uint16_t byte_index;
    for (byte_index = 0; byte_index < nbytes; byte_index++)
    {
        /* *Spi_spdr = bytes[byte_index]; */
        WriteSpiDataRegister(bytes[byte_index]);
        SpiSlaveSignalDataIsReady();
        /* =====[ OLD: wait for transfer to be done ]===== */
        while ( !SpiTransferIsDone() );
        // Added 2018-08-23: I missed this before
        SpiClearFlagTransferIsDone(); // TODO: add unit tests for this
        /* =====[ NEW: wait for transfer to be done in ISR ]===== */
        /* // This does not work in lib Spi because HasSpiData is an application global. */
        /* while (!HasSpiData); */
        /* HasSpiData = false; // ignore rx byte, clear the flag */
    }
    // When is it safe to load the next byte?
        // Just wait for the transmission to end.
        // The transmit buffer is single-buffered.
        // It cannot be written until the transmission finishes.
        // The receive buffer is double-buffered.
        // Old values are available up until the last bit of the next byte is
        // shifted in.
    // The SPI master waits for MISO to go low after every read.
    // This gaurantees the next byte of data is ready.
    // The SPI master does not have to release SlaveSelect, but it can.
    // SlaveSelect being low should not impact the slave's ability to disable
    // SPI and pull MISO low.
}

static void WriteSpiDataRegister_Implementation(uint8_t byte_to_write)
{
    *Spi_spdr = byte_to_write;
}
void (*WriteSpiDataRegister)(uint8_t) = WriteSpiDataRegister_Implementation;

