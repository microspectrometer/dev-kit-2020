#ifndef _FT1248_HW_H
#define _FT1248_HW_H

#include <stdint.h>    // uint8_t
//#include <avr/io.h>  // This is the file being faked.

// =====[ Use This File With "Ft1248.h" ]=====
// This file resolves hardware dependencies declared extern in "Ft1248.h".
//
// =====[ Fake the I/O Register Addresses ]=====
// Fake IO by letting the compiler allocate arbitrary static memory addresses.
// Make these `static` because symbols `value_in_fake_FtBlah` are not used.
static uint8_t volatile value_in_fake_Ft1248_ddr;
static uint8_t volatile value_in_fake_Ft1248_port;
static uint8_t volatile value_in_fake_Ft1248_pin;
static uint8_t volatile value_in_fake_FtMiosio_ddr;
static uint8_t volatile value_in_fake_FtMiosio_port;
static uint8_t volatile value_in_fake_FtMiosio_pin;
//
//
//
//
//
//  =====[ I/O Register Address ]=====
// control lines
uint8_t volatile * const Ft1248_ddr     =   &value_in_fake_Ft1248_ddr;
uint8_t volatile * const Ft1248_port    =   &value_in_fake_Ft1248_port;
uint8_t volatile * const Ft1248_pin     =   &value_in_fake_Ft1248_pin;
// bi-directional data lines (driven by both master and slave)
uint8_t volatile * const FtMiosio_ddr   =   &value_in_fake_FtMiosio_ddr;
uint8_t volatile * const FtMiosio_port  =   &value_in_fake_FtMiosio_port;
uint8_t volatile * const FtMiosio_pin   =   &value_in_fake_FtMiosio_pin;

// =====[ Faking the Ft1248 pin connection on the simBrd ]=====
// control lines
uint8_t const Ft1248_Ss     =   0;    // Slave select
uint8_t const Ft1248_Sck    =   1;    // Ft1248 bus clock
uint8_t const Ft1248_Miso   =   2;    // slave data line: master-in, slave-out
// bi-directional data lines (driven by both master and slave)
uint8_t const FtMiosio0     =   0;
uint8_t const FtMiosio1     =   1;
uint8_t const FtMiosio2     =   2;
uint8_t const FtMiosio3     =   3;
uint8_t const FtMiosio4     =   4;
uint8_t const FtMiosio5     =   5;
uint8_t const FtMiosio6     =   6;
uint8_t const FtMiosio7     =   7;

#endif // _FT1248_HW_H



