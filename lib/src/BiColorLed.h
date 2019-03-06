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

/* =====[ Old stuff. My old stuff sucked. ]===== */
/* This sucks. Why is it so complicated. Make this like the other headers. */
/* The ports and pins should be extern so that a test can easily redefine them. */
//                      =====[ Porcelain ]=====
/* void DebugLedInit( // cfg i/o, make LED lit green */
/*     uint8_t volatile * const DDR_register_address, */
/*     uint8_t volatile * const PORT_register_address, */
/*     uint8_t volatile * const PIN_register_address, */
/*     uint8_t const DebugLed_pin_number_in_register */
/*     ); */
/* extern void (*DebugLedTurnGreen)(void); // is a func ptr for mockist tests */
/* void DebugLedTurnRed(void); */
/* extern void (*DebugLedTurnRedToShowError)(void);  // alias for DebugLedTurnRed */
/* void DebugLedToggleColor(void); */

//                      =====[ Plumbing ]=====
/* bool DebugLedIsRed(void); */
/* bool DebugLedIsGreen(void); */
/* extern void (*DebugLedTurnOn)(void);    // is a func ptr for mockist tests */
/* void DebugLedTurnOff(void); */

/*                  =====[ Harware Dependency ]=====
 * 'DebugLed' has no hardware dependency. It is the client's responsibility to
 * define the dependency through DebugLedInit(...). Use the variables defined
 * in DebugLed-Hardware.h as the arguments to DebugLedInit.
 * Example:
 *      this file:       #include "DebugLed.h"
 *      actual hardware: #include "simBrd/DebugLed-Hardware.h"
 *      fake hardware:   #include "fake/DebugLed-Hardware.h"
 *      =====[ Somewhere In Client's `setup()` ]=====
 *          DebugLedInit(
 *              DebugLed_ddr,
 *              DebugLed_port,
 *              DebugLed_pin,
 *              debug_led
 *              );
 *
 * Note that **avr-gcc** does not catch unresolved hardware dependencies.
 */

/*              =====[ When is the Plumbing useful ]=====
 * The DebugLed provides a visual indicator while testing/debugging.
 * But it can also be used to store error status for the entire system.
 *      Read the error status by checking the color.
 *      Green: no error. Red: error.
 * Example:
 *      if (DebugLedIsGreen()) __no_error__do_normal_stuff__();
 * =====[ nop on avr-targets ]=====
 * This is rare, but if checking the color of the DebugLed immediately after
 * changing color, insert a nop for synchronization before calling
 * DebugLedIsRed/Green:
 * Example:
 *      DebugLedTurnRed();
 *      __no_operation();
 *      if (DebugLedIsRed()) __error__do_error_handling_stuff__();
 */

/*      =====[ Strange DebugLedTurnOff behavior for `simBrd` ]=====
 * DebugLedTurnOff turns off the LED if it is green.
 * DebugLedTurnOff only dims the LED if it is red.
 * 
 * The strange dim-red behavior happens because the LED is turned off by
 * making it an input pin. Turning the LED red enables the internal pull-up
 * resistor when the pin is an input. The internal pull-up provides a weak
 * drive current, making a dim-red LED.
 * 
 * It is the client's responsibility to also call DebugLedTurnGreen() to
 * completely turn off the LED.
 */
#endif // _BICOLOR_LED_H

