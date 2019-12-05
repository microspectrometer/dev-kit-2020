#ifndef _TEST_BICOLORLED_HARDWARE_H
#define _TEST_BICOLORLED_HARDWARE_H
#include <stdint.h>
#include "BiColorLed.h"

/* =====[ Fake i/o registers for unit tests ]===== */
static uint8_t volatile value_in_fake_BiColorLed_ddr;

bicolorled_ptr BiColorLed_ddr = &value_in_fake_BiColorLed_ddr;
bicolorled_num led_0 = 0;

#endif // _TEST_BICOLORLED_HARDWARE_H

