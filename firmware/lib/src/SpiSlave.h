#ifndef _SPISLAVE_H
#define _SPISLAVE_H
#include <stdint.h>
#include "ReadWriteBits.h"
//---Hardware types: register addresses, pin numbers, bit numbers---
typedef uint8_t volatile * const spi_ptr; // i/o reg address
typedef uint8_t const spi_pin; // bit index of i/o reg for i/o pin
typedef uint8_t const spi_bit; // bit index into i/o reg
// Register address, pin number, and bit definitions depend on
// compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/Hardware.h
// ---Registers---
extern spi_ptr Spi_ddr;
extern spi_ptr Spi_port;
extern spi_ptr Spi_SPCR;
extern spi_ptr Spi_SPDR;
extern spi_ptr Spi_SPSR;
// ---Pins---
extern spi_pin Spi_DataReady; // slave signal DR to master
extern spi_pin Spi_Miso; // master-in, slave-out
// ---Bits---
extern spi_bit Spi_Enable;
extern spi_bit Spi_InterruptEnable;
// ---API---
void SpiSlaveInit(void);
void DisableSpiInterrupt(void);
void EnableSpiInterrupt(void);
inline void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes)
{
    uint16_t byte_index;
    for (byte_index = 0; byte_index < nbytes; byte_index++)
    {
        *Spi_SPDR = input_buffer[byte_index];
    }
}
inline uint8_t ReadSpiStatusRegister(void) { return *Spi_SPSR; }
inline uint8_t ReadSpiDataRegister(void) { return *Spi_SPDR; }
inline void ClearSpiInterruptFlag(void)
{
    /** Manually clear SPI interrupt flag by first reading the
     * SPI status register, then reading the SPI data register.
     * */
    ReadSpiStatusRegister(); // in	r24, 0x2d
    ReadSpiDataRegister(); // in	r24, 0x2e
}
#endif // _SPISLAVE_H
