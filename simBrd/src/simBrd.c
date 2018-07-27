#include <DebugLed.h>
#include <DebugLed-Hardware.h>
#include <Ft1248.h>
#include <Ft1248-Hardware.h>
#include <Usb.h>
void operate_UsbWrite(void)
{
    //
    // Write a few bytes for testing purposes.
    //
    // A one-liner call to UsbWrite is not enough.
    // UsbWrite needs to know:
    //     - what bytes to send
    //     - how many bytes to send
    //
    uint8_t write_buffer[] = {0, 1, 2, 3, 4, 5};
    uint16_t num_bytes_to_send = sizeof(write_buffer);
    UsbWrite(write_buffer, num_bytes_to_send);
}
void UsbWrite_called_before_UsbInit_turns_debug_led_red(void)
{
    //
    // Do not call UsbInit.
    // Call UsbWrite.
    // UsbWrite should follow the sad path.
    //
    // Visually confirm the debug LED is red.
    //
    // UsbWrite follows the sad path if the tx buffer is full.
    // The `tx buffer` signal looks `full` if the Ft1248 hardware is not
    // initialized.
    //
    operate_UsbWrite();
}
void UsbWrite_took_the_happy_path_if_debug_led_is_green(void)
{
    //
    // Call UsbInit.
    // Call UsbWrite.
    // UsbWrite should follow the happy path.
    //
    // Visually confirm the debug LED is green.
    //
    UsbInit();
    operate_UsbWrite();
}
void test_UsbWrite(void)
{
    //
    // Pick one test to run.
    // Uncomment that test.
    // Leave the other tests commented out.
    //
    /* UsbWrite_took_the_happy_path_if_debug_led_is_green(); // PASS 2018-07-27 */
    /* UsbWrite_called_before_UsbInit_turns_debug_led_red();  // PASS 2018-07-27 */
}
//
void SetupDebugLed(void)
{
    DebugLedInit(
        DebugLed_ddr,
        DebugLed_port,
        DebugLed_pin,
        debug_led
        );
}
int main()
{
    SetupDebugLed();
    test_UsbWrite();
}
    /* UsbWrite_called_before_UsbInit_turns_debug_led_red(); */
    /* uint16_t num_bytes_sent = UsbWrite(write_buffer, num_bytes_to_send); */
    /* if (UsbHasDataToRead()) */
    /* { */
    /*     DebugLedTurnGreen(); */
    /* } */
