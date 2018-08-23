#ifndef _LIS_HW_H
#define _LIS_HW_H

#include <stdint.h>    // uint8_t
//#include <avr/io.h>  // This is the file being faked.

// =====[ Use This File With lib header "Lis.h" ]=====
// This file resolves hardware dependencies declared extern in the lib headers.
//
// =====[ Fake the I/O Register Addresses ]=====
// Fake IO by letting the compiler allocate arbitrary static memory addresses.
// Make these `static` because symbols `value_in_fake_LisBlah` are not to be
// used by the test code. The symbols are only for obtaining addresses.
static uint8_t volatile value_in_fake_Lis_ddr1;
static uint8_t volatile value_in_fake_Lis_ddr2;
static uint8_t volatile value_in_fake_Lis_port1;
static uint8_t volatile value_in_fake_Lis_port2;
static uint8_t volatile value_in_fake_Lis_tccr0a;
static uint8_t volatile value_in_fake_Lis_tccr0b;
static uint8_t volatile value_in_fake_Lis_clktop;
static uint8_t volatile value_in_fake_Lis_clkth;
static uint8_t volatile value_in_fake_Lis_pin1;

//  =====[ I/O Register Address ]=====
uint8_t volatile * const Lis_ddr1   = &value_in_fake_Lis_ddr1;
uint8_t volatile * const Lis_ddr2   = &value_in_fake_Lis_ddr2;
uint8_t volatile * const Lis_port1  = &value_in_fake_Lis_port1;
uint8_t volatile * const Lis_pin1   = &value_in_fake_Lis_pin1;
uint8_t volatile * const Lis_port2  = &value_in_fake_Lis_port2;
/* =====[ Use 8-bit PWM for Lis clock ]===== */
uint8_t volatile * const Lis_tccr0a = &value_in_fake_Lis_tccr0a;
uint8_t volatile * const Lis_tccr0b = &value_in_fake_Lis_tccr0b;
uint8_t volatile * const Lis_clktop  = &value_in_fake_Lis_clktop;
uint8_t volatile * const Lis_clkth  = &value_in_fake_Lis_clkth;
// PWM frequency = fcpu/Lis_clktop
// PWM duty cycle = Lis_clkth/Lis_clktop
// =====[ Faking the Lis pin connection on the mBrd ]=====
/* ---Pin Names--- */
uint8_t const Lis_PixSelect = 0;  // PB0 ddr2 port2
uint8_t const Lis_Clk       = 5;  // PD5 ddr1 port1
uint8_t const Lis_Rst       = 6;  // PD6 ddr1 port1
uint8_t const Lis_Sync      = 7;  // PD7 ddr1 pin1

#endif // _LIS_HW_H
