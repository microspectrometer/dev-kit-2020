// $ avr-gcc src/avra/S13131-S13131Readout.c -S -O3 -o src/avra/S13131-S13131Readout.avra -I../lib/src -Isrc -mmcu=atmega328p -DS13131

#include "Hardware.h"
#include "S13131.h"
#include "VisCmd.h"

void entry_point(void)
{
    S13131Readout();
}
