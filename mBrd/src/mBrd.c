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
    // [x] App_version_of_Slave_RespondToRequestsForData
        // [x] Sends_dummy_byte_for_cmd_send_dummy_byte
        // [x] Sends_four_dummy_bytes_for_cmd_send_four_dummy_bytes
            // This is the example for the slave to send a frame of data.
        // [x] Slave_ignores_cmd_slave_ignore
        // [x] Slave_indicates_unknown_cmd_on_led_4
    // [-] App_version_of_Slave_receives_request_with_interrupts

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
void SendDummyByte()
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    *Spi_spdr = cmd_send_dummy_byte;
    SpiSlaveSignalDataIsReady();
}
void SpiSlaveSendBytes(uint8_t *bytes, uint16_t nbytes)
{
    uint16_t byte_index;
    for (byte_index = 0; byte_index < nbytes; byte_index++)
    {
        *Spi_spdr = bytes[byte_index];
        SpiSlaveSignalDataIsReady();
        while ( !SpiTransferIsDone() );
    }
    // When is it safe to load the next byte?
        // Just wait for the transmission to end.
        // The transmit buffer is single-buffered.
        // It cannot be written until the transmission finishes.
        // The receive buffer is double-buffered.
        // Old values are available up until the last bit of the next byte is
        // shifted in.
    // The SPI master waits for MISO to go low after every read.
    // This gaurantees the next byte of data is ready.
    // The SPI master does not have to release SlaveSelect, but it can.
    // SlaveSelect being low should not impact the slave's ability to disable
    // SPI and pull MISO low.
}
void SendFourDummyBytes(void)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    uint8_t fake_data[] = {0x01, 0x02, 0x03, 0x04};
    uint16_t nbytes = sizeof(fake_data);
    SpiSlaveSendBytes(fake_data, nbytes);
}
void SendDummyFrame(void)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    uint8_t fake_data[num_bytes_in_a_dummy_frame];
    uint16_t byte_index;
    for (byte_index = 0; byte_index < num_bytes_in_a_dummy_frame; byte_index++)
    {
        fake_data[byte_index] = byte_index + 65; // 'A' is '\x41' is '65'
    }
    SpiSlaveSendBytes(fake_data, (uint16_t) num_bytes_in_a_dummy_frame);
}
void IndicateUnknownCommand(void)
{
    DebugLedsTurnRed(debug_led4);  // for manual testing
}
void DoNothing(void){}
void SendDataMasterAskedFor(void)
{
    DebugLedsTurnRed(debug_led1);  // for manual testing
    uint8_t cmd = SpiSlaveRead();
    // parse and act
    // each action gets data, loads it into SPDR, and signals master when ready
    if      (cmd == cmd_send_dummy_byte) SendDummyByte();
    else if (cmd == cmd_send_four_dummy_bytes) SendFourDummyBytes();
    else if (cmd == cmd_send_dummy_frame) SendDummyFrame();
    else if (cmd == slave_ignore) DoNothing();
    else IndicateUnknownCommand();
    DebugLedsTurnRed(debug_led3);  // for manual testing
    /* // need an established command to ignore -- for when master does read */
    /* else IndicateUnknownCommand();  // PASS 2018-08-03 */
}
void RespondToRequestsForData(void)
{
    // Poll SPI bus. Process data requests, if any.
    if ( SpiTransferIsDone() ) SendDataMasterAskedFor();
}
void App_version_of_Slave_RespondToRequestsForData(void)
{
    // [x] Sends_dummy_byte_for_cmd_send_dummy_byte
    // pairs with Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host
    // [x] Sends_four_dummy_bytes_for_cmd_send_four_dummy_bytes
    // pairs with Get_several_bytes_from_slave_and_write_bytes_to_USB_host
    // [x] Sends_dummy_frame_for_cmd_send_dummy_frame
    // pairs with Get_a_frame_from_slave_and_write_frame_to_USB_host
    // [x] Slave_indicates_unknown_cmd_on_led_4
    // pairs with Slave_indicates_unknown_cmd_on_led_4
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    /* =====[ Main Loop ]===== */
    while(1) RespondToRequestsForData();
    // Actual app does other stuff, e.g., pet the watchdog.
    /* =====[ Test ]===== */
    // Program master to send a command from `Spi-Commands.h`.
    //
    // Visually confirm debug LEDs are green.
    // Move `SW2` to `SPI`.
    // Press reset button.
    // Visually confirm debug LED 1 is red: slave hears master.
    // Visually confirm debug LED 2 is red: slave understood master.
    // Visually confirm debug LED 3 is red: success or failure, slave is done.
    //
// Notes for designing the SPI master:
    // The slave assumes the master does not leave the SPI pins tri-stated.
    // The slave assumes *any* SPI communication is a request from the master.
    // The slave responds to *all* SPI communications.
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
}
void test_SpiSlave(void)
{
    /* Turn_led3_red_when_SpiSlave_receives_a_byte(); // PASS 2018-07-31 */
    /* SpiSlaveRead_and_show_received_data_on_debug_leds(); // PASS 2018-08-01 */
    /* SPI_interrupt_routine_turns_debug_led1_red(); // PASS 2018-08-01 */
    /* SPI_read_in_ISR_and_show_data_on_debug_leds(); // PASS 2018-08-01 */
    /* Slave_receives_request_and_sends_response_when_ready(); // PASS 2018-08-02 */
    App_version_of_Slave_RespondToRequestsForData(); // PASS 2018-08-08
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
}
