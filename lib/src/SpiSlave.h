#ifndef _SPI_SLAVE_H
#define _SPI_SLAVE_H

#include <stdint.h>

/* =====[ Registers defined in -Hardware.h ]===== */
extern uint8_t volatile * const Spi_ddr;
extern uint8_t volatile * const Spi_spcr;  // SPI control register
extern uint8_t volatile * const Spi_spsr;  // SPI status register
/* =====[ Pins defined in -Hardware.h ]===== */
extern uint8_t const Spi_Miso;
/* =====[ SPI control register bits ]===== */
extern uint8_t const Spi_Enable;
extern uint8_t const Spi_InterruptEnable;
/* =====[ SPI status register bits ]===== */
extern uint8_t const Spi_InterruptFlag;
/* =====[ API ]===== */
void SpiSlaveInit(void);

#endif // _SPI_SLAVE_H
