#ifndef _DEBUG_LED_H
#define _DEBUG_LED_H

#include <stdint.h>
#include <stdbool.h> // define 'bool' for c-compilers

//                      =====[ Porcelain ]=====
void DebugLedInit( // cfg i/o, make LED lit green
    uint8_t volatile * const DDR_register_address,
    uint8_t volatile * const PORT_register_address,
    uint8_t volatile * const PIN_register_address,
    uint8_t const DebugLed_pin_number_in_register
    );
extern void (*DebugLedTurnGreen)(void); // is a func ptr for mockist tests
void DebugLedTurnRed(void);
void DebugLedToggleColor(void);

//                      =====[ Plumbing ]=====
bool DebugLedIsRed(void);
bool DebugLedIsGreen(void);
extern void (*DebugLedTurnOn)(void);    // is a func ptr for mockist tests
void DebugLedTurnOff(void);

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
#endif // _DEBUG_LED_H

