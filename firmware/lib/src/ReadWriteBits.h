#ifndef _READWRITEBITS_H
#define _READWRITEBITS_H
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t volatile * const register_address;
typedef uint8_t const bit_index;
// Write bits
inline void SetBit(register_address reg_addr, bit_index bit)
{
    /** SetBit behavior:\n 
      * - sets bit in register\n 
      * */
    *reg_addr |= 1<<bit;
}
inline void ClearBit(register_address reg_addr, bit_index bit)
{
    /** ClearBit behavior:\n 
      * - clears bit in register\n 
      * */
    *reg_addr &= ~(1<<bit);
}
// Read bits
inline bool BitIsSet(register_address reg_addr, bit_index bit)
{
    return *reg_addr & 1<<bit;
}
inline bool BitIsClear(register_address reg_addr, bit_index bit)
{
    return !(*reg_addr & 1<<bit);
}

#endif // _READWRITEBITS_H
