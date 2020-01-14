#ifndef _SPISLAVE_FAKED_H
#define _SPISLAVE_FAKED_H
#include <stdint.h>
#include <stdbool.h>

// ---Private functions faked for testing---
bool _SpiTransferIsDone_fake(void);
void _SignalDataReady_fake(void);

// ---API functions faked for testing---
void SpiSlaveTxByte_fake(uint8_t input_byte);
void EnableSpiInterrupt_fake(void);
void DisableSpiInterrupt_fake(void);
void ClearSpiInterruptFlag_fake(void);

#endif // _SPISLAVE_FAKED_H
