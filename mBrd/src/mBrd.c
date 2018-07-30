#include <ReadWriteBits.h>
#include <DebugLeds.h>
#include "DebugLeds-Hardware.h"

void All_debug_leds_turn_on_and_turn_green(void)
{
    DebugLedsTurnAllOn();
    DebugLedsTurnAllGreen();
}
void All_debug_leds_turn_on_and_turn_red(void)
{
    DebugLedsTurnAllOn();
    DebugLedsTurnAllRed();
}
void Turn_led1_red_and_the_rest_green(void)
{
    DebugLedsTurnAllOn();
    DebugLedsTurnGreen(debug_led1);
    DebugLedsTurnRed(debug_led2);
    DebugLedsTurnRed(debug_led3);
    DebugLedsTurnRed(debug_led4);
}

int main()
{
    /* All_debug_leds_turn_on_and_turn_green(); // PASS 2018-07-30 */
    /* All_debug_leds_turn_on_and_turn_red(); // PASS 2018-07-30 */
    /* Turn_led1_red_and_the_rest_green(); // PASS 2018-07-30 */
}
