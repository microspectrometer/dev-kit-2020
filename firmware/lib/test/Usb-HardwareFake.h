#ifndef _USB_HARDWAREFAKE_H
#define _USB_HARDWAREFAKE_H
#include <stdint.h>
#include "Usb.h"
// ---Fake hardware registers as PC memory for unit tests---
static uint8_t volatile fake_FtCtrl_port;
static uint8_t volatile fake_FtCtrl_pin;
static uint8_t volatile fake_FtData_pin;
// ---Registers---
usb_reg FtCtrl_port = &fake_FtCtrl_port;
usb_reg FtCtrl_pin = &fake_FtCtrl_pin;
usb_reg FtData_pin = &fake_FtData_pin;
// ---Pins---
usb_pin FtMiso = 2; // in (out-from-slave) !RxEmpty signal
usb_pin FtMiosio0 = 0;
#endif // _USB_HARDWAREFAKE_H


