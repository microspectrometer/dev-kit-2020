#ifndef _BICOLORLED_H
#define _BICOLORLED_H
#include <stdint.h>
#include "ReadWriteBits.h"

typedef uint8_t const bicolorled_num; // i/o bit/pin number
typedef uint8_t volatile * const bicolorled_ptr; // i/o reg address

// I/O register definitions in Hardware.h for dependency on make target
extern bicolorled_ptr BiColorLed_ddr;
extern bicolorled_ptr BiColorLed_port;

inline void BiColorLedOn(bicolorled_num led)
{
    /** BiColorLedOn behavior:\n 
      * - sets bit in ddr\n 
      * */
    SetBit(BiColorLed_ddr, led);
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
