#ifndef _DEBUGLEDS_H
#define _DEBUGLEDS_H

#include <stdint.h>

extern uint8_t volatile * const DebugLeds_ddr;
extern uint8_t volatile * const DebugLeds_port;
extern uint8_t const debug_led1;
extern uint8_t const debug_led2;
extern uint8_t const debug_led3;
extern uint8_t const debug_led4;
void DebugLedsTurnOn(uint8_t debug_led);
void DebugLedsTurnRed(uint8_t debug_led);
#define MacroDebugLedsTurnRed(debug_led) MacroSetBit(DebugLeds_port, debug_led)
void DebugLedsTurnGreen(uint8_t debug_led);
#define MacroDebugLedsTurnGreen(debug_led) MacroClearBit(DebugLeds_port, debug_led)
void DebugLedsTurnAllRed(void);
void DebugLedsTurnAllGreen(void);
#define MacroDebugLedsAllGreen() do { \
    MacroClearBit(DebugLeds_port, debug_led1); \
    MacroClearBit(DebugLeds_port, debug_led2); \
    MacroClearBit(DebugLeds_port, debug_led3); \
    MacroClearBit(DebugLeds_port, debug_led4); \
} while (0)

void DebugLedsTurnAllOn(void);

#endif // _DEBUGLEDS_H
