#include <DebugLed.h>
#include <DebugLed-Hardware.h>
#include <Ft1248.h>
#include <Ft1248-Hardware.h>
#include <Usb.h>
int main()
{
    DebugLedInit(
        DebugLed_ddr,
        DebugLed_port,
        DebugLed_pin,
        debug_led
        );
    /* DebugLedTurnRedToShowError(); */
    uint8_t write_buffer[] = {0, 1, 2, 3, 4, 5};
    uint16_t num_bytes_to_send = sizeof(write_buffer);
    UsbWrite(write_buffer, num_bytes_to_send);
    /* uint16_t num_bytes_sent = UsbWrite(write_buffer, num_bytes_to_send); */
    /* if (UsbHasDataToRead()) */
    /* { */
    /*     DebugLedTurnGreen(); */
    /* } */
}
