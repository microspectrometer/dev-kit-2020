// $ avr-gcc src/avra/S13131-S13131Expose.c -S -O3 -o src/avra/S13131-S13131Expose.avra -I../lib/src -Isrc -mmcu=atmega328p -DS13131

#include "Hardware.h"
#include "S13131.h"

void not_really_an_entry_point(void)
{
    S13131Expose();
}
