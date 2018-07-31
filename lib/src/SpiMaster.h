#ifndef _SPI_MASTER_H
#define _SPI_MASTER_H

#include <stdint.h>
#include <stdbool.h>

/* =====[ API ]===== */
void SpiMasterInit(void);
void SpiMasterWrite(uint8_t byte_to_send);

/* =====[ Not part of the API. Exposed for testing only. ]===== */
extern void (*SpiMasterOpenSpi)(void);
extern void (*SpiMasterCloseSpi)(void);
extern bool (*SpiTransferIsDone)(void);

/* =====[ Hardware dependencies to be resolved in SpiMaster-Hardware.h ]===== */
/* ---Pin Direction and I/O Registers--- */
extern uint8_t volatile * const Spi_ddr;
extern uint8_t volatile * const Spi_port;
/* extern uint8_t volatile * const Spi_pin; */
/* ---Pin Names--- */
extern uint8_t const Spi_Ss;
extern uint8_t const Spi_Mosi;
extern uint8_t const Spi_Sck;
/* ---SPI Registers--- */
extern uint8_t volatile * const Spi_spcr;  // SPI control register
extern uint8_t volatile * const Spi_spsr;  // SPI status register
extern uint8_t volatile * const Spi_spdr;  // SPI data register
/* ---SPI control register bit names--- */
extern uint8_t const Spi_MasterSlaveSelect;
extern uint8_t const Spi_ClockRateBit0;
extern uint8_t const Spi_ClockRateBit1;
extern uint8_t const Spi_Enable;
/* ---SPI status register bit names--- */
extern uint8_t const Spi_DoubleClockRate;
extern uint8_t const Spi_InterruptFlag;


#endif // _SPI_MASTER_H
