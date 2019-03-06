/** \file */
#ifndef _BICOLOR_LED_HARDWARE_H
#define _BICOLOR_LED_HARDWARE_H

#include <stdint.h>
#include <avr/io.h>

/** Include this header to define the `extern` variables in
 * "lib/src/BiColorLed.h". Port and pin macros are defined in
 * "avr/iom328p.h". These macros are included with avr-gcc compiler flag
 * `-mmcu=atmega328p \
 * -B $(INSTALL_DIR)/Atmel/Studio/7.0/packs/atmel/ATmega_DFP/1.2.203/gcc/dev/atmega328p/'
 * */

//  =====[ I/O Register Address ]=====          =====[ Register's Purpose ]=====
uint8_t volatile * const BiColorLed_ddr  = &DDRC;  // controls if input or output
uint8_t volatile * const BiColorLed_port = &PORTC; // controls output high/low

// =====[ Status LED Pin Connection On simBrd ]=====
uint8_t const status_led = PINC3;

#endif // _BICOLOR_LED_HARDWARE_H


