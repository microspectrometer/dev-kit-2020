#ifndef _UARTSPI_H
#define _UARTSPI_H
#include <stdint.h>
#include "ReadWriteBits.h"
//---Hardware types: register addresses, pin numbers, bit numbers---
typedef uint8_t volatile * const uspi_ptr; // i/o reg address
typedef uint16_t volatile * const uspi_ptr16; // i/o reg address
typedef uint8_t const uspi_pin; // bit index into i/o reg for an i/o pin
typedef uint8_t const uspi_bit; // bit index into i/o reg
// Register address, pin number, and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/Hardware.h
// ---Registers---
extern uspi_ptr UartSpi_ddr;
extern uspi_ptr UartSpi_port;
extern uspi_ptr UartSpi_UCSR0A;
extern uspi_ptr UartSpi_UCSR0B;
extern uspi_ptr UartSpi_UCSR0C;
extern uspi_ptr UartSpi_UDR0;
extern uspi_ptr16 UartSpi_UBRR0;
// ---Pins---
extern uspi_pin UartSpi_Miso; // UART RX
extern uspi_pin UartSpi_AdcConv; // high: convert, low: readout
extern uspi_pin UartSpi_Sck; // UART XCK
// ---Bits---
extern uspi_bit UartSpi_UMSEL00; // set: mode select 0: SPI Master
extern uspi_bit UartSpi_UMSEL01; // set: mode select 1: SPI Master
extern uspi_bit UartSpi_RXEN0; // set: enable Rx
extern uspi_bit UartSpi_TXEN0; // set: enable Tx
extern uspi_bit UartSpi_UCPOL0; // set: CPOL=1 (Clock Polarity)
extern uspi_bit UartSpi_UCPHA0; // set: CPHA=1 (Clock Phase)
extern uspi_bit UartSpi_UDORD0; // clear: Data Order is MSB first
extern uspi_bit UartSpi_UDRE0; // (DataRegEmtpy) is set on Tx done
extern uspi_bit UartSpi_RXC0; // (RxComplete) is set on Rx done

// ---API---
void UartSpiInit(void);
inline void StartAdcReadout(void)
{
    ClearBit(UartSpi_port, UartSpi_AdcConv);
    // ---Expected Assembly---
    // cbi	0x0b, 2	; 11
}

#endif // _UARTSPI_H
