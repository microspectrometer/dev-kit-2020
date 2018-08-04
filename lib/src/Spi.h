#ifndef _SPI_H
#define _SPI_H

#include <stdint.h>
#include <stdbool.h>

/* =====[ AVR asm macros ]===== */
extern void (*GlobalInterruptEnable)(void);
extern void (*GlobalInterruptDisable)(void);
/* #ifndef _AVR_INTERRUPT_H_ */
/* #define cli() */
/* #define sei() */
/* #endif // _AVR_INTERRUPT_H_ */

/* =====[ SPI Master API ]===== */
void SpiMasterInit(void);
void SpiMasterWrite(uint8_t byte_to_send);
uint8_t SpiMasterRead(void);
extern bool (*SpiResponseIsReady)(void);
void SpiMasterWaitForResponse(void);
/* =====[ Not part of the API. Exposed for testing SPI Master only. ]===== */
extern void (*SpiMasterOpenSpi)(void);
extern void (*SpiMasterCloseSpi)(void);
extern bool (*SpiTransferIsDone)(void);
extern uint8_t (*ReadSpiDataRegister)(void);

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

/* =====[ SPI Slave API ]===== */
void SpiSlaveInit(void);
void SpiEnableInterrupt(void);
uint8_t SpiSlaveRead(void);
void SpiSlaveSignalDataIsReady(void);
void SpiSlaveSignalDataIsNotReady(void);

#endif // _SPI_H
