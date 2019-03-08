/** \file */
#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>
#include <stdbool.h>
#include "ReadWriteBits.h"

/* =====[ Hardware dependencies to be resolved in Spi-Hardware.h ]===== */
/* ---Pin Direction and I/O Registers--- */
extern uint8_t volatile * const Spi_ddr;
extern uint8_t volatile * const Spi_port;
extern uint8_t volatile * const Spi_pin;
/* ---Pin Names--- */
extern uint8_t const Spi_Ss;    // used in software for master
extern uint8_t const Spi_Mosi;  // used in software for master
extern uint8_t const Spi_Sck;   // used in software for master
extern uint8_t const Spi_Miso;  // used in software for slave
/* ---SPI Registers--- */
extern uint8_t volatile * const Spi_spcr;  // SPI control register
extern uint8_t volatile * const Spi_spsr;  // SPI status register
extern uint8_t volatile * const Spi_spdr;  // SPI data register
/* ---SPI control register bit names--- */
extern uint8_t const Spi_MasterSlaveSelect;
extern uint8_t const Spi_ClockRateBit0;
extern uint8_t const Spi_ClockRateBit1;
extern uint8_t const Spi_Enable;
extern uint8_t const Spi_InterruptEnable;   // used in software for slave
/* ---SPI status register bit names--- */
extern uint8_t const Spi_DoubleClockRate;
extern uint8_t const Spi_InterruptFlag;

/* =====[ AVR asm macros ]===== */
// TODO: replace the next two lines of code with a #include "AvrAsmMacros.h"
// and put the #include in the Spi.c lib file.
extern void (*GlobalInterruptEnable)(void);
extern void (*GlobalInterruptDisable)(void);
/* #ifndef _AVR_INTERRUPT_H_ */
/* #define cli() */
/* #define sei() */
/* #endif // _AVR_INTERRUPT_H_ */

/* =====[ Old stuff begins here ]===== */

/* =====[ SPI Master API ]===== */
void SpiMasterInit(void);     // to use the Usb-Spi bridge as intended
void SpiMasterDisable(void);  // to let another Spi Master take over the SPI bus
#define MacroSpiMasterOpenSpi()  ClearBit(Spi_port, Spi_Ss)
#define MacroSpiMasterCloseSpi() SetBit(Spi_port, Spi_Ss)
#define MacroSpiTransferIsDone() BitIsSet(Spi_spsr, Spi_InterruptFlag)
// TODO: this macro is not used. Erase it? Or is its original function in need
// of replacement with the macro?
/* #define MacroClearPendingSpiInterrupt() do { \ */
/*     *Spi_spsr; *Spi_spdr; \ */
/* } while (0) */
void SpiMasterWrite(uint8_t byte_to_send);
#define MacroSpiMasterWrite(byte) do { \
    MacroSpiMasterOpenSpi(); \
    *Spi_spdr = byte; \
    while ( !MacroSpiTransferIsDone() ); \
    /* Do a throw-away access of reg 0x2e (SPDR) to clear the flag */ \
    *Spi_spdr; \
    MacroSpiMasterCloseSpi(); \
    /* Cannot put the delay here or master misses MISO rising */ \
    /* Delay3CpuCyclesPerTick(SpiWriteDelayTicks); \ */ \
} while (0)
// Writes followed by a short delay to let the slave catch up.
// Even with only a few ticks, performance is OK.
// 50 ticks really guards against commands ever getting garbled.
#define SpiWriteDelayTicks 50
#define MacroSpiMasterWriteAndDelay(byte) do { \
    MacroSpiMasterWrite(byte); \
    Delay3CpuCyclesPerTick(SpiWriteDelayTicks); \
} while (0)
uint8_t SpiMasterRead(void);
#define MacroReadSpiDataRegister() (*Spi_spdr)
/* ---Macro version of SpiMasterRead--- */
// This is not a macro because I do not know how to return a value.
// Inline `SpiMasterRead` by replacing with the following two lines:
/* MacroSpiMasterWrite(slave_ignore); */
/* {client_code_var} = MacroReadSpiDataRegister(); */
extern bool (*SpiResponseIsReady)(void);
#define MacroSpiResponseIsReady() BitIsClear(Spi_pin, Spi_Miso)
void SpiMasterWaitForResponse(void);
#define MacroSpiMasterWaitForResponse() do { \
    /* =====[ wait for master pull-up to pull MISO High ]===== */ \
    /* right after a transmission, MISO has to recharge */ \
    /* on the next board, put an explicit 10k pull-up on MISO */ \
    while(  MacroSpiResponseIsReady() ); \
    /* =====[ wait for slave to pull MISO Low ]===== */ \
    while( !MacroSpiResponseIsReady() ); \
    /* =====[ wait for slave to pull MISO High ]===== */ \
    while(  MacroSpiResponseIsReady() ); \
} while (0)
// The following TODO is obsolete. Instead of outputting a PWM signal to set the
// led current, Osram is making the LED driver a SPI slave.
// ~TODO~: create a special version of MacroSpiMasterWaitForResponse that is
// MacroDriveLedWhileSpiMasterWaitsForResponse
    // ~TODO~: put software PWM here for Osram to control the current in their
    // LED The PWM signal connects to the Dim pin on their OnSemi board.

