// This PCB gives the Chromation spectrometer chip a digital-out interface.
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
    // TODO: this no-tri-state requirement conflicts with the use of the ISP
    // pins to bypass the spi-master-simulant and provide access to the SPI bus
    // for an Arduino. In this instance, the master *should* tri-state the pins
    // to let another master take over.
//
// my libs and the headers that resolve their hardware dependencies
#include <ReadWriteBits.h>      // SetBit, ClearBit, etc.
#include <DebugLeds.h>          // controls the 4 debug LEDs
#include "DebugLeds-Hardware.h" // map debug LEDs to actual hardware
#include <Spi.h>                // Chromation spectrometer is a SPI slave
#include "Spi-Hardware.h"       // map SPI I/O to actual hardware
#include "Spi-Commands.h"       // commands understood by the SPI slave
#include <UartSpi.h>            // mBrd USART in MSPIM mode for ADC readout
#include "UartSpi-Hardware.h"   // map UART MSPIM I/O to actual hardware
#include <Lis.h>                // mBrd LIS-770i I/O pins and functions
#include "Lis-Hardware.h"       // map LIS-770i I/O to actual AVR I/O
#include <Pwm.h>                // lib `Lis` uses PWM for the clock signal
#include "Pwm-Hardware.h"       // map `Pwm` I/O to actual AVR I/O
#include "AvrAsmMacros.h"       // resolve lib dependencies on AVR asm macros
// avr libs
#include <avr/interrupt.h>      // defines macro ISR()

/* =====[ List of application example tests ]===== */
    // [x] App_version_of_Slave_RespondToRequestsForData
        // [x] Sends_dummy_byte_for_cmd_send_dummy_byte
        // pairs with SpiMaster_get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host
        //
        // [x] Sends_four_dummy_bytes_for_cmd_send_four_dummy_bytes
        // pairs with Get_several_bytes_from_slave_and_write_bytes_to_USB_host
        //
        // [x] Sends_dummy_frame_for_cmd_send_dummy_frame
        // pairs with SpiMaster_get_a_frame_from_slave_and_write_frame_to_USB_host
        //
        // [x] Slave_indicates_unknown_cmd_on_led_4
        // pairs with Slave_indicates_unknown_cmd_on_led_4
        //
        // [x] Slave_ignores_cmd_slave_ignore
        // pairs with Slave_ignores_cmd_slave_ignore

/* =====[ List of old tests ]===== */
    // [-] App_version_of_Slave_receives_request_with_interrupts
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

/* =====[ Spi-slave application-level API ]===== */
void SetupDebugLeds(void);
void App_version_of_Slave_RespondToRequestsForData(void);
void RespondToRequestsForData(void);
// ...uses:
void SendDataMasterAskedFor(void);
// ...uses these examples:
// these examples will turn into useful application functions
void SendDummyByte(void);
void SendFourDummyBytes(void);
void FillDummyFrameWithAlphabet(void);      // recording a large array to output
void FillDummyFrameWithAdcReadings(void);   // recording a large array to output
void SendDummyFrame(void);             // output a large array
void SendAdcReading(void);
void SendFakeAdcReading(void);
void SendAdcFrame(void);        // just a frame of ADC readings, no LIS yet
// helpers for command parsing
void IndicateUnknownCommand(void) { DebugLedsTurnRed(debug_led4); }
void DoNothing(void){}

/* =====[ Test groups ]===== */
void test_DebugLeds(void);
void test_SpiSlave(void);

/* =====[ DebugLeds application-level API ]===== */
void All_debug_leds_turn_on_and_turn_green(void);
void All_debug_leds_turn_on_and_turn_red(void);
void Turn_led1_red_and_the_rest_green(void);
void Show_data_on_debug_leds(uint8_t four_bits);

