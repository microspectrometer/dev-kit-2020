/** \file */
#ifndef _BICOLOR_LED_H
#define _BICOLOR_LED_H

#include <stdint.h>
#include <stdbool.h> // define 'bool'
#include "ReadWriteBits.h"

/** Bi-color LEDs are indicator LEDs on the circuit board. */
/* 0 := green */
/* 1 := red */

/** The I/O register names are defined in the hardware-specific header.
 * The `_ddr` `_port` and `_pin` names are supplied during linking and
 * the `inline` call is inlined as a single `sbi` or `cbi` instruction.
 * */
extern uint8_t volatile * const BiColorLed_ddr;
extern uint8_t volatile * const BiColorLed_port;

/* The header that defines symbols for the `extern` registers */
/* also defines the LED names with their bit index in the I/O port. */
/* Some examples: */
/* uint8_t const status_led_1 = 4;  // status led 1 is bit 4 on the I/O port.*/
/* uint8_t const status_led_2 = 5;  // status led 2 is bit 5 on the I/O port.*/
/* uint8_t const debug_led = 3;     // debug  led   is bit 3 on the I/O port.*/
/* uint8_t const tx_led = 0;      // transmit led   is bit 0 on the I/O port.*/
/* Pass these LED names as arg `led_name` in the API calls. */
/* Example: */
/* BiColorLedGreen(tx_led); */

/* =====[ API ]===== */
/** These are all commands. Read them as actions, not queries.
 * */
inline void BiColorLedGreen(uint8_t led_name)
{
    ClearBit(BiColorLed_port, led_name);
}
inline void BiColorLedRed(uint8_t led_name)
{
    SetBit(BiColorLed_port, led_name);
}
inline void BiColorLedToggleColor(uint8_t led_name)
{
    ToggleBit(BiColorLed_port, led_name);
}
inline void BiColorLedOff(uint8_t led_name)
{
    ClearBit(BiColorLed_ddr, led_name);
    /* Disable internal pull-up to turn *completely* off. */
    ClearBit(BiColorLed_port, led_name);
}
inline void BiColorLedOn(uint8_t led_name)
{
    /** LED defaults to green when turned on.
     * To blink red, remember to turn the LED
     * red *each time* it is turned on. */
    SetBit(BiColorLed_ddr, led_name);
}

#endif // _BICOLOR_LED_H