/* =====[ Not part of the API. Exposed for testing SPI Master only. ]===== */
extern void (*SpiMasterOpenSpi)(void);
extern void (*SpiMasterCloseSpi)(void);
extern bool (*SpiTransferIsDone)(void);
void SpiClearFlagTransferIsDone(void);  // 2018-08-23 - I missed this before


/* =====[ SPI Slave API ]===== */
void SpiSlaveInit(void);
uint8_t SpiSlaveRead(void);
extern void (*SpiSlaveSignalDataIsReady)(void);
extern uint8_t const slave_ignore;      // slave ignores cmd `slave_ignore`
extern uint8_t const test_unknown_cmd;  // tests slave response to unknown cmd
void SpiSlaveSendBytes(uint8_t const *bytes, uint16_t const nbytes);
#define MacroWriteSpiDataRegister(byte) (*Spi_spdr = byte)
#define MacroSpiSlaveSignalDataIsReady() do { \
    ClearBit(Spi_port, Spi_Miso); \
    MacroDisableSpi(); \
    /* ---10 ticks is determined empirically to be the minimum delay--- */ \
    /* Delay3CpuCyclesPerTick(10); \ */ \
    /* ---But that only works *when the programmer is connected*--- */ \
    Delay3CpuCyclesPerTick(10); \
    /* SetBit(Spi_port, Spi_Miso); \ */ \
    MacroEnableSpi(); \
} while (0)
#define MacroSpiSlaveSendBytes(byte_array, nbytes) do { \
    for (uint16_t byte_index = 0; byte_index < nbytes; byte_index++) \
    { \
        MacroWriteSpiDataRegister(byte_array[byte_index]); \
        MacroSpiSlaveSignalDataIsReady(); \
        /* SpiSlaveSignalDataIsReady(); \ */ \
        while ( !MacroSpiTransferIsDone() ); \
        /* Do a throw-away access of reg 0x2e (SPDR) to clear the flag */ \
        *Spi_spdr; \
    } \
} while (0)

/* =====[ Plumbing for all AVR SPI devices, exposed for testing ]===== */
extern uint8_t (*ReadSpiDataRegister)(void);
extern void (*WriteSpiDataRegister)(uint8_t);
void SpiEnableInterrupt(void);
extern void (*ClearPendingSpiInterrupt)(void);
extern uint8_t (*ReadSpiStatusRegister)(void);
extern void (*DisableSpi)(void);
#define MacroDisableSpi() ClearBit(Spi_spcr, Spi_Enable)
extern void (*EnableSpi)(void);
#define MacroEnableSpi() SetBit(Spi_spcr, Spi_Enable)

/* =====[ New stuff begins here ]===== */