void Output_100kHz_PWM(void)
{
    DebugLedsTurnAllRed();
    LisInit();
    // Output PWM on Clk pin
    // PD5 has alternate PWM function OC0B
    // 100kHz, 50% duty cycle square wave
    // Prescaler = 1, OCR0A = 100, OCR0B = 50
    OCR0A = 100; OCR0B = 50;
    // Use Fast PWM mode:
    // TCCR0A: WGM0[1:0] = 11 -- fast PWM mode
    SetBit(&TCCR0A, WGM00); SetBit(&TCCR0A, WGM01);
    // TCCR0B: set WGM02 -- TOP is the value in register OCR0A
    SetBit(&TCCR0B, WGM02);
    // TCCR0B: CS0[2:0] = 001 clk is fcpu, no prescaling
    SetBit(&TCCR0B, CS00); ClearBit(&TCCR0B, CS01); ClearBit(&TCCR0B, CS02);
    // TCCR0A bits COM0B[1:0] control the PWM pin behavior:
    // 00 - Normal port operation, OC0B disconnected
    // 10 - set OC0B at bottom, clear OC0B on compare match
    ClearBit(&TCCR0A, COM0B0); SetBit(&TCCR0A, COM0B1);

    /* =====[ Test ]===== */
    /* probe `J3` pin `CLK` on scope */
    /* expect output is a square wave with a period of 10us and 50% duty cycle */
}
void oldLisRunClkAt50kHz(void)
{
    // 50kHz, 50% duty cycle square wave
    // Prescaler = 1, OCR0A = 200, OCR0B = 100
    OCR0A = 200; OCR0B = 100;
    // Use Fast PWM mode:
    // TCCR0A: WGM0[1:0] = 11 -- fast PWM mode
    SetBit(&TCCR0A, WGM00); SetBit(&TCCR0A, WGM01);
    // TCCR0B: set WGM02 -- TOP is the value in register OCR0A
    SetBit(&TCCR0B, WGM02);
    // TCCR0B: CS0[2:0] = 001 clk is fcpu, no prescaling
    SetBit(&TCCR0B, CS00); ClearBit(&TCCR0B, CS01); ClearBit(&TCCR0B, CS02);
    // TCCR0A bits COM0B[1:0] control the PWM pin behavior:
    // 00 - Normal port operation, OC0B disconnected
    // 10 - set OC0B at bottom, clear OC0B on compare match
    ClearBit(&TCCR0A, COM0B0); SetBit(&TCCR0A, COM0B1);

    /* =====[ Test ]===== */
    /* probe `J3` pin `CLK` on scope */
    /* expect output is a square wave with a period of 10us and 50% duty cycle */
}
/* #define WaitForLisClkFedge while(!(TIFR0 & (1<<OCF0B))); */
void DemoFastestRstResponseToClk(void)
{
    while (1) // PASS with best results: no ISR, use lowest-level code
    {
        while(!(TIFR0 & (1<<OCF0A))); // true if bit is clear; wait for clk redge
        PORTD |= 1<<PD6;    // set bit: Rst High
        // delay bewteen Clk high and Rst high is 0.4 to 0.6us
        TIFR0 |= 1<<OCF0A; // clear the flag by setting the bit
        while(!(TIFR0 & (1<<OCF0B))); // true if bit is clear; wait for clk fedge
        PORTD &= ~(1<<PD6); // clear bit: Rst Low
        // delay bewteen Clk low and Rst low is 0.5 to 0.6us
        TIFR0 |= 1<<OCF0B; // clear the flag by setting the bit
    }
}
// Try inlining. `inline` did nothing. Try macros.
void DemoMacroFastestRstResponseToClk(void)
{
    while (1) // PASS with best results: no ISR, use lowest-level code
    {
        while(MacroBitIsClear(&TIFR0, OCF0A)); // wait for clock rising edge
        MacroSetBit(Lis_port1, Lis_Rst);
        // delay bewteen Clk high and Rst high is 0.4 to 0.6us
        MacroSetBit(&TIFR0, OCF0A); // clear the flag
        while(MacroBitIsClear(&TIFR0, OCF0B)); // wait for clock falling edge
        MacroClearBit(Lis_port1, Lis_Rst);
        // delay bewteen Clk low and Rst low is 0.4 to 0.6us
        MacroSetBit(&TIFR0, OCF0B); // clear the flag
    }
}
void DemoIsrForRstResponseToClk(void)
{  // This is failingly slow if the ISR uses high-level code.
    // Enable interrupts for LisClk redge and fedge
    GlobalInterruptDisable();
    // Clear pending timer interrupts
    SetBit(&TIFR0, OCF0A); // clear the clk redge interrupt flag
    SetBit(&TIFR0, OCF0B); // clear the clk fedge interrupt flag
    SetBit(&TIMSK0, OCIE0A); // enable clk redge interrupt
    SetBit(&TIMSK0, OCIE0B); // enable clk fedge interrupt
    GlobalInterruptEnable();
    while (1);  // sit in an infinite loop; ISR does everything
}
void DemoFailinglySlowRstResponseToClk(void)
{
    while(1)
    {
        // The delay is so long, edges are missed and the frequency is doubled.
        {
            while(BitIsClear(&TIFR0, OCF0B)); // wait for clock falling edge
            ClearBit(Lis_port1, Lis_Rst);
            // -O1 delay is about 20us
            SetBit(&TIFR0, OCF0B); // clear the flag
            while(BitIsClear(&TIFR0, OCF0A)); // wait for clock rising edge
            SetBit(Lis_port1, Lis_Rst);
            // -O1 delay is almost 10us
            SetBit(&TIFR0, OCF0A); // clear the flag
        }
    }
}
int main()
{
    //
    // Pick one test group to run.
    // Uncomment that test group.
    // Leave the other test groups commented out.
    //
    SetupDebugLeds();
    /* test_DebugLeds(); // All tests pass 2018-07-30 */
    /* test_SpiSlave();  // All tests pass 2018-08-08 */
    /* App_version_of_Slave_RespondToRequestsForData(); // PASS 2018-08-08 */
    /* =====[ Test ]===== */
    // Visually confirm clock is 50kHz: PASS 2018-08-21
    // Looking at timing. Nasty surprise: readable high-level code is slow.
    // What if I use the ISR? Is the delay any less?
        // Not for the high-level code.
        // Delay time with the ISR is actually longer for the low-level code.
        // It goes from being under 1us to being almost 2us.
    LisInit();  // PASS 2018-08-21
    /* =====[ Test ]===== */
    // Visually confirm Rst follows Clk.
        // Rst cannot reliably follow Clk if high-level code is used.
        // Low-level reliably has Rst following Clk with and without interrupts.
        // Compare delays for gcc optimization levels -O1, -O2, -O3.
        // It does not matter what optimization level is used.
    // Also look at the delay between Rst edges and Clk edges.
        // Delay time using interrupts is 1.8us to 2us for both edges.
        // Delay time without interrupts is 0.4-06us for redge and0.5-0.6us for
        // fedge.
    /* DemoFastestRstResponseToClk(); // PASS: this has the fastest response */
    /* DemoMacroFastestRstResponseToClk(); // PASS: just as fast but easier to read */
    /* DemoIsrForRstResponseToClk(); // PASS but slower than not using ISR */
    /* DemoFailinglySlowRstResponseToClk(); // FAIL: high-level code is too slow */

    // LIS RST idles low.
    while(1); // PASS: Rst is low.
    //  Pull it high just after a Clk fedge:
    while(!(TIFR0 & (1<<OCF0B))); // wait for Clk fedge
    // pull Rst high
    /* LisExposureStart(); */
    /* SetBit(Lis_port1, Lis_Rst); */
    // Pixels are reset. LIS integrates until Rst goes low again.
    // See sketch `walk through a frame readout`

}
/* =====[ SpiSlave application-level API details ]===== */
void App_version_of_Slave_RespondToRequestsForData(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    UartSpiInit();
    /* =====[ Main Loop ]===== */
    while(1) RespondToRequestsForData();
    // Actual app does other stuff, e.g., pet the watchdog.
    /* =====[ Test ]===== */
    // Program master to send a command from `Spi-Commands.h`.
        // Visually confirm debug LEDs are green.
        // Move `SW2` to `SPI`.
        // Press reset button.
    // Check LEDs:
        // Visually confirm debug LED 1 is red: slave hears master.
        // Visually confirm debug LED 2 is red: slave understood master.
        // Visually confirm debug LED 3 is red: success or failure, slave is done.
}
void RespondToRequestsForData(void)
{
    // Poll SPI bus. Process data requests, if any.
    if ( SpiTransferIsDone() ) SendDataMasterAskedFor();
}
void SendDataMasterAskedFor(void)
{
    DebugLedsTurnRed(debug_led1);  // for manual testing
    uint8_t cmd = SpiSlaveRead();
    // parse and act
    // each action gets data, loads it into SPDR, and signals master when ready
    if      (cmd == cmd_send_dummy_byte) SendDummyByte();
    else if (cmd == cmd_send_four_dummy_bytes) SendFourDummyBytes();
    else if (cmd == cmd_send_dummy_frame) SendDummyFrame();
    else if (cmd == cmd_send_adc_reading) SendAdcReading();
    else if (cmd == cmd_send_fake_adc_reading) SendFakeAdcReading();
    else if (cmd == cmd_send_adc_frame) SendAdcFrame();
    else if (cmd == slave_ignore) DoNothing();  // PASS 2018-08-03
    // `slave_ignore` is available through spi lib
    // for master to send when slave does read
    else IndicateUnknownCommand();              // PASS 2018-08-03
    DebugLedsTurnRed(debug_led3);  // for manual testing
}
void SetupDebugLeds(void)
{
    DebugLedsTurnAllOn();
    DebugLedsTurnAllGreen();
}

