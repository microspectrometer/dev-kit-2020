#ifndef _SPI_FAKED_H
#define _SPI_FAKED_H
#include <stdint.h>
#include <stdbool.h>

// ---Private functions faked for testing---
bool _SpiTransferIsDone_fake(void);

// ---API functions faked for testing---
void ClearSpiInterruptFlag_fake(void);

#endif // _SPI_FAKED_H
