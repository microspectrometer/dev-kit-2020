#ifndef _LIS_FAKED_H
#define _LIS_FAKED_H
#include <stdint.h>

// ---Private functions faked for testing---
void _ConfigAs28bits_fake(uint8_t * config);
void _EnterLisProgrammingMode_fake(void);
void _WriteLisConfigBit_fake(uint8_t const * config, uint8_t bit_index);
void _Write28bitLisConfig_fake(uint8_t * config);
void _ExitLisProgrammingMode_fake(void);
void _WaitForLisClkLow_fake(void);
void _WaitForLisClkHigh_fake(void);

#endif // _LIS_FAKED_H
