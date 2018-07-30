#ifndef _SPI_SLAVE_H
#define _SPI_SLAVE_H

#include <stdint.h>

/* =====[ Registers defined in -Hardware.h ]===== */
extern uint8_t volatile * const Spi_ddr;
extern uint8_t volatile * const Spi_spcr;
/* =====[ Pins defined in -Hardware.h ]===== */
extern uint8_t const Spi_Miso;
/* =====[ SPI control register bits ]===== */
extern uint8_t const Spi_Enable;
/* =====[ API ]===== */
void SpiSlaveInit(void);

#endif // _SPI_SLAVE_H
