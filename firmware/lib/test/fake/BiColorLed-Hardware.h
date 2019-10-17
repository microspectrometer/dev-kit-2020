#ifndef _BICOLOR_LED_HARDWARE_H
#define _BICOLOR_LED_HARDWARE_H

#include <stdint.h>    // uint8_t
//#include <avr/io.h>  // This is the file being faked.

// =====[ Use This File With "lib/src/BiColorLed.h" ]=====
// Hardware dependencies are `extern` variables in the lib header.

/* =====[ Fake registers for unit tests. ]===== */
static uint8_t volatile value_in_fake_BiColorLed_ddr;
static uint8_t volatile value_in_fake_BiColorLed_port;
static uint8_t volatile value_in_fake_BiColorLed_pin;

// Defines symbols for the `extern` variables in the lib header.
uint8_t volatile * const BiColorLed_ddr       = &value_in_fake_BiColorLed_ddr;
uint8_t volatile * const BiColorLed_port      = &value_in_fake_BiColorLed_port;

// Define LED names with their bit index in the I/O port.
// Add led names here and assign arbitrary bit numbers for unit tests.
/* For faking LED in src/BiColorLed-Hardware for usb-bridge.c */
uint8_t const status_led = 0; // fake bit with any number from 0 to 7
/* For faking LEDs in src/BiColorLed-Hardware for vis-spi-out.c */
uint8_t const led_TxRx = 0;
uint8_t const led_Done = 1;
/* TODO: erase these if not used: */
/* uint8_t const status_led1 = 1; // fake bit with any number from 0 to 7 */
/* uint8_t const status_led2 = 2; // fake bit with any number from 0 to 7 */
/* uint8_t const status_led3 = 3; // fake bit with any number from 0 to 7 */
/* uint8_t const status_led4 = 4; // fake bit with any number from 0 to 7 */

#endif // _BICOLOR_LED_HARDWARE_H 

