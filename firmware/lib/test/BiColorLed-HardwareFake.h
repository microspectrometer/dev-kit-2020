#ifndef _TEST_BICOLORLED_HARDWAREFAKE_H
#define _TEST_BICOLORLED_HARDWAREFAKE_H
#include <stdint.h>
#include "BiColorLed.h"

/* =====[ Fake i/o registers for unit tests ]===== */
static uint8_t volatile value_in_fake_BiColorLed_ddr;
static uint8_t volatile value_in_fake_BiColorLed_port;

bicolorled_num led_0 = 0;
bicolorled_ptr BiColorLed_ddr  = &value_in_fake_BiColorLed_ddr;
bicolorled_ptr BiColorLed_port = &value_in_fake_BiColorLed_port;

#endif // _TEST_BICOLORLED_HARDWAREFAKE_H
