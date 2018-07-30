#ifndef _SPI_MASTER_H
#define _SPI_MASTER_H

#include <stdint.h>

/* =====[ Registers defined in -Hardware.h ]===== */
extern uint8_t volatile * const Spi_ddr;
extern uint8_t volatile * const Spi_port;
/* extern uint8_t volatile * const Spi_pin; */
extern uint8_t volatile * const Spi_spcr;
extern uint8_t volatile * const Spi_spsr;

/* =====[ Pins defined in -Hardware.h ]===== */
extern uint8_t const Spi_Ss;
extern uint8_t const Spi_Mosi;
extern uint8_t const Spi_Sck;

/* =====[ SPI control register bits ]===== */
extern uint8_t const Spi_MasterSlaveSelect;
extern uint8_t const Spi_ClockRateBit0;
extern uint8_t const Spi_ClockRateBit1;
extern uint8_t const Spi_Enable;

/* =====[ SPI status register bits ]===== */
extern uint8_t const Spi_DoubleClockRate;

/* =====[ API ]===== */
void SpiMasterInit(void);

#endif // _SPI_MASTER_H
