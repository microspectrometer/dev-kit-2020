/** \file BiColorLed.h */
#ifndef _BICOLORLED_H
#define _BICOLORLED_H
#include <stdint.h>
#include "ReadWriteBits.h"

typedef uint8_t volatile * const bicolorled_ptr; // i/o reg address

//! Indicator LED numbers in the JSON file are not necessarily the same as the corresponding bit number in the BiColorLED register.
typedef uint8_t const bicolorled_num; // i/o bit number

// I/O register definitions in Hardware.h for dependency on make target
extern bicolorled_ptr BiColorLed_ddr;
extern bicolorled_ptr BiColorLed_port;
// Bit definitions in Hardware.h for dependency on make target
extern bicolorled_num status_led; // usb-bridge
extern bicolorled_num led_0; // vis-spi-out
extern bicolorled_num led_1; // vis-spi-out

// ---API---
inline void BiColorLedOn(bicolorled_num led)
{
    /** BiColorLedOn behavior:\n 
      * - sets bit in ddr\n 
      * */
    SetBit(BiColorLed_ddr, led);
}
inline void BiColorLedOff(bicolorled_num led)
{
    /** BiColorLedOff behavior:\n 
      * - clears bit in ddr\n 
      * */
    ClearBit(BiColorLed_ddr, led);
}
inline void BiColorLedGreen(bicolorled_num led)
{
    /** BiColorLedGreen behavior:\n 
      * - clears bit in port\n 
      * */
    ClearBit(BiColorLed_port, led);
}
inline void BiColorLedRed(bicolorled_num led)
{
    /** BiColorLedRed behavior:\n 
      * - sets bit in port\n 
      * */
    SetBit(BiColorLed_port, led);
}

#endif // _BICOLORLED_H
