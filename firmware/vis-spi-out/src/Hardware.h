#ifndef _HARDWARE_H
#define _HARDWARE_H
// avr libs
#include <avr/interrupt.h> // defines macro ISR()
#include <avr/io.h> // includes iom328p.h for hardware i/o values
// ${inlhw_lib_src} libs have inline functions so add those defs
// here for inclusion in the main() translation unit
#include "BiColorLed-Hardware.h"
// ${hw_lib_src} libs need the hardware defs in their own
// translation units so I cannot include those defs here:
// "SpiSlave-Hardware.h"

#endif // _HARDWARE_H
