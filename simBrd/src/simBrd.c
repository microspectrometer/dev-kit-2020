#include <DebugLed.h>
#include <DebugLed-Hardware.h>
int main()
{
    DebugLedInit(
        DebugLed_ddr,
        DebugLed_port,
        DebugLed_pin,
        debug_led
        );
    DebugLedTurnRed();
}
