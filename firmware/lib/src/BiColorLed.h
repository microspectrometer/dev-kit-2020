#ifndef _BICOLORLED_H
#define _BICOLORLED_H
#include <stdint.h>

typedef uint8_t const bicolorled_n; // i/o bit/pin number
typedef uint8_t volatile * const bicolorled_p; // i/o reg address

// I/O register definitions in Hardware.h for dependency on make target
extern bicolorled_p BiColorLed_ddr;

inline void BiColorLedOn(bicolorled_n led_name)
{
    /* SetBit(BiColorLed_ddr, led_name); */
    // change!
    *BiColorLed_ddr |= (uint8_t)(1<<led_name);
}

#endif // _BICOLORLED_H
