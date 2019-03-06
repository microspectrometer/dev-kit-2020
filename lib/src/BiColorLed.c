#include "BiColorLed.h"

/* Declare inline functions to define their symbols in *this* translation unit. */
void BiColorLedGreen(uint8_t led_name);
void BiColorLedRed(uint8_t led_name);
void BiColorLedToggleColor(uint8_t led_name);
void BiColorLedOff(uint8_t led_name);
void BiColorLedOn(uint8_t led_name);

/* =====[ Old stuff. Old stuff sucks. I didn't know what I was doing. ]===== */
/* This sucks. Too complicated. */
/* static uint8_t volatile * ddr_register_; */
/* static uint8_t volatile * port_register_; */
/* static uint8_t volatile * pin_register_; */
/* static uint8_t debug_led_; */
/* // */
/* void DebugLedInit( */
/*     uint8_t volatile * const ddr_register, */
/*     uint8_t volatile * const port_register, */
/*     uint8_t volatile * const pin_register, */
/*     uint8_t const debug_led) */
/* { */
/*     ddr_register_ = ddr_register; */
/*     port_register_ = port_register; */
/*     pin_register_ = pin_register; */
/*     debug_led_ = debug_led; */
/*     DebugLedTurnOn(); */
/*     DebugLedTurnGreen(); */
/* } */

/* static void DebugLedTurnOn_Implementation(void) */
/* { */
/*     SetBit(ddr_register_, debug_led_); */
/* } */
/* void (*DebugLedTurnOn)(void) = DebugLedTurnOn_Implementation; */

/* void DebugLedTurnOff(void) */
/* { */
/*     ClearBit(ddr_register_, debug_led_); */
/* } */

/* static void DebugLedTurnGreen_Implementation(void) */
/* { */
/*     ClearBit(port_register_, debug_led_); */
/* } */
/* void (*DebugLedTurnGreen)(void) = DebugLedTurnGreen_Implementation; */

/* void DebugLedTurnRed(void) */
/* { */
/*     SetBit(port_register_, debug_led_); */
/* } */
/* void (*DebugLedTurnRedToShowError)(void) = DebugLedTurnRed; */

/* void DebugLedToggleColor(void) */
/* { */
/*     ToggleBit(port_register_, debug_led_); */
/* } */

/* bool DebugLedIsRed(void) */
/* { */
/*     return BitIsSet(pin_register_, debug_led_); */
/* } */

/* bool DebugLedIsGreen(void) */
/* { */
/*     return BitIsClear(pin_register_, debug_led_); */
/* } */
