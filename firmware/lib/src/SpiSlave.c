#include "SpiSlave.h"
// avr headers included by Makefile:
// <avr/interrupt.h> defines macros `sei()` and `cli()`

// ---Private---
void _SignalDataReady(void);
void _SignalDataNotReady(void);
/* void _SpiSlave_StopRxQueue(void); */

// ---API---
void SpiSlaveInit(void);
void SpiSlaveTxByte(uint8_t input_byte);
void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes);
void ClearSpiInterruptFlag(void);
void DisableSpiInterrupt(void);
void EnableSpiInterrupt(void);

