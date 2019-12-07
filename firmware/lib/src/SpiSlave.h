#ifndef _SPISLAVE_H
#define _SPISLAVE_H
#include <stdint.h>
#include "ReadWriteBits.h"

typedef uint8_t const spi_pin; // bit index into i/o reg for an i/o pin
typedef uint8_t const spi_bit; // bit index into i/o reg
typedef uint8_t volatile * const spi_ptr; // i/o reg address

// I/O register definitions in Hardware.h for dependency on make target
extern spi_ptr Spi_ddr;
extern spi_ptr Spi_port;
extern spi_ptr Spi_spcr;
extern spi_ptr Spi_spdr;
extern spi_ptr Spi_spsr;

// pin definitions in Hardware.h for dependency on make target
extern spi_pin Spi_DataReady; // slave signal DR to master
extern spi_pin Spi_Miso; // master-in, slave-out
extern spi_bit Spi_Enable;
extern spi_bit Spi_InterruptEnable;

void SpiSlaveInit(void);
#endif // _SPISLAVE_H
