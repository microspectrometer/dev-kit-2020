#ifndef _BICOLORLED_HARDWARE_H
#define _BICOLORLED_HARDWARE_H
#include <stdint.h>
#include <avr/io.h> // includes iom328p.h for hardware i/o values
#include "BiColorLed.h"

bicolorled_ptr BiColorLed_ddr = &DDRC; // controls if pin is input or output
bicolorled_ptr BiColorLed_port = &PORTC; // controls if pin outputs HIGH or LOW
bicolorled_num led_0 = PINC0;
bicolorled_num led_1 = PINC1;

#endif // _BICOLORLED_HARDWARE_H
