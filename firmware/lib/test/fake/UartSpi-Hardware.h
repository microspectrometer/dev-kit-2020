#ifndef _UARTSPI_HW_H
#define _UARTSPI_HW_H

#include <stdint.h>    // uint8_t
//#include <avr/io.h>  // This is the file being faked.

// =====[ Use This File With lib header "UartSpi.h" ]=====
// This file resolves hardware dependencies declared extern in the lib headers.
//
// =====[ Fake the I/O Register Addresses ]=====
// Fake IO by letting the compiler allocate arbitrary static memory addresses.
// Make these `static` because symbols `value_in_fake_UartSpiBlah` are not to be
// used by the test code. The symbols are only for obtaining addresses.
static uint8_t  volatile value_in_fake_UartSpi_ddr;
static uint8_t  volatile value_in_fake_UartSpi_port;
static uint8_t  volatile value_in_fake_UartSpi_csra;
static uint8_t  volatile value_in_fake_UartSpi_csrb;
static uint8_t  volatile value_in_fake_UartSpi_csrc;
static uint8_t  volatile value_in_fake_UartSpi_data;
static uint16_t volatile value_in_fake_UartSpi_br;
//
//  =====[ I/O Register Address ]=====
uint8_t  volatile * const UartSpi_ddr    = &value_in_fake_UartSpi_ddr;
uint8_t  volatile * const UartSpi_port   = &value_in_fake_UartSpi_port;
uint8_t  volatile * const UartSpi_csra   = &value_in_fake_UartSpi_csra;
uint8_t  volatile * const UartSpi_csrb   = &value_in_fake_UartSpi_csrb;
uint8_t  volatile * const UartSpi_csrc   = &value_in_fake_UartSpi_csrc;
uint8_t  volatile * const UartSpi_data   = &value_in_fake_UartSpi_data;
uint16_t volatile * const UartSpi_br     = &value_in_fake_UartSpi_br;
//
// =====[ Faking the UartSpi pin connection on the mBrd ]=====
/* ---Pin Names--- */
uint8_t const UartSpi_Miso      = 0;  // UART RX
uint8_t const UartSpi_Mosi      = 1;  // not used because ADC is read-only
uint8_t const UartSpi_AdcConv   = 2;  // high to ADC convert, low to readout
uint8_t const UartSpi_Sck       = 4;  // UART XCK
/* =====[ UartSpi control and status register bits ]===== */
uint8_t const UartSpi_ModeSelect0   = 6; // UartSpi_csrc
uint8_t const UartSpi_ModeSelect1   = 7; // UartSpi_csrc
uint8_t const UartSpi_RxEnable      = 4; // UartSpi_csrb
uint8_t const UartSpi_TxEnable      = 3; // UartSpi_csrb
uint8_t const UartSpi_ClockPolarity = 0; // UartSpi_csrc
uint8_t const UartSpi_ClockPhase    = 1; // UartSpi_csrc
uint8_t const UartSpi_DataOrder     = 2; // UartSpi_csrc
uint8_t const UartSpi_DataRegEmpty  = 5; // UartSpi_csra
uint8_t const UartSpi_RxComplete    = 7; // UartSpi_csra

#endif // _UARTSPI_HW_H