/* ---------------------------------------------------- */
/* | 2019-03-04 WIP: inline version of SpiMasterWrite | */
/* ---------------------------------------------------- */
inline void WaitUntilSpiWriteIsDone(void)
{
    /** Wait until the SPI transmission is done.
     *  Throwaway the received byte.
     */
    /*
     * Expect this is inlined. Expect this compiles as:
     * loop:
     * `in reg,SPSR`
     * `sbrs reg,SPIF`
     * `rjmp loop`
     * `ret` */
    while (BitIsClear(Spi_spsr, Spi_InterruptFlag));
    /* Do a throw-away read of the SPI data register to
     * clear the SPI interrupt flag. */
    *Spi_spdr;
}
/* TODO: pass port and pin as arguments for future multi-slave system. */
inline void OpenSpiSlave(void)
{
    /** Open communication with SPI slave. */
    /* Expect `ClearBit` is compiled inline as a single `cbi` instruction. */
    ClearBit(Spi_port, Spi_Ss);
}
inline void CloseSpiSlave(void)
{
    /** Close communication with SPI slave. */
    /* Expect `SetBit` is compiled inline as a single `sbi` instruction. */
    SetBit(Spi_port, Spi_Ss);
}
inline void SpiLaunchByte(uint8_t const byte)
{
    /** Write the SPI data register with the byte to send.
     *  Writing to the SPI data register starts the SPI transmission. */
    *Spi_spdr = byte;
}
inline uint8_t SpiMasterReadByte(void)
{
    /** Read one byte from the `SpiSlave`. */
    OpenSpiSlave();
    SpiLaunchByte(slave_ignore);
    WaitUntilSpiWriteIsDone();
    CloseSpiSlave();
    /* TODO: inline this */
    /* ReadSpiDataRegister */
    return *Spi_spdr;
}
/* Every SpiSlave response starts with two bytes indicating how many more bytes are coming. */
typedef struct {
    uint8_t msb;
    uint8_t lsb;
} spi_BytesComing_s;
inline uint16_t BytesComing(spi_BytesComing_s response_size)
{
    return response_size.msb<<8 | response_size.lsb;
}
inline void SpiMasterWriteByte(uint8_t const byte)
{
    /** Write one byte to the `SpiSlave`. */
    OpenSpiSlave();
    SpiLaunchByte(byte);
    WaitUntilSpiWriteIsDone();
    CloseSpiSlave();

    /* TODO: try sending many bytes to figure out if this is necessary! */
    /* ? Delay3CpuCyclesPerTick(50); */
    /* If I do need a delay here, then why don't I check for SPI slave is ready */
    /* signal instead of an open loop delay? */
    /* And let the caller handle where that is a delay or a `slave_ready` check. */

    /* Protocol has two scenarios: */
    /* 1. SpiMaster sends a cmd without args and expects a response. */
    /* 2. SpiMaster sends a cmd with args and expects a response. */
    /* SpiMaster releases SpiSlave and watches `data_ready` for the response. */
    /* When sending data *to* the slave, the master should watch for a `ready` */
    /* signal *after every byte sent*. This is the simplest solution. So there */
    /* is no *write N bytes*, only a *write byte*, and it is up to the caller */
    /* to perform a sequence of these *write byte* commands to achieve the */
    /* desired data flow. */
}
inline void SpiMasterWriteN_NoInlineHelpers(uint8_t const * bytes, uint8_t const nbytes)
{
    /** This version calls helpers via function pointer seams.
     *  Write nbytes to the `SpiSlave`.
     *  
     *  For each byte in bytes:
     *
     *  1. drive `Spi_Ss` low
     *  2. do 8-bits of SPI transmission
     *  3. drive `Spi_Ss` high
     *  ~~4. delay to give `SpiSlave` time to store the byte~~
     */
    uint8_t num_bytes_sent = 0;
    while (num_bytes_sent++ < nbytes)
    {
        ClearBit(Spi_port, Spi_Ss); // inline

        /* Load the transmit buffer to start the SPI transmission. */
        *Spi_spdr = *(bytes++);

        /* TODO: inline this */
        while (!SpiTransferIsDone()) ;

        /* TODO: inline this */
        SpiClearFlagTransferIsDone();

        SetBit(Spi_port, Spi_Ss); // inline

        /* To delay or not to delay */
        /* ? Delay3CpuCyclesPerTick(50); */
    }
}
inline bool SpiSlaveShowsDataReady(void)
{
    /** SpiSlave drives `Spi_Miso` low to signal to the SpiMaster
     * when it is ready for the next SPI transmission.
     */
    return BitIsClear(Spi_pin, Spi_Miso);
}
inline bool IsSpiSlaveReadyToSend(void)
{
    /** SpiSlave releases `Spi_Miso` and it slowly rises high via its pull-up. */
    return BitIsSet(Spi_pin, Spi_Miso);
}
inline void SpiMasterWaitForSlaveReady(void)
{
    /** Wait for the *Data Ready* signal from the SpiSlave. */
    /* Watch for SpiSlave to spike `Spi_Miso` low. */
    while( !SpiSlaveShowsDataReady() );
    /* Watch for SpiSlave to release `Spi_Miso` to the pull-up. */
    while( !IsSpiSlaveReadyToSend() );
}

/* ---------------------------------------------------- */

/* --------------------------------------------------------------------------------------- */
/* | 2019-03-03 WIP: New SpiSlave API functionality for robust multi-byte communication. | */
/* --------------------------------------------------------------------------------------- */
/* =====[ SPI Slave API ]===== */
/* Functions of type `SensorCmd` take nothing and return nothing. */
/* The *key* acts as the command since it is the command lookup. */
/* If the commands need additional parameters, they will read additional bytes. */
/* All functions in the lookup table must have the same signature, so commands that */
/* take return functions other than void-void need to go in a different jump table. */
typedef void (SensorCmd)(void);
/* Give tests of LookupSensorCmd access to names of functions in */
/* jump table to compare pointer values. */
SensorCmd SensorLed1Red;
SensorCmd SensorLed1Green;
/* SensorCmd spi_CfgLis; */
/* This is the datatype to use when calling LookupSpiCmd: */
typedef uint8_t sensor_cmd_key;  // SpiSlave jump-table dictionary uses 8-bit keys
/* Declare keys for callers of LookupCmd (values hidden in .c file) */
/* extern sensor_cmd_key const spi_LedRed_key; */
extern sensor_cmd_key const SensorLed1Red_key;
extern sensor_cmd_key const SensorLed1Green_key;
/* extern sensor_cmd_key const spi_CfgLis_key; */
/* LookupSensorCmd takes key from SpiMaster and returns the function pointer to call. */
SensorCmd* LookupSensorCmd(sensor_cmd_key const key);
/* report status to SpiMaster */
void SpiSlaveWrite_StatusOk(sensor_cmd_key valid_cmd);
void SpiSlaveWrite_StatusInvalid(sensor_cmd_key invalid_cmd);
// SpiSlaveWrite_StatusError
/* =====[ API to communicate ]===== */
// SpiSlaveRead_OneByte
/* --------------------------------------------------------------------------------------- */


#endif // _SPI_H
