/* #include <ReadWriteBits.h> */
#include <DebugLeds.h>
#include "DebugLeds-Hardware.h"
#include <SpiSlave.h>
#include "SpiSlave-Hardware.h"

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
void test_DebugLeds(void)
{
    All_debug_leds_turn_on_and_turn_green(); // PASS 2018-07-30
    /* All_debug_leds_turn_on_and_turn_red(); // PASS 2018-07-30 */
    /* Turn_led1_red_and_the_rest_green(); // PASS 2018-07-30 */
}
void Turn_led3_red_when_SpiSlave_receives_a_byte(void)
{
    DebugLedsTurnRed(debug_led3);
}

int main()
{
    test_DebugLeds(); // All tests pass 2018-07-30
    /* =====[ test SpiSlave ]===== */
    SpiSlaveInit();
    Turn_led3_red_when_SpiSlave_receives_a_byte();
}
