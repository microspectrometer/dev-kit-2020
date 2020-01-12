/** \file
 * Flag uses AVR GPIOR registers for direct-bit access to flags.
 * */
#ifndef _FLAG_HARDWARE_H
#define _FLAG_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "Flag.h"
// ---Registers---
flag_reg Flag_SpiFlags = &GPIOR0;
// ---Bits---
flag_bit Flag_SlaveRx      = GPIOR00; // bit 0 in GPIOR0
flag_bit Flag_TransferDone = GPIOR01; // bit 1 in GPIOR0
#endif // _FLAG_HARDWARE_H

