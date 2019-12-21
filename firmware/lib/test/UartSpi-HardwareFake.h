#ifndef _UARTSPI_HARDWAREFAKE_H
#define _UARTSPI_HARDWAREFAKE_H
#include <stdint.h>
#include "UartSpi.h"
// ---Fake hardware registers as PC memory for unit tests---
static uint8_t volatile fake_ddr;
static uint8_t volatile fake_port;
static uint8_t volatile fake_UCSR0A;
static uint8_t volatile fake_UCSR0B;
static uint8_t volatile fake_UCSR0C;
static uint8_t volatile fake_UDR0;
static uint16_t volatile fake_UBRR0;
// ---Registers---
uspi_ptr UartSpi_ddr  = &fake_ddr;
uspi_ptr UartSpi_port = &fake_port;
uspi_ptr UartSpi_UCSR0A = &fake_UCSR0A;
uspi_ptr UartSpi_UCSR0B = &fake_UCSR0B;
uspi_ptr UartSpi_UCSR0C = &fake_UCSR0C;
uspi_ptr UartSpi_UDR0 = &fake_UDR0;
uspi_ptr16 UartSpi_UBRR0 = &fake_UBRR0;
// ---Pins---
uspi_pin UartSpi_Miso = 0;
uspi_pin UartSpi_AdcConv = 2;
uspi_pin UartSpi_Sck = 4;
// ---Bits---
uspi_bit UartSpi_UMSEL00 = 6;
uspi_bit UartSpi_UMSEL01 = 7;
uspi_bit UartSpi_RXEN0   = 4;
uspi_bit UartSpi_TXEN0   = 3;
uspi_bit UartSpi_UCPOL0  = 0;
uspi_bit UartSpi_UCPHA0  = 1;
uspi_bit UartSpi_UDORD0  = 2;
uspi_bit UartSpi_UDRE0   = 5;
uspi_bit UartSpi_RXC0    = 7;
#endif // _UARTSPI_HARDWAREFAKE_H
