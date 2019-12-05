#ifndef _BICOLORLED_H
#define _BICOLORLED_H
#include <stdint.h>

typedef uint8_t const bicolorled_num; // i/o bit/pin number
typedef uint8_t volatile * const bicolorled_ptr; // i/o reg address

// I/O register definitions in Hardware.h for dependency on make target
extern bicolorled_ptr BiColorLed_ddr;

inline void BiColorLedOn(bicolorled_num led_name)
{
    /* SetBit(BiColorLed_ddr, led_name); */
    // change!
    *BiColorLed_ddr |= (uint8_t)(1<<led_name);
}

#endif // _BICOLORLED_H
