#ifndef _FLAG_HARDWAREFAKE_H
#define _FLAG_HARDWAREFAKE_H
#include <stdint.h>
#include "Flag.h"
/* =====[ Fake i/o registers for unit tests ]===== */
static uint8_t volatile fake_SpiFlags;
// ---Registers---
flag_reg Flag_SpiFlags = &fake_SpiFlags; // GPIOR0
// ---Bits---
flag_bit Flag_SlaveRx      = 0; // bit GPIOR00 in GPIOR0
flag_bit Flag_TransferDone = 1; // bit GPIOR01 in GPIOR0
#endif // _FLAG_HARDWAREFAKE_H
