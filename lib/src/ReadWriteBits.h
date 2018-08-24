#ifndef _READWRITEBITS_H
#define _READWRITEBITS_H
#include <stdint.h>  // define uint8_t
#include <stdbool.h> // define 'bool' for c-compilers

typedef bool (ReadBits)(uint8_t volatile * const port, uint8_t const bit);
ReadBits BitIsSet;
ReadBits BitIsClear;
typedef void (WriteBits)(uint8_t volatile * const port, uint8_t const bit);
WriteBits SetBit;
WriteBits ClearBit;
WriteBits ToggleBit;

/* =====[ Bit-Operation Macros ]===== */
// Use these when speed is critical, e.g., responding to a clock edge.
#define MacroToggleBit(ADDRESS,BIT)       (*ADDRESS ^= (uint8_t)(1<<BIT))
#define MacroSetBit(ADDRESS,BIT)          (*ADDRESS |= (uint8_t)(1<<BIT))
#define MacroClearBit(ADDRESS,BIT)        (*ADDRESS &= (uint8_t)~(1<<BIT))
#define MacroBitIsSet(ADDRESS,BIT)        (*ADDRESS & (uint8_t)(1<<BIT))
#define MacroBitIsClear(ADDRESS,BIT)    (!(*ADDRESS & (uint8_t)(1<<BIT)))


#endif // _READWRITEBITS_H
