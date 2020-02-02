#ifndef _USB_HARDWAREFAKE_H
#define _USB_HARDWAREFAKE_H
#include <stdint.h>
#include "Usb.h"
// ---Fake hardware registers as PC memory for unit tests---
static uint8_t volatile fake_FtCtrl_port;
static uint8_t volatile fake_FtCtrl_pin;
static uint8_t volatile fake_FtData_pin;
static uint8_t volatile fake_FtData_ddr;
static uint8_t volatile fake_FtData_port;
// ---Registers---
// control lines
usb_reg FtCtrl_port = &fake_FtCtrl_port;
usb_reg FtCtrl_pin  = &fake_FtCtrl_pin;
// bi-directional data lines (driven by both master and slave)
usb_reg FtData_pin  = &fake_FtData_pin;
usb_reg FtData_ddr  = &fake_FtData_ddr;
usb_reg FtData_port = &fake_FtData_port;
// ---Pins---
// control lines
usb_pin FtMiso       = 2; // input: !RxbufferEmpty signal
usb_pin FtChipSelect = 0; // output: activate interface
usb_pin FtClock      = 1; // output: clock edges signal drive/sample data on bus
// bi-directional data lines (driven by both master and slave)
usb_pin FtMiosio0 = 0; // data bus bit 0
#endif // _USB_HARDWAREFAKE_H


