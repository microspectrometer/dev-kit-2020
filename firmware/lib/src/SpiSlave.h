#ifndef _SPISLAVE_H
#define _SPISLAVE_H
#include <stdint.h>
#include "ReadWriteBits.h"

typedef uint8_t const spi_pin; // i/o bit/pin number
typedef uint8_t volatile * const spi_ptr; // i/o reg address

// I/O register definitions in Hardware.h for dependency on make target
extern spi_ptr Spi_ddr;
extern spi_ptr Spi_port;

// pin definitions in Hardware.h for dependency on make target
extern spi_pin Spi_DataReady; // master-in, slave-out

void SpiSlaveInit(void);
/*inline void SpiSlaveInit(void) */
/*{ */
/*    /1** SpiSlaveInit behavior:\n */ 
/*      * - makes DataReady an output pin\n */ 
/*      * - idles DataReady high\n */ 
/*      * *1/ */
/*    // DataReady pin idle high */
/*    SetBit(Spi_port, Spi_DataReady); // sbi	0x05, 1 */
/*    // Set DataReady as an output pin */
/*    SetBit(Spi_ddr, Spi_DataReady); // sbi	0x04, 1 */
/*} */
#endif // _SPISLAVE_H
