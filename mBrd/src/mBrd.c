#include <ReadWriteBits.h>
#include "DebugLed-Hardware.h"

void All_debug_leds_turn_on_and_turn_green(void)
{
    // Turn on debug_led1
    SetBit(DebugLed_ddr, debug_led1);
    // Turn on debug_led2
    SetBit(DebugLed_ddr, debug_led2);
    // Turn on debug_led3
    SetBit(DebugLed_ddr, debug_led3);
    // Turn on debug_led4
    SetBit(DebugLed_ddr, debug_led4);
}
void All_debug_leds_turn_on_and_turn_red(void)
{
    // Turn on debug_led1
    SetBit(DebugLed_ddr, debug_led1);
    // Turn debug_led1 red
    SetBit(DebugLed_port, debug_led1);
    //
    // Turn on debug_led2
    SetBit(DebugLed_ddr, debug_led2);
    // Turn debug_led2 red
    SetBit(DebugLed_port, debug_led2);
    //
    // Turn on debug_led3
    SetBit(DebugLed_ddr, debug_led3);
    // Turn debug_led3 red
    SetBit(DebugLed_port, debug_led3);
    //
    // Turn on debug_led4
    SetBit(DebugLed_ddr, debug_led4);
    // Turn debug_led4 red
    SetBit(DebugLed_port, debug_led4);
}
void Turn_led1_red_and_the_rest_green(void)
{
    // Turn on debug_led1
    SetBit(DebugLed_ddr, debug_led1);
    // Turn debug_led1 red
    SetBit(DebugLed_port, debug_led1);
    //
    // Turn on debug_led2
    SetBit(DebugLed_ddr, debug_led2);
    //
    // Turn on debug_led3
    SetBit(DebugLed_ddr, debug_led3);
    //
    // Turn on debug_led4
    SetBit(DebugLed_ddr, debug_led4);
}

int main()
{
    /* All_debug_leds_turn_on_and_turn_green(); // PASS 2018-07-29 */
    /* All_debug_leds_turn_on_and_turn_red(); // PASS 2018-07-29 */
    /* Turn_led1_red_and_the_rest_green(); // PASS 2018-07-29 */
}
