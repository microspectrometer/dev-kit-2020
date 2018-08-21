#include "ReadWriteBits.h"

inline void SetBit(uint8_t volatile * const port, uint8_t const bit) {
    *port |= 1<<bit;
}

inline void ClearBit(uint8_t volatile * const port, uint8_t const bit) {
    *port &= ~(1<<bit);
}

inline void ToggleBit(uint8_t volatile * const port, uint8_t const bit) {
    *port ^= (1<<bit);
}

inline bool BitIsSet(uint8_t volatile * const port, uint8_t const bit) {
    return *port & (1<<bit);
}

inline bool BitIsClear(uint8_t volatile * const port, uint8_t const bit) {
    return !(*port & (1<<bit));
}

