#include "SpiSlave.h"
// avr headers included by Makefile:
// <avr/interrupt.h> defines macros `sei()` and `cli()`

// ---Private---
void _SignalDataReady(void);
void _SignalDataNotReady(void);
void _EnableSpiModule(void);
void _SpiSlave_StopRxQueue(void);
bool _SpiTransferIsDone(void);

// ---API---
void SpiSlaveInit(void);
void SpiSlaveTxByte(uint8_t input_byte);
void SpiSlaveTx(uint8_t const *input_buffer, uint16_t nbytes);
uint8_t ReadSpiStatusRegister(void);
uint8_t ReadSpiDataRegister(void);
void ClearSpiInterruptFlag(void);
void DisableSpiInterrupt(void);
void EnableSpiInterrupt(void);

