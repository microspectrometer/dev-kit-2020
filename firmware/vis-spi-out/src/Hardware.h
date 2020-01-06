/** \file 
 * vis-spi-out.c includes this file to pick up the
 * -Hardware.h headers and AVR macros.
 * */
#ifndef _HARDWARE_H
#define _HARDWARE_H
// avr libs
#include <avr/interrupt.h> // defines macro ISR()
#include <avr/io.h> // includes iom328p.h for hardware i/o values
// ${inlhw_lib_src} libs have inline functions so add those defs
// here for inclusion in the main() translation unit
#include "BiColorLed-Hardware.h"
#include "Lis-Hardware.h"
// ${hw_lib_src} libs need the hardware defs in their own
// translation units so I cannot include those defs here:
// do not #include "SpiSlave-Hardware.h"
// do not #include "UartSpi-Hardware.h"
// vis-spi-out.c still picks up the hardware definitions (somehow)

#endif // _HARDWARE_H