/* =====[ Helper functions ]===== */
/* ---Spi-slave application-level API details--- */
void SendDummyByte(void)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    *Spi_spdr = cmd_send_dummy_byte;
    SpiSlaveSignalDataIsReady();
}
void SendFourDummyBytes(void)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    uint8_t fake_data[] = {0x01, 0x02, 0x03, 0x04};
    uint16_t nbytes = sizeof(fake_data);
    SpiSlaveSendBytes(fake_data, nbytes);
}
void FillDummyFrameWithAlphabet(void)
{
    uint8_t * pdummy_frame = dummy_frame;
    uint16_t byte_index;
    for (byte_index = 0; byte_index < sizeof_dummy_frame; byte_index++)
    {
        *(pdummy_frame++) = (byte_index%26) + 65; // 'A' is '\x41' is '65'
    }
}
void FillDummyFrameWithAdcReadings(void)
{
    uint16_t byte_count = 0;
    uint8_t *pframe = dummy_frame;
    while (byte_count < num_bytes_in_a_dummy_frame)
    {
        UartSpiRead(pframe);
        pframe++; pframe++; byte_count++; byte_count++;
    }
}
void SendDummyFrame(void)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    FillDummyFrameWithAlphabet();  // SpiSlaveRunMeasurement();
    uint8_t *pdummy_frame = dummy_frame;
    // Send measurement data to master.
    SpiSlaveSendBytes(pdummy_frame, sizeof_dummy_frame);
}
void SendAdcReading(void)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    uint8_t adc_reading[] = {0,0};
    UartSpiRead(adc_reading);
    uint16_t nbytes = sizeof(adc_reading);
    SpiSlaveSendBytes(adc_reading, nbytes);
}
void SendFakeAdcReading(void)
{
    DebugLedsTurnRed(debug_led2);  // for manual testing
    uint8_t adc[] = {0,0};
    adc[0] = fake_adc_reading >> 8;   // MSByte
    adc[1] = fake_adc_reading & 0xFF; // LSByte

    uint16_t nbytes = sizeof(adc);
    SpiSlaveSendBytes(adc, nbytes);
}
void SendAdcFrame(void)
{
    DebugLedsTurnRed(debug_led2);   // for manual testing
    FillDummyFrameWithAdcReadings();  // SpiSlaveRunMeasurement();
    uint8_t *pdummy_frame = dummy_frame;
    // Send measurement data to master.
    SpiSlaveSendBytes(pdummy_frame, sizeof_dummy_frame);
}

