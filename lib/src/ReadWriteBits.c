#include "ReadWriteBits.h"

void SetBit(uint8_t volatile * const port, uint8_t const bit) {
    *port |= 1<<bit;
}

void ClearBit(uint8_t volatile * const port, uint8_t const bit) {
    *port &= ~(1<<bit);
}

void ToggleBit(uint8_t volatile * const port, uint8_t const bit) {
    *port ^= (1<<bit);
}

bool BitIsSet(uint8_t volatile * const port, uint8_t const bit) {
    return *port & (1<<bit);
}

bool BitIsClear(uint8_t volatile * const port, uint8_t const bit) {
    return !(*port & (1<<bit));
}

