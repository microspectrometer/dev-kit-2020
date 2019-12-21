/** \file
 * UartSpi uses internal UART module in SPI Master Mode.
 *
 * SPI Slave is an external ADC.
 * */
#ifndef _UARTSPI_HARDWARE_H
#define _UARTSPI_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "UartSpi.h"
// ---Registers---
uspi_ptr UartSpi_ddr  = &DDRD; // controls if pin is input or output
uspi_ptr UartSpi_port = &PORTD; // controls if pin outputs HIGH or LOW
uspi_ptr UartSpi_UCSR0A  = &UCSR0A; // ctrl and status reg A
uspi_ptr UartSpi_UCSR0B  = &UCSR0B; // ctrl and status reg B
uspi_ptr UartSpi_UCSR0C  = &UCSR0C; // ctrl and status reg C
uspi_ptr UartSpi_UDR0    = &UDR0; // data register
uspi_ptr16 UartSpi_UBRR0 = &UBRR0; // baud rate register
// ---Pins---
uspi_pin UartSpi_Miso    = PD0; // connect to ADC Data Out
uspi_pin UartSpi_AdcConv = PD2; // connect to ADC Conversion Start
uspi_pin UartSpi_Sck     = PD4; // connect to ADC clock in
// ---Bits---
uspi_bit UartSpi_UMSEL00 = UMSEL00;
uspi_bit UartSpi_UMSEL01 = UMSEL01;
uspi_bit UartSpi_RXEN0   = RXEN0;
uspi_bit UartSpi_TXEN0   = TXEN0;
uspi_bit UartSpi_UCPOL0  = UCPOL0;
uspi_bit UartSpi_UCPHA0  = UCPHA0;
uspi_bit UartSpi_UDORD0  = UDORD0;
uspi_bit UartSpi_UDRE0   = UDRE0;
uspi_bit UartSpi_RXC0    = RXC0;
#endif // _UARTSPI_HARDWARE_H
