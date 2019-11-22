#ifndef _READWRITEBITS_H
#define _READWRITEBITS_H
#include <stdint.h>  // define uint8_t
#include <stdbool.h> // define 'bool' for c-compilers

/* =====[ Bit-Operation are macros instead of inline functions  ]===== */
// Bit operations are macros. This is the only way I can inline the ReadBits
// functions. Using `inline`, ReadBits functions are sometimes optimized away by
// avr-gcc. I do not know why.
//
// The only way I've found to prevent the avr-gcc from optimizing away the
// function is to explicitly copy the function body into the caller. But this is
// exactly what `inline` is for. When the code is not optimized away, the
// `inline` happens: the function call overhead is eliminated, and the function
// body shows up in the `.lst`. So `inline` is correctly implemented. But the
// compiler sometimes erasese the call completely. The function body does not
// show up in the `.lst`. I see this with the call:
//   `while(BitIsClear(&PORTD, PD7));`
// the expected `.lst` is:
//   sbis	0x09, 7	; 9
//   rjmp	.-4      	;
// The actual `.lst` is nothing. It is equivalent to erasing the while loop.
//
// On the otherhand, if the read-bits function body is copied directly into the
// caller, the resulting code is *not* optimized away.

// WriteBits
#define ToggleBit(ADDRESS,BIT)       (*ADDRESS ^= (uint8_t)(1<<BIT))
#define SetBit(ADDRESS,BIT)          (*ADDRESS |= (uint8_t)(1<<BIT))
#define ClearBit(ADDRESS,BIT)        (*ADDRESS &= (uint8_t)~(1<<BIT))
// ReadBits
#define BitIsSet(ADDRESS,BIT)        (*ADDRESS & (uint8_t)(1<<BIT))
#define BitIsClear(ADDRESS,BIT)    (!(*ADDRESS & (uint8_t)(1<<BIT)))

inline void _SetBit(uint8_t volatile * const port, uint8_t const bit)
{
    *port |= 1<<bit;
}
inline void _ClearBit(uint8_t volatile * const port, uint8_t const bit)
{
    *port &= ~(1<<bit);
}
inline void _ToggleBit(uint8_t volatile * const port, uint8_t const bit)
{
    *port ^= (1<<bit);
}

/* --------------------------------------------------- */

#endif // _READWRITEBITS_H
