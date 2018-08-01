#include <ReadWriteBits.h>      // SetBit, ClearBit, etc.
#include <DebugLeds.h>          // controls the 4 debug LEDs
#include "DebugLeds-Hardware.h" // map debug LEDs to actual hardware
#include <Spi.h>                // Chromation spectrometer is a SPI slave
#include "Spi-Hardware.h"       // map SPI I/O to actual hardware

/* =====[ List of Tests ]===== */
    // [x] SPI_interrupt_routine_turns_debug_led1_red
    //
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
void Turn_led1_green_and_the_rest_red(void)
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
    /* Turn_led1_green_and_the_rest_red(); // PASS 2018-07-30 */
}
void Turn_led3_red_when_SpiSlave_receives_a_byte(void)
{
    /* =====[ Operate ]===== */
    // SPI Master sends any byte.
    SpiSlaveInit();
    while( !SpiTransferIsDone() );
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
    /* =====[ Operate ]===== */
    // SPI Master sends a 4-bit value.
    SpiSlaveInit();
    // TODO: make this SpiSlaveRead
    while( !SpiTransferIsDone() );
    Show_data_on_debug_leds(*Spi_spdr);
}
#include <avr/interrupt.h>      // defines sei and cli
void SpiEnableInterruptForTransferIsDone(void)
{
    cli(); // Disable all interrupts.
    // TODO: clear any pending Transfer-Complete interrupt
    SetBit(Spi_spcr, Spi_InterruptEnable); // Enable SPI interrupt.
    sei(); // Enable all enabled interrupts.
}
ISR(SPI_STC_vect)
{
    /* Turn_led1_green_and_the_rest_red(); */
    DebugLedsTurnRed(debug_led1);
}
void SPI_interrupt_routine_turns_debug_led1_red(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    /* =====[ Operate ]===== */
    SpiEnableInterruptForTransferIsDone();
    /* =====[ Test ]===== */
    // Program the SPI Master to send any byte on reset.
    // Visually confirm the debug LEDs are all green.
    // Flip `SW2` to `SPI`. Press the reset button.
    // Visually confirm debug LED 1 turns red.
    //
    // Repeat test by flipping `SW2` to `ISP` and pressing reset to turn the
    // LEDs back to all green.
    //
    /* while(0); // exit loop immediately */
    while(1); // loop forever
    DebugLedsTurnAllRed();  // This should *never* be called.
}
void test_SpiSlave(void)
{
    /* Turn_led3_red_when_SpiSlave_receives_a_byte(); // PASS 2018-07-31 */
    /* SpiSlaveRead_and_show_received_data_on_debug_leds(); // PASS 2018-08-01 */
    SPI_interrupt_routine_turns_debug_led1_red();
}
int main()
{
    /* test_DebugLeds(); // All tests pass 2018-07-30 */
    DebugLedsTurnAllOn();
    DebugLedsTurnAllGreen();
    test_SpiSlave();
}
