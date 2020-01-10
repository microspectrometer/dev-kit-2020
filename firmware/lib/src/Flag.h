#ifndef _FLAG_H
#define _FLAG_H
#include <stdint.h>
//---Hardware types: register addresses and bit numbers---
typedef uint8_t volatile * const flag_reg; // i/o reg address
typedef uint8_t const flag_bit; // bit index into i/o reg
// Register address and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/Hardware.h
// ---Registers---
extern flag_reg Flag_SpiFlags; // GPIOR0
// ---Bits---
extern flag_bit Flag_SlaveRx; // GPIOR00

// ---Throwaway function to test plumbing---
void Flag(void);

#endif // _FLAG_H
