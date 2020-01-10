#ifndef _SPISLAVE_FAKED_H
#define _SPISLAVE_FAKED_H
#include <stdint.h>
#include <stdbool.h>

// ---Private functions faked for testing---
bool _TransferIsDone_fake(void);

// ---API functions faked for testing---
void SpiSlaveTxByte_fake(uint8_t input_byte);
void EnableSpiInterrupt_fake(void);

#endif // _SPISLAVE_FAKED_H
