#ifndef _BICOLORLED_HARDWARE_H
#define _BICOLORLED_HARDWARE_H
#include <stdint.h>
#include "BiColorLed.h"

bicolorled_p BiColorLed_ddr = &DDRC; // controls if input or output
bicolorled_n led_0 = PINC0;

#endif // _BICOLORLED_HARDWARE_H
