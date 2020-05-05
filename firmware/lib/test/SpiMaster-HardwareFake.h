/** \file
 * This file is not used anymore.\n
 * There used to be separate Spi hardware files for master and
 * slave.
 * Now there is one Spi.h file and Spi-Hardware and
 * Spi-HardwareFake.
 * There are still SpiMaster.h and SpiSlave.h files, but these
 * are for the functions unique to master and slave.
 * */
#ifndef _SPIMASTER_HARDWAREFAKE_H
#define _SPIMASTER_HARDWAREFAKE_H

#include "SpiMaster.h"
// ---Fake hardware registers as PC memory for unit tests---
static uint8_t volatile fake_SpiM_ddr;
static uint8_t volatile fake_SpiM_pin;
static uint8_t volatile fake_SpiM_port;
static uint8_t volatile fake_SpiM_SPCR;
static uint8_t volatile fake_SpiM_SPSR;
// ---Registers---
spim_reg SpiM_ddr  = &fake_SpiM_ddr;
spim_reg SpiM_pin  = &fake_SpiM_pin;
spim_reg SpiM_port = &fake_SpiM_port;
spim_reg SpiM_SPCR = &fake_SpiM_SPCR;
spim_reg SpiM_SPSR = &fake_SpiM_SPSR;

// ---Pins---
spim_bit SpiM_DataReady = 1;
spim_bit SpiM_Ss        = 2;
spim_bit SpiM_Mosi      = 3;
spim_bit SpiM_Miso      = 4;
spim_bit SpiM_Sck       = 5;
// ---Bits---
spim_bit SpiM_MasterSlaveSelect = 4;
spim_bit SpiM_ClockBit0 = 0;
spim_bit SpiM_ClockBit1 = 1;
spim_bit SpiM_DoubleClock = 0;
spim_bit SpiM_SpiEnable = 6;

#endif // _SPIMASTER_HARDWAREFAKE_H
