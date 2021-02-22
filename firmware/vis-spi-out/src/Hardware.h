/** \file 
 * vis-spi-out.c includes this file to pick up the
 * -Hardware.h headers and AVR macros.
 * */
#ifndef _HARDWARE_H
#define _HARDWARE_H
// avr libs
#include <avr/interrupt.h> // defines macro ISR()
#include <avr/io.h> // includes iom328p.h for hardware i/o values
// all hardware libs have hardware definitions
// add those definitions here ("-Hardware.h")
// for inclusion in the main() translation unit
#include "BiColorLed-Hardware.h"
/* #ifdef LIS */
#include "Lis-Hardware.h"
/* #endif */
#ifdef S13131
#include "S13131-Hardware.h"
#endif
/* #include "SpiSlave-Hardware.h" */
#include "Spi-Hardware.h"
#include "UartSpi-Hardware.h"
#include "Flag-Hardware.h"

#endif // _HARDWARE_H
