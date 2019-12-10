#ifndef _SPISLAVE_H
#define _SPISLAVE_H
#include <stdint.h>
#include "ReadWriteBits.h"
//---Hardware types: register addresses, pin numbers, bit numbers---
typedef uint8_t volatile * const spi_ptr; // i/o reg address
typedef uint8_t const spi_pin; // bit index into i/o reg for an i/o pin
typedef uint8_t const spi_bit; // bit index into i/o reg
// Register address, pin number, and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/Hardware.h
// ---Registers---
extern spi_ptr Spi_ddr;
extern spi_ptr Spi_port;
extern spi_ptr Spi_spcr;
extern spi_ptr Spi_spdr;
extern spi_ptr Spi_spsr;
// ---Pins---
extern spi_pin Spi_DataReady; // slave signal DR to master
extern spi_pin Spi_Miso; // master-in, slave-out
// ---Bits---
extern spi_bit Spi_Enable;
extern spi_bit Spi_InterruptEnable;
// ---API---
void SpiSlaveInit(void);
#endif // _SPISLAVE_H
