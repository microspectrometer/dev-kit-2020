#include "Spi.h"
// avr headers included by Makefile:
// <avr/interrupt.h> defines macros `sei()` and `cli()`

// ---Private---
void _EnableSpiModule(void);
bool _SpiTransferIsDone(void);

// ---API---
uint8_t ReadSpiStatusRegister(void);
uint8_t ReadSpiDataRegister(void);

