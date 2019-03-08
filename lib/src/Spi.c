/** \file */
#include "Spi.h"
#include "ReadWriteBits.h"
/* #include "DebugLeds.h"          // controls the 4 debug LEDs */
#include "BiColorLed.h"
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
uint16_t BytesComing(spi_BytesComing_s response_size);
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
/* Define command functions in jump table */
extern uint8_t const status_led1;
extern uint8_t const status_led2;
extern uint8_t const status_led3;
extern uint8_t const status_led4;
void SensorLed1Red(void)
{
    BiColorLedRed(status_led1);
    SpiSlaveWrite_StatusOk(SensorLed1Red_key);
}
void SensorLed2Red(void)
{
    BiColorLedRed(status_led2);
    SpiSlaveWrite_StatusOk(SensorLed2Red_key);
}
void SensorLed3Red(void)
{
    BiColorLedRed(status_led3);
    SpiSlaveWrite_StatusOk(SensorLed3Red_key);
}
void SensorLed4Red(void)
{
    BiColorLedRed(status_led4);
    SpiSlaveWrite_StatusOk(SensorLed4Red_key);
}

void SensorLed1Green(void)
{
    BiColorLedGreen(status_led1);
    SpiSlaveWrite_StatusOk(SensorLed1Green_key);
}
void SensorLed2Green(void)
{
    BiColorLedGreen(status_led2);
    SpiSlaveWrite_StatusOk(SensorLed2Green_key);
}
void SensorLed3Green(void)
{
    BiColorLedGreen(status_led3);
    SpiSlaveWrite_StatusOk(SensorLed3Green_key);
}
void SensorLed4Green(void)
{
    BiColorLedGreen(status_led4);
    SpiSlaveWrite_StatusOk(SensorLed4Green_key);
}

void SensorCfgLis(void)
{
    /* TODO: left off here */
    ;// get 4-byte arg from bridge
    /* Fake receiving a valid `cfg`. */
    uint8_t const valid_cfg[] = {0x0F, 0xFF, 0xFF, 0xF9};
    LisWriteCfg(valid_cfg);
    // LisWriteCfg must handle the StatusOk since it follow that with
    // the updated cfg.
}

/* Define a named key for each function (`FooBar_key` is the key for `FooBar`) */
sensor_cmd_key const SensorLed1Green_key = 0;
sensor_cmd_key const SensorLed1Red_key = 1;
sensor_cmd_key const SensorLed2Green_key = 2;
sensor_cmd_key const SensorLed2Red_key = 3;
sensor_cmd_key const SensorLed3Green_key = 4;
sensor_cmd_key const SensorLed3Red_key = 5;
sensor_cmd_key const SensorLed4Green_key = 6;
sensor_cmd_key const SensorLed4Red_key = 7;
/* TODO: left off here */
sensor_cmd_key const SensorCfgLis_key = 8;
SensorCmd* LookupSensorCmd(sensor_cmd_key const key) {
    /* pf is an array of pointers to SensorCmd functions */
    /* pf lives in static memory, not on the `LookupSensorCmd` stack frame */
    static SensorCmd* const pf[] = {
        SensorLed1Green,
        SensorLed1Red,
        SensorLed2Green,
        SensorLed2Red,
        SensorLed3Green,
        SensorLed3Red,
        SensorLed4Green,
        SensorLed4Red,
        SensorCfgLis
        };
    /* Return func ptr. Prevent attempts at out-of-bounds access. */
    if (key < sizeof(pf)/sizeof(*pf))   return pf[key];
    /* Out of bounds keys return a NULL pointer. */
    else return NULL;
    /* Up to caller to check for NULL and take appropriate action. */
    /* Recommended action: tell SpiMaster the command was not recognized. */
}
/* SpiSlaveSendBytes has been unit-tested. No need to unit test this. */
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd)
{
                             // | nbytes  | data           |
    uint8_t const StatusOk[] = {0x00, 0x02, 0x00, valid_cmd };
    SpiSlaveSendBytes(StatusOk,4);
}
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd)
{                                  // | nbytes  |   data          |
    uint8_t const StatusInvalid[] = { 0x00, 0x02, 0xFF, invalid_cmd };
    SpiSlaveSendBytes(StatusInvalid,4);
}
/* --------------------------------------------------------------------------------------- */

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
    // After SPI is enabled, the alternate port function (SPI MISO) takes
    // control of the PORT value.
    // MISO is only driven hard high or hard low when:
    // - a SPI tranfer is in progress
    // - SPI is disabled
    // When not driven hard, the SPI module makes MISO a pull-up.
    SetMisoAsOutput();         // pin-direction is user-defined
    EnableSpi();
    /* ------------------------ */
    /* | Added on 2019-03-04: | */
    /* Drive Spi_Miso pin low when the SPI module is disabled. */
    /* See SpiSlaveSignalDataIsReady for explanation. */
    ClearBit(Spi_port, Spi_Miso);
    /* ------------------------ */
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
static void SpiSlaveSignalDataIsReady_Implementation(void)
{

    /* Prepare to output a low. */
    ClearBit(Spi_port, Spi_Miso);

    /* Make Spi_Miso a general purpose I/O pin. MISO goes low. */
    DisableSpi();

    /* Should I drive the pin high before re-enabling SPI? */
    /* ? SetBit(Spi_port, Spi_Miso); */
    /* If I do this, I think the SPI master must add a small delay */
    /* after seeing MISO go high to be sure the SPI slave is ready. */

    /* Make Spi_Miso the SPI Slave Output pin. MISO slowly rises up. */
    EnableSpi();
}
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
        while ( !SpiTransferIsDone() );
        // Added 2018-08-23: I missed this before
        SpiClearFlagTransferIsDone(); // TODO: add unit tests for this
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

