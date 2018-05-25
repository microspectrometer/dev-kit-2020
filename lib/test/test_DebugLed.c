#include "test_DebugLed.h"
#include <DebugLed.h>
#include "mock_DebugLed.h"
#include "fake/DebugLed-Hardware.h"
#include <unity.h>
#include <Mock.h>

// =====[ List of Tests To Write ]=====
    // DebugLedInit should:
    // [-] assign io
    //     I cannot think of a way to test this.
    //     Test of the other functions indirectly check that io is assigned.
    // [x] turns the debug led on and green
    //
    // For the remaining tests:
    // DebugLed functions only change one bit in the debug led registers.
    // After checking whether the intended bits are high or low, check that the
    // other bits in the register are unaffected by the function.
    // [x] DebugLedTurnOn_turns_on_debug_led_after_a_turn_off
    // [x] DebugLedTurnOff_turns_off_debug_led_afer_a_turn_on
    // [x] DebugLedTurnRed_turns_the_debug_led_red_after_being_green
    // [x] DebugLedTurnGreen_turns_the_debug_led_green_after_being_red
    // [x] DebugLedToggleColor_changes_the_debug_led_from_red_to_green
    // [x] DebugLedToggleColor_changes_the_debug_led_from_green_to_red
    // [x] DebugLedIsRed_returns_true_if_red
    // [x] DebugLedIsRed_returns_false_if_green
    // [x] DebugLedIsGreen_returns_true_if_green
    // [x] DebugLedIsGreen_returns_false_if_red

static void SetUpMock_DebugLedInit(void)
{
    mock = Mock_new();
    //
    Stub_DebugLedTurnOn();
    Stub_DebugLedTurnGreen();
}
static void TearDownMock_DebugLedInit(void)
{
    Unstub_DebugLedTurnOn();
    Unstub_DebugLedTurnGreen();
    //
    Mock_destroy(mock);
}
//static void InitializeRegisterValues(void) {
//    *DebugLed_ddr  = 0;  // clear bits in fake hardware registers
//    *DebugLed_port = 0;  // clear bits in fake hardware registers
//    *DebugLed_pin  = 0;  // clear bits in fake hardware registers
//}
static void LoadWrongIoAddresses(void) {
    // Create another set of registers to redirect the DebugLed registers.
    uint8_t volatile  value_in_wrong_ddr_register;
    uint8_t volatile  value_in_wrong_port_register;
    uint8_t volatile  value_in_wrong_pin_register;
    // TODO: ensure bit numbers are in range 0 to 7.
        // replace simple `blah+1` with `mod8(blah+1)`
    uint8_t const wrong_debug_led = debug_led+1;
    //
    // Assign DebugLed to the wrong registers.
    // This forces the tests to use a test setUp that calls DebugLedInit.
    DebugLedInit(
        &value_in_wrong_ddr_register,
        &value_in_wrong_port_register,
        &value_in_wrong_pin_register,
        wrong_debug_led
        );
}
void SetUp_DebugLedInit(void){
    SetUpMock_DebugLedInit();    // create the mock object to record calls
}
void TearDown_DebugLedInit(void){
    TearDownMock_DebugLedInit(); // destroy the mock object
    LoadWrongIoAddresses();      // restore the *wrong* fake addresses
}

void DebugLedInit_turns_debug_led_on_and_green(void)
{   // [x] turns the debug led on and green
    Expect_DebugLedTurnOn();
    Expect_DebugLedTurnGreen();
    DebugLedInit(
        DebugLed_ddr,
        DebugLed_port,
        DebugLed_pin,
        debug_led
        );
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}


