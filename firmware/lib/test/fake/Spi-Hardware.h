#ifndef _SPI_HW_H
#define _SPI_HW_H

#include <stdint.h>    // uint8_t
//#include <avr/io.h>  // This is the file being faked.

// =====[ Use This File With lib header "Spi.h" ]=====
// This file resolves hardware dependencies declared extern in the lib headers.
//
// =====[ Fake the I/O Register Addresses ]=====
// Fake IO by letting the compiler allocate arbitrary static memory addresses.
// Make these `static` because symbols `value_in_fake_SpiBlah` are not to be
// used by the test code. The symbols are only for obtaining addresses.
static uint8_t volatile value_in_fake_Spi_ddr;
static uint8_t volatile value_in_fake_Spi_port;
static uint8_t volatile value_in_fake_Spi_pin;
static uint8_t volatile value_in_fake_Spi_spcr;  // SPI control register
static uint8_t volatile value_in_fake_Spi_spsr;  // SPI status  register
static uint8_t volatile value_in_fake_Spi_spdr;  // SPI data    register
//
//  =====[ I/O Register Address ]=====
uint8_t volatile * const Spi_ddr    =   &value_in_fake_Spi_ddr;
uint8_t volatile * const Spi_port   =   &value_in_fake_Spi_port;
uint8_t volatile * const Spi_pin    =   &value_in_fake_Spi_pin;
uint8_t volatile * const Spi_spcr   =   &value_in_fake_Spi_spcr;
uint8_t volatile * const Spi_spsr   =   &value_in_fake_Spi_spsr;
uint8_t volatile * const Spi_spdr   =   &value_in_fake_Spi_spdr;

// =====[ Faking the Spi pin connection on the simBrd ]=====
// control lines
uint8_t const Spi_Ss    =   2;    // slave select driven by the master
uint8_t const Spi_Mosi  =   3;    // master-out, slave-in
uint8_t const Spi_Miso  =   4;    // master-in,  slave-out
uint8_t const Spi_Sck   =   5;    // Spi clock driven by the master
uint8_t const Spi_DataReady    =   1;    //  master-in,  slave-out

/* =====[ SPI control register bits ]===== */
uint8_t const Spi_MasterSlaveSelect = 4;
uint8_t const Spi_ClockRateBit0     = 0;
uint8_t const Spi_ClockRateBit1     = 1;
uint8_t const Spi_Enable            = 6;
uint8_t const Spi_InterruptEnable   = 7;

/* =====[ SPI status register bits ]===== */
uint8_t const Spi_DoubleClockRate   = 0;
uint8_t const Spi_InterruptFlag     = 7;

#endif // _SPI_HW_H
