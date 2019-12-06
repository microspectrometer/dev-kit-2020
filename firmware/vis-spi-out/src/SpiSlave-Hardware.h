#ifndef _SPISLAVE_HARDWARE_H
#define _SPISLAVE_HARDWARE_H
#include <stdint.h>
#include "SpiSlave.h"

spi_ptr Spi_ddr = &DDRB; // controls if pin is input or output
spi_ptr Spi_port = &PORTB; // controls if pin outputs HIGH or LOW
spi_pin Spi_DataReady = PB1; // master-in, slave-out

#endif // _SPISLAVE_HARDWARE_H

