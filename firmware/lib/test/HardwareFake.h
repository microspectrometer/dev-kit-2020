#ifndef _HARDWAREFAKE_H
#define _HARDWAREFAKE_H
// assign fake hardware i/o values to Chromation libs
#include "BiColorLed-HardwareFake.h"
#include "Flag-HardwareFake.h"
/* #include "SpiSlave-HardwareFake.h" */
/* #include "SpiMaster-HardwareFake.h" */
#include "Spi-HardwareFake.h"
#include "UartSpi-HardwareFake.h"
/* #ifdef LIS */
#include "Lis-HardwareFake.h"
/* #endif */
#ifdef S13131
#include "S13131-HardwareFake.h"
#endif
#include "Usb-HardwareFake.h"

#endif // _HARDWAREFAKE_H
