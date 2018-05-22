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


#endif // _READWRITEBITS_H
