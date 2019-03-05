#include "ReadWriteBits.h"

/* Declare inline functions here to emit symbols in this translation unit. */
/* Use typedefs to make this easier to read. */
typedef bool (ReadBits)(uint8_t volatile * const port, uint8_t const bit);
ReadBits BitIsSet;
ReadBits BitIsClear;
typedef void (WriteBits)(uint8_t volatile * const port, uint8_t const bit);
WriteBits SetBit;
WriteBits ClearBit;
WriteBits ToggleBit;
