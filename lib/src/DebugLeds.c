#include "DebugLeds.h"
#include "ReadWriteBits.h"

void DebugLedsTurnOn(uint8_t debug_led)
{
    SetBit(DebugLeds_ddr, debug_led);
}

void DebugLedsTurnRed(uint8_t debug_led)
{
    SetBit(DebugLeds_port, debug_led);
}

void DebugLedsTurnGreen(uint8_t debug_led)
{
    ClearBit(DebugLeds_port, debug_led);
}

void DebugLedsTurnAllRed(void)
{
    SetBit(DebugLeds_port, debug_led1);
    SetBit(DebugLeds_port, debug_led2);
    SetBit(DebugLeds_port, debug_led3);
    SetBit(DebugLeds_port, debug_led4);
}

void DebugLedsTurnAllGreen(void)
{
    ClearBit(DebugLeds_port, debug_led1);
    ClearBit(DebugLeds_port, debug_led2);
    ClearBit(DebugLeds_port, debug_led3);
    ClearBit(DebugLeds_port, debug_led4);
}
