// my libs and the headers that resolve their hardware dependencies
#include <ReadWriteBits.h>      // SetBit, ClearBit, etc.
#include <DebugLeds.h>          // controls the 4 debug LEDs
#include "DebugLeds-Hardware.h" // map debug LEDs to actual hardware
#include <Spi.h>                // Chromation spectrometer is a SPI slave
#include "Spi-Hardware.h"       // map SPI I/O to actual hardware
#include "AvrAsmMacros.h"       // resolve lib dependencies on AVR asm macros
// avr libs
#include <avr/interrupt.h>      // defines macro ISR()

/* =====[ List of Tests ]===== */
    // [x] SPI_interrupt_routine_turns_debug_led1_red
        // Tests `SpiEnableInterrupt()`
    // [x] SPI_read_in_ISR_and_show_data_on_debug_leds
        // Receives data using interrupts. Does not use `SpiSlaveRead`.
    // [x] SpiSlaveRead_and_show_received_data_on_debug_leds
        // Tests `SpiSlaveRead()`
        // Receives data without using interrupts.
    // [x] Slave_receives_request_and_sends_response_when_ready
        // Master sends the request.
        // Slave parses the request.
        // Slave signals to master it has a response ready.
        // Master gets response from slave.
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
    DebugLedsTurnAllGreen();
    DebugLedsTurnRed(debug_led1);
}
void Turn_led1_green_and_the_rest_red(void)
{
    DebugLedsTurnAllOn();
    DebugLedsTurnAllRed();
    DebugLedsTurnGreen(debug_led1);
}
void test_DebugLeds(void)
{
    All_debug_leds_turn_on_and_turn_green(); // PASS 2018-07-30
    /* All_debug_leds_turn_on_and_turn_red(); // PASS 2018-07-30 */
    /* Turn_led1_green_and_the_rest_red(); // PASS 2018-07-30 */
}
void Turn_led3_red_when_SpiSlave_receives_a_byte(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    /* =====[ Operate ]===== */
    // SPI Master sends any byte.
    // Hang until the byte is received.
    SpiSlaveRead(); // discard the byte
    DebugLedsTurnRed(debug_led3);
}
void Show_data_on_debug_leds(uint8_t four_bits)
{
    // Show the lower nibble of input `four_bits`
    uint8_t *pfour_bits = &four_bits;
    if (BitIsSet(pfour_bits, 0)) DebugLedsTurnRed(debug_led1);
    if (BitIsSet(pfour_bits, 1)) DebugLedsTurnRed(debug_led2);
    if (BitIsSet(pfour_bits, 2)) DebugLedsTurnRed(debug_led3);
    if (BitIsSet(pfour_bits, 3)) DebugLedsTurnRed(debug_led4);
}
void SpiSlaveRead_and_show_received_data_on_debug_leds(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    /* =====[ Operate ]===== */
    // SPI Master sends a 4-bit value.
    Show_data_on_debug_leds(SpiSlaveRead());
}
/* =====[ Move control over the SPI ISR into the test code ]===== */
typedef void (SPI_ISR_task)(void); SPI_ISR_task *DoTaskForThisTest;
//
ISR(SPI_STC_vect)
{
    DoTaskForThisTest(); // fptr assigned in test code
}
void SPI_interrupt_routine_turns_debug_led1_red(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    DoTaskForThisTest = Turn_led1_red_and_the_rest_green;
    /* =====[ Operate ]===== */
    SpiEnableInterrupt();
    /* while(0); // exit loop immediately */
    while(1); // loop forever
    DebugLedsTurnAllRed();  // This should *never* be called.
    /* =====[ Test ]===== */
    // Program the SPI Master to send any byte on reset.
    // Visually confirm the debug LEDs are all green.
    // Flip `SW2` to `SPI`. Press the reset button.
    // Visually confirm debug LED 1 turns red.
    //
    // Repeat test by flipping `SW2` to `ISP` and pressing reset to turn the
    // LEDs back to all green.
    //
}
void ShowSpiDataOnDebugLeds(void)
{ Show_data_on_debug_leds(*Spi_spdr); }
void SPI_read_in_ISR_and_show_data_on_debug_leds(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    DoTaskForThisTest = ShowSpiDataOnDebugLeds;
    /* =====[ Operate ]===== */
    SpiEnableInterrupt();
    // You must loop forever.
    // Interrupts do not execute if the program is allowed to finish.
    while(1);
    DebugLedsTurnAllRed();  // This should *never* be called.
    /* =====[ Test ]===== */
    // Program the SPI Master to send `0x0B` on reset.
    // Visually confirm the debug LEDs are all green.
    // Flip `SW2` to `SPI`. Press the reset button.
    //
    // Visually confirm the lower nibble is displayed:
    // led number: 4  3  2  1
    // led color:  R  G  R  R
}
void Slave_receives_request_and_sends_response_when_ready(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    DebugLedsTurnRed(debug_led1);
    // Visually confirm debug LED 1 is red.
    // The slave is waiting to receive the command.
    //
    // Master sends a byte requesting data.
    uint8_t const cmd_slave_respond_0xBA = 0xAB;
    /* =====[ Operate ]===== */
    // wait for a command from the master
    uint8_t cmd = SpiSlaveRead();
    // parse the command
    if (cmd == cmd_slave_respond_0xBA)
    {
        DebugLedsTurnRed(debug_led2);
        uint8_t const response = 0xBA;
        *Spi_spdr = response;
        SpiSlaveSignalDataIsReady();
    }
    /* =====[ Test ]===== */
    // Visually confirm debug LED 2 is red.
    // The slave parsed the request correctly.
}
void test_SpiSlave(void)
{
    /* Turn_led3_red_when_SpiSlave_receives_a_byte(); // PASS 2018-07-31 */
    /* SpiSlaveRead_and_show_received_data_on_debug_leds(); // PASS 2018-08-01 */
    /* SPI_interrupt_routine_turns_debug_led1_red(); // PASS 2018-08-01 */
    /* SPI_read_in_ISR_and_show_data_on_debug_leds(); // PASS 2018-08-01 */
    Slave_receives_request_and_sends_response_when_ready(); // PASS 2018-08-02
}
int main()
{
    /* test_DebugLeds(); // All tests pass 2018-07-30 */
    DebugLedsTurnAllOn();
    DebugLedsTurnAllGreen();
    test_SpiSlave(); // All tests pass 2018-08-02
}
