// $ avr-gcc src/avra/S13131-S13131PinSetup.c -S -O3 -o - -I../lib/src -Isrc -mmcu=atmega328p -DS13131
#include "Hardware.h"
#include "S13131.h"

#include "Hardware.h"
#include "S13131.h"

void entry_point(void)
{
    S13131PinSetup();
}

