#ifndef _BICOLORLED_H
#define _BICOLORLED_H
#include <stdint.h>

typedef uint8_t const bicolorled;
typedef uint8_t volatile * const bicolorled_io;

// I/O register definitions depend on make target
extern bicolorled_io BiColorLed_ddr;

inline void BiColorLedOn(bicolorled led_name)
{
    /* SetBit(BiColorLed_ddr, led_name); */
    // change!
    *BiColorLed_ddr |= (uint8_t)(1<<led_name);
}

#endif // _BICOLORLED_H
