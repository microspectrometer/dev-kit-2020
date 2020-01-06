/** \file SpiSlave.c
 * # API
 * void SpiSlaveInit(void);\n 
 * void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes);\n 
 * uint8_t ReadSpiStatusRegister(void);\n 
 * uint8_t ReadSpiDataRegister(void);\n 
 * void ClearSpiInterruptFlag(void);\n 
 * void DisableSpiInterrupt(void);\n 
 * void EnableSpiInterrupt(void);\n 
 * */
#ifdef SPISLAVE_FAKED
#include "SpiSlave_faked.h"
#endif
#include "SpiSlave.h"
// avr headers included by Makefile:
// <avr/interrupt.h> defines macros `sei()` and `cli()`

// ---Private---
void EnableSpiModule(void);

// ---API---
void SpiSlaveInit(void);
void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes);
uint8_t ReadSpiStatusRegister(void);
uint8_t ReadSpiDataRegister(void);
void ClearSpiInterruptFlag(void);
void DisableSpiInterrupt(void);

// ---API functions with fakes for unit tests---
#ifndef SPISLAVE_FAKED
void EnableSpiInterrupt(void);
#endif
