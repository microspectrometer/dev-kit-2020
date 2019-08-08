#ifndef _UARTSPI_HW_H
#define _UARTSPI_HW_H

#include <stdint.h>  // uint8_t
#include <avr/io.h>  // defines IO addresses and pin names

// =====[ Use This File With "UartSpi.h" ]=====
// This file resolves hardware dependencies declared extern in "UartSpi.h".
//
// Hardware dependencies declared in this file are included by avr-gcc:
    // avr-gcc ... -mmcu=atmega328p -B ${atmega328_lib} ...
    // # atmega328_lib is the path to the .o and .a lib files and the spec file.
    // atmega328_lib = '/cygdrive/c/Program Files (x86)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'
    // These lib object files are included with Atmel Studio 7.0. They are not
    // built when make builds the client project. The compiler uses the
    // atmega328 source headers to check macro values like 'DDRC'.
    // I take it on faith that Atmel's .o lib objects match their .h API files.
// The atmega328 hardware dependency API is in <avr/iom328p.h>
    // To open this file with 'gf', add the avr include path to the Vim path
    // Example:
    //      let avr_include='/cygdrive/c/Program\ Files\ (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain/avr/include'
    //      let &path = &path . ',' . avr_include

//  =====[ I/O Register ]=====          =====[ Register's Purpose ]=====
uint8_t volatile * const UartSpi_ddr    =   &DDRD;  // data direction in/out
uint8_t volatile * const UartSpi_port   =   &PORTD;  // data direction in/out
uint8_t volatile * const UartSpi_csra   =   &UCSR0A;// ctrl and status reg C
uint8_t volatile * const UartSpi_csrb   =   &UCSR0B;// ctrl and status reg C
uint8_t volatile * const UartSpi_csrc   =   &UCSR0C;// ctrl and status reg C
uint8_t volatile * const UartSpi_data   =   &UDR0;  // data reg
uint16_t volatile * const UartSpi_br     =   &UBRR0; // baud rate register
/* =====[ UartSpi pin connections on `mBrd` ]===== */
uint8_t const UartSpi_Miso    =   PD0; // to adc spi-slave data out
uint8_t const UartSpi_Mosi    =   PD1; // not connected! ADC is read-only
uint8_t const UartSpi_AdcConv =   PD2; // to adc spi-slave conversion start
uint8_t const UartSpi_Sck     =   PD4; // to adc spi-slave clock in
/* =====[ UartSpi control and status register bits ]===== */
uint8_t const UartSpi_ModeSelect0   =   UMSEL00;  // set for spi-master
uint8_t const UartSpi_ModeSelect1   =   UMSEL01;  // set for spi-master
uint8_t const UartSpi_RxEnable      =   RXEN0;    // set to enable
uint8_t const UartSpi_TxEnable      =   TXEN0;    // set to enable
uint8_t const UartSpi_ClockPolarity =   UCPOL0;   // set for CPOL=1
uint8_t const UartSpi_ClockPhase    =   UCPHA0;   // set for CPHA=1
uint8_t const UartSpi_DataOrder     =   UDORD0;   // clear for MSB first
uint8_t const UartSpi_DataRegEmpty  =   UDRE0;    // set when tx is done
uint8_t const UartSpi_RxComplete    =   RXC0;     // set when rx is done

#endif // _UARTSPI_HW_H

