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
#include "SpiSlave-Hardware.h"
#include "UartSpi-Hardware.h"
#include "Flag-Hardware.h"

#endif // _HARDWARE_H