/* ---DebugLeds--- */
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
void Show_data_on_debug_leds(uint8_t four_bits)
{
    // Show the lower nibble of input `four_bits`
    uint8_t *pfour_bits = &four_bits;
    if (BitIsSet(pfour_bits, 0)) DebugLedsTurnRed(debug_led1);
    if (BitIsSet(pfour_bits, 1)) DebugLedsTurnRed(debug_led2);
    if (BitIsSet(pfour_bits, 2)) DebugLedsTurnRed(debug_led3);
    if (BitIsSet(pfour_bits, 3)) DebugLedsTurnRed(debug_led4);
}

/* =====[ SpiSlave ISR ]===== */
/* ---examples using the SPI ISR (found no use for this yet)--- */
void SPI_interrupt_routine_turns_debug_led1_red(void);
void SPI_read_in_ISR_and_show_data_on_debug_leds(void);
/* =====[ Move control over the SPI ISR into the test code ]===== */
typedef void (SPI_ISR_task)(void); SPI_ISR_task *DoTaskForThisTest;
//
ISR(SPI_STC_vect)
{
    DoTaskForThisTest(); // fptr assigned in test code
}
ISR(TIMER0_COMPA_vect)
{
    // high-level delay:
    /* SetBit(Lis_port1, Lis_Rst); */
    // low-level delay:
    PORTD |= 1<<PD6;    // set bit: Rst High
    // delay is 1.8us-2us from redge to Rst High
}
ISR(TIMER0_COMPB_vect)
{
    // high-level delay:
    /* ClearBit(Lis_port1, Lis_Rst); */
    // low-level delay:
    PORTD &= ~(1<<PD6); // clear bit: Rst Low
    // delay is 1.8us-2us from fedge to Rst low
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
/* =====[ SpiSlave non-ISR ]===== */
/* ---examples *not* using ISR (version I am using so far)--- */
void SpiSlaveRead_and_show_received_data_on_debug_leds(void);
void Slave_receives_request_and_sends_response_when_ready(void);
/* =====[ Move control over the SPI ISR into the test code ]===== */
void SpiSlaveRead_and_show_received_data_on_debug_leds(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveInit();
    /* =====[ Operate ]===== */
    // SPI Master sends a 4-bit value.
    Show_data_on_debug_leds(SpiSlaveRead());
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

/* =====[ Test groups ]===== */
void test_DebugLeds(void)
{
    /* All_debug_leds_turn_on_and_turn_green(); // PASS 2018-07-30 */
    /* All_debug_leds_turn_on_and_turn_red(); // PASS 2018-07-30 */
    /* Turn_led1_green_and_the_rest_red(); // PASS 2018-07-30 */
}
void test_SpiSlave(void)
{
    /* SPI_interrupt_routine_turns_debug_led1_red(); // PASS 2018-08-01 */
    /* SPI_read_in_ISR_and_show_data_on_debug_leds(); // PASS 2018-08-01 */
    /* SpiSlaveRead_and_show_received_data_on_debug_leds(); // PASS 2018-08-01 */
    /* Slave_receives_request_and_sends_response_when_ready(); // PASS 2018-08-02 */
    App_version_of_Slave_RespondToRequestsForData(); // PASS 2018-08-08
}
void Show_four_MSB_of_adc_reading_on_debug_leds(void)
{
    UartSpiInit();
    uint8_t adc[] = {0,0};
    UartSpiRead(adc);
    Show_data_on_debug_leds( adc[0] >> 4 );
}
void test_UartSpi(void)
{
    // This is a stupid test without having some way to control the analog
    // differential voltage.
    Show_four_MSB_of_adc_reading_on_debug_leds();
}

/* =====[ Hardware troubleshooting tests ]===== */
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

