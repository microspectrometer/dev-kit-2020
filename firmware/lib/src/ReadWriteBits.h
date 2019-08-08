#ifndef _READWRITEBITS_H
#define _READWRITEBITS_H
#include <stdint.h>  // define uint8_t
#include <stdbool.h> // define 'bool' for c-compilers

/* =====[ Bit-Operation Macros ]===== */
// Use these when speed is critical, e.g., responding to a clock edge.
/* #define MacroToggleBit(ADDRESS,BIT)       (*ADDRESS ^= (uint8_t)(1<<BIT)) */
/* #define MacroSetBit(ADDRESS,BIT)          (*ADDRESS |= (uint8_t)(1<<BIT)) */
/* #define MacroClearBit(ADDRESS,BIT)        (*ADDRESS &= (uint8_t)~(1<<BIT)) */
/* #define MacroBitIsSet(ADDRESS,BIT)        (*ADDRESS & (uint8_t)(1<<BIT)) */
/* #define MacroBitIsClear(ADDRESS,BIT)    (!(*ADDRESS & (uint8_t)(1<<BIT))) */

/* --------------------------------------------------- */
/* | 2019-03-04 WIP: inline version of ReadWriteBits | */
/* --------------------------------------------------- */

inline void SetBit(uint8_t volatile * const port, uint8_t const bit)
{
    *port |= 1<<bit;
}
inline void ClearBit(uint8_t volatile * const port, uint8_t const bit)
{
    *port &= ~(1<<bit);
}
inline void ToggleBit(uint8_t volatile * const port, uint8_t const bit)
{
    *port ^= (1<<bit);
}
inline bool BitIsSet(uint8_t volatile * const port, uint8_t const bit)
{
    return *port & (1<<bit);
}
inline bool BitIsClear(uint8_t volatile * const port, uint8_t const bit)
{
    return !(*port & (1<<bit));
}

/* --------------------------------------------------- */

#endif // _READWRITEBITS_H
