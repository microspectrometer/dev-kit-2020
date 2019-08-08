#ifndef _TEST_BICOLORLED_H
#define _TEST_BICOLORLED_H

#include <stdint.h>

/** `status_led` is defined in `fake/BiColorLed-Hardware.h`. */
/* I cannot simply include that header because the test suite */
/* has multiple test translation units. */
/* If the Usb and Bicolor tests both include the */
/* BiColorLed-Hardware header, then `status_led` is defined */
/* multiple times: once in `test_Usb.o` and once in `test_BiColor.o`. */
extern uint8_t const status_led;
void BiColorLedRed_is_a_high_on_led_pin(void);
void BiColorLedGreen_is_a_low_on_led_pin(void);
void BiColorLedToggleColor_changes_green_to_red(void);
void BiColorLedToggleColor_changes_red_to_green(void);
void BiColorLedOff_is_a_low_on_led_pin_and_data_direction(void);
void BiColorLedOn_is_low_on_led_pin_but_high_on_led_pin_data_direction(void);

#endif // _TEST_BICOLORLED_H
