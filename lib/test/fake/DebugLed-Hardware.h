#ifndef _DEBUG_LED_HARDWARE_H
#define _DEBUG_LED_HARDWARE_H

// =====[ Unit tests spy on private data in the DebugLed module. ]=====
// Tests spy on stubbed hardware by dereferencing register addresses directly.
// This is only for unit tests. Client code should not do this.
// Client code should only manipulate private data indirectly.
// Data is manipulated indirectly through the API function calls.
// The API function calls use lib ReadWriteBits.
// TODO: introduce hardware abstraction to force the desired privacy.

//=====[ Stub #include <avr/io.h> by letting the compiler pick addresses. ]=====
static uint8_t volatile  value_in_stubbed_ddr_register;
static uint8_t volatile  value_in_stubbed_port_register;
static uint8_t volatile  value_in_stubbed_pin_register;

// =====[ Addresses Of Stubbed Memory-Mapped I/O Registers ]=====
uint8_t volatile * const DebugLed_ddr  = &value_in_stubbed_ddr_register;
uint8_t volatile * const DebugLed_port = &value_in_stubbed_port_register;
uint8_t volatile * const DebugLed_pin  = &value_in_stubbed_pin_register;
// =====[ Faking DebugLed Pin Connection On simBrd ]=====
uint8_t const debug_led = 3;  // value does not matter for tests

#endif // _DEBUG_LED_HARDWARE_H 

