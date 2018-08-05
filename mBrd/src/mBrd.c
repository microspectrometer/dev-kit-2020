// This PCB gives the Chromation spectrometer chip a digital-out interface.
//
// my libs and the headers that resolve their hardware dependencies
#include <ReadWriteBits.h>      // SetBit, ClearBit, etc.
#include <DebugLeds.h>          // controls the 4 debug LEDs
#include "DebugLeds-Hardware.h" // map debug LEDs to actual hardware
#include <Spi.h>                // Chromation spectrometer is a SPI slave
#include "Spi-Hardware.h"       // map SPI I/O to actual hardware
#include "Spi-Commands.h"       // commands understood by the SPI slave
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
    // [x] App_version_of_Slave_receives_request_without_interrupts
    // [ ] App_version_of_Slave_receives_request_with_interrupts

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
        // app should either:
            // poll SpiTransferIsDone() then call SpiSlaveRead()
            // or
            // enable the interrupt and move SpiSlaveRead() into the ISR
        // just calling SpiSlaveRead() as in this example,
        // the app loops checking SpiTransferIsDone() and never does anything
        // else, like petting the watchdog
        //
        // I left this example as-is to make the test clear.
        // There is nothing to refactor for this test.
        // Refactor in the `App_version` versions of this test.
        //
    uint8_t cmd = SpiSlaveRead();
    // parse the command
        // refactor this as another command pattern:
        // SpiParseCommand(); SpiExecuteCommand();
        // parse points the execute at the correct response routine
        // every routine follows the form: do something, load data, signal ready
    if (cmd == cmd_slave_respond_0xBA)
    {
        DebugLedsTurnRed(debug_led2);
        /* =====[ Do something to get data ]===== */
        uint8_t const response = 0xBA;
        /* =====[ Load data ]===== */
        *Spi_spdr = response;
        /* =====[ Signal ready to send ]===== */
        SpiSlaveSignalDataIsReady();
    }
    /* =====[ Test ]===== */
    // Visually confirm debug LED 2 is red.
    // The slave parsed the request correctly.
}
void SendDummyData(uint8_t dummy_byte)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    *Spi_spdr = dummy_byte;
    SpiSlaveSignalDataIsReady();
}
void SendDummyFrame(void)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
}
void LoadError(void)
{
    DebugLedsTurnRed(debug_led4);  // for manual testing
}
void SendDataMasterAskedFor(void)
{
    /* SpiSlaveSignalDataIsReady(); */
    ClearBit(Spi_port, Spi_Miso);
    while(1);
    DebugLedsTurnRed(debug_led1);  // for manual testing
    uint8_t cmd = SpiSlaveRead();
    // parse and act: get the data, load into SPDR, signal master when ready
    if      (cmd == cmd_send_dummy_data_0xDB) SendDummyData(0xDB);
    /* else if (cmd == cmd_send_bytes_0xB1_0xB2_0xB3_0xB4) SendDummyFrame(); */
    /* // need an established command to ignore -- for when master does read */
    /* else LoadError();  // PASS 2018-08-03 */
    /* DebugLedsTurnRed(debug_led3);  // for manual testing */
}
void RespondToRequestsForData(void)
{
    // If the SPI master requested data, process the request.
    // If there is no request for data, do nothing.
    /* if ( SpiTransferIsDone() ) DoWhatMasterSays(); */
    if ( SpiTransferIsDone() ) SendDataMasterAskedFor();
    // Note: if the SPI master tri-states the SPI pins, it is *very* easy for
    // noise to trip the SPI slave hardware module into communication:
        // - do not program the master to do SPI communication
        // - do not configure the SPI pins on the master, leave them tri-stated
        // - put `SW2` to `SPI`
        // - press the reset button
        // - touch `SW2` but do not actually switch it
        // - touching `SW2` couples noise which trips the SPI module!
    // This issue is easily avoided by initializing the SPI master with
    // SpiMasterInit().
    // The slave assumes the master *is* does not leave the SPI pins tri-stated.
    // The slave assumes *any* SPI communication is a request from the master.
    // The slave responds to *all* SPI communications.
}
void App_version_of_Slave_receives_request_without_interrupts(void)
{
    DebugLedsTurnRed(debug_led1);  // for manual testing
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    // Try loading data register with 0. See if that affects value of MISO.
    *Spi_spdr = 0x00;  // no effect, MISO still high
    /* SpiSlaveSignalDataIsReady(); // does the master see this? no */
    // try waiting to pull MISO low until after a handshake
    SpiSlaveRead(); // wait for a byte, read it and discard it
    // PASS: the slave hangs here until a byte is sent
    DebugLedsTurnRed(debug_led2);  // for manual testing
    SpiSlaveSignalDataIsReady();
    /* // Try disabling SPI! */
    /* ClearBit(Spi_spcr, Spi_Enable); // PASS */
    /* // And re-enabling SPI */
    /* SetBit(Spi_spcr, Spi_Enable); */
    DebugLedsTurnRed(debug_led3);  // for manual testing
    while(1); // Loop forever
    /* =====[ Test ]===== */
    // Visually confirm led1 red: running this test
    // Visually confirm led2 red: finished reading byte from master
    // Visually confirm led3 red: pulled MISO low (or tried to)
    /* =====[ Main Loop ]===== */
    while(1) RespondToRequestsForData();
    // Actual app does other stuff, e.g., pet the watchdog.
    /* =====[ Test ]===== */
    // Program master to send byte 0x01.
    //
    // Visually confirm debug LEDs are green.
    // Move `SW2` to `SPI`.
    // Press reset button.
    // Visually confirm debug LED 1 is red: slave hears master.
    // Visually confirm debug LED 2 is red: slave understood master.
    // Visually confirm debug LED 3 is red: slave is done.
    //
    // Check if master received 0xDB.
}
void test_SpiSlave(void)
{
    /* Turn_led3_red_when_SpiSlave_receives_a_byte(); // PASS 2018-07-31 */
    /* SpiSlaveRead_and_show_received_data_on_debug_leds(); // PASS 2018-08-01 */
    /* SPI_interrupt_routine_turns_debug_led1_red(); // PASS 2018-08-01 */
    /* SPI_read_in_ISR_and_show_data_on_debug_leds(); // PASS 2018-08-01 */
    /* Slave_receives_request_and_sends_response_when_ready(); // PASS 2018-08-02 */
    App_version_of_Slave_receives_request_without_interrupts();
}
void Miso_measures_3V_when_it_outputs_a_hard_high(void)
{
    SetBit(Spi_ddr,  Spi_Miso);  // output
    SetBit(Spi_port, Spi_Miso);  // high
    /* =====[ Test ]===== */
    // PASS: MISO measures 2.983V when SPI master is not enabled
    // PASS: MISO measures 2.995V when SPI master is enabled
    // This test is not affected by `SW2` or the connection of the Atmel-ice
}
void Miso_measures_0V_when_it_outputs_a_hard_low(void)
{
    SetBit(Spi_ddr,  Spi_Miso);     // output
    ClearBit(Spi_port, Spi_Miso);   // low
    /* =====[ Test ]===== */
    // PASS: MISO measures 0.005V when SPI master is not enabled
    // PASS: MISO measures 0.005V when SPI master is enabled
    // This test is not affected by `SW2` or the connection of the Atmel-ice
}
void SpiSlaveInit_should_make_Miso_output_a_hard_high(void)
{
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    // PASS: Expect MISO measures 2.983V when SPI master is enabled
    // This test is not affected by `SW2` or the connection of the Atmel-ice
    //
    // Fail: Expect MISO measures 2.983V when SPI master is not enabled
        // - output is 0V!
        // Why is Miso a hard low when the SPI master is not enabled
    // Reason:
        // The combination of `EnableSpi()` and connecting the Atmel-ice causes
        // MISO to go low.
        // I do not know why this happens.
        // With SPI enabled and the Atmel-ice connected, the SPI module ends up
        // in an unpredictable state.
    // Solution 1:
        // - after downloading flash
        // - flip `SW2` from `ISP` to `SPI`
        // - the measured voltage goes from 0V back up to 2.983V
    // Solution 2:
        // - first program the SPI master to call SpiMasterInit()
        // - this enables SPI on the SPI master
}
void SpiSlaveSignalDataIsReady_outputs_a_hard_low(void)
{   // I cannot get this test to pass!
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    /* =====[ Operate ]===== */
    SpiSlaveSignalDataIsReady(); // FAIL
    /* SetBit(Spi_ddr,  Spi_Miso);     // output */
    /* ClearBit(Spi_port, Spi_Miso);   // low */
    /* =====[ Test ]===== */
    // Expect MISO measures 0V when SPI master is not enabled.
    // FAIL: output is 3V
    // Reason: the Atmel-ice causes it but I don't know why
    // Solution:
        // - completely disconnect the Atmel-ice after programming
        // - hit the reset button
    // Expect MISO measures 0V when SPI master is enabled.
    // FAIL: If SPI master calls SpiMasterInit(), MISO stays high no matter what
}
void test_Atmel_ice_quirk_requires_flipping_SW2_to_SPI(void)
{
    /* Miso_measures_3V_when_it_outputs_a_hard_high();  // PASS 2018-08-03 */
    /* Miso_measures_0V_when_it_outputs_a_hard_low();   // PASS 2018-08-03 */
    //
    // This test only passes after flipping SW2 to SPI.
    /* SpiSlaveInit_should_make_Miso_output_a_hard_high(); // PASS 2018-08-03 */
    //
    // This test does not pass if the SPI master is enabled.
    /* SpiSlaveSignalDataIsReady_outputs_a_hard_low(); // FAIL 2018-08-03 */
    DebugLedsTurnRed(debug_led4);
    while (1);
}
int main()
{
    /* test_DebugLeds(); // All tests pass 2018-07-30 */
    DebugLedsTurnAllOn();
    DebugLedsTurnAllGreen();
    test_SpiSlave(); // All tests pass 2018-08-02
    /* test_Atmel_ice_quirk_requires_flipping_SW2_to_SPI(); // FAIL */
}
/* void GetDataMasterAskedFor(uint8_t cmd) */
/* { */
/*     // parse and act: get the data and load it into SPDR */
/*     if      (cmd == cmd_send_dummy_data_0xDB) SendDummyData(0xDB); */
/*     else if (cmd == cmd_send_bytes_0xB1_0xB2_0xB3_0xB4) SendDummyFrame(); */
/*     else LoadError();  // PASS 2018-08-03 */
/* } */
/* void DoWhatMasterSays(void) */
/* { */
/*     /1* DebugLedsTurnRed(debug_led1);  // for manual testing *1/ */
/*     // All commands ask the SPI slave to get some data. */
/*     GetDataMasterAskedFor( SpiSlaveRead() ); */
/*     SpiSlaveSignalDataIsReady(); */
/*     /1* DebugLedsTurnRed(debug_led3);  // for manual testing *1/ */
/* } */
