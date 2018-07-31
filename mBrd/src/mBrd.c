/* #include <avr/interrupt.h>      // defines sei and cli */
#include <ReadWriteBits.h>      // SetBit, ClearBit, etc.
#include <DebugLeds.h>          // controls the 4 debug LEDs
#include "DebugLeds-Hardware.h" // map debug LEDs to actual hardware
#include <Spi.h>                // Chromation spectrometer is a SPI slave
#include "Spi-Hardware.h"       // map SPI I/O to actual hardware

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
    DebugLedsTurnGreen(debug_led1);
    DebugLedsTurnRed(debug_led2);
    DebugLedsTurnRed(debug_led3);
    DebugLedsTurnRed(debug_led4);
}
void test_DebugLeds(void)
{
    All_debug_leds_turn_on_and_turn_green(); // PASS 2018-07-30
    /* All_debug_leds_turn_on_and_turn_red(); // PASS 2018-07-30 */
    /* Turn_led1_red_and_the_rest_green(); // PASS 2018-07-30 */
}
void Turn_led3_red_when_SpiSlave_receives_a_byte(void)
{
    /* =====[ Operate ]===== */
    // SPI Master sends a byte.
    /* =====[ Version without interrupts ]===== */
    // Do nothing until SPI Interrupt Flag is set.
    while( BitIsClear(Spi_spsr, Spi_InterruptFlag) );
    DebugLedsTurnRed(debug_led3);
}
void Show_received_SPI_data_on_debug_leds(void)
{
    /* =====[ Operate ]===== */
    // SPI Master sends a 4-bit value.
    /* =====[ Version without interrupts ]===== */
    // Do nothing until SPI Interrupt Flag is set.
    while( BitIsClear(Spi_spsr, Spi_InterruptFlag) );
    if (BitIsSet(Spi_spdr, 0)) DebugLedsTurnRed(debug_led1);
    if (BitIsSet(Spi_spdr, 1)) DebugLedsTurnRed(debug_led2);
    if (BitIsSet(Spi_spdr, 2)) DebugLedsTurnRed(debug_led3);
    if (BitIsSet(Spi_spdr, 3)) DebugLedsTurnRed(debug_led4);
}

int main()
{
    /* test_DebugLeds(); // All tests pass 2018-07-30 */
    DebugLedsTurnAllOn();
    DebugLedsTurnAllGreen();
    /* =====[ test SpiSlave ]===== */
    SpiSlaveInit();
    /* sei(); // Enable interrupts */
    /* Turn_led3_red_when_SpiSlave_receives_a_byte(); // PASS 2018-07-30 */
    Show_received_SPI_data_on_debug_leds(); // PASS 2018-07-30
}
