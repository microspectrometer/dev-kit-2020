// This PCB is the bridge from the USB host to the Chromation spectrometer.
//
#include <DebugLed.h>               // controls the debug LED
#include "DebugLed-Hardware.h"      // map debug LED to actual hardware
#include <Ft1248.h>                 // supports Usb -- TODO: does app need this?
#include "Ft1248-Hardware.h"        // map FT1248 (USB) I/O to actual hardware
#include <Usb.h>                    // USB host communication
#include <Spi.h>                    // Chromation spectrometer is a SPI slave
#include "../../mBrd/src/Spi-Hardware.h" // map SPI I/O to actual hardware
#include "../../mBrd/src/Spi-Commands.h" // commands understood by the SPI slave
#include <ReadWriteBits.h>
#include <stdio.h>  // snprintf()
#include <string.h> // strlen()
//
/* =====[ Required Hardware Settings in FT_Prog ]===== */
/* - Hardware Specific -> Ft1248 Settings */
/* - Clock Polarity High: unchecked */
/* - Bit Order LSB: unchecked */
/* - Flow Ctrl not selected: checked */
//
/* =====[ List of tests ]===== */
// test_UsbWrite
    // [x] UsbWrite_took_the_happy_path_if_debug_led_is_green
    // [x] UsbWrite_called_before_UsbInit_turns_debug_led_red
    // [x] UsbWrite_sends_bytes_to_the_USB_host
// test_UsbRead
    // [x] Turn_debug_led_red_when_there_is_a_byte_to_read
    // [x] Turn_debug_led_red_when_rx_byte_is_0x01
// test_EchoByte
    // [x] EchoByte_reads_a_byte_and_writes_it_back_to_the_host
// test_SpiMaster
    // [x] SpiMaster_sends_a_byte_and_slave_debug_leds_show_lower_nibble
    // [x] SpiMaster_detects_when_slave_is_ready_to_send_data
    // [x] Slave_receives_request_and_sends_response_when_ready
        // Master sends the request.
        // Slave parses the request.
        // Slave signals to master it has a response ready.
        // Master gets response from slave.
    // [x] Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host
    // [x] Get_several_bytes_from_slave_and_write_bytes_to_USB_host
    // [x] Get_a_frame_from_slave_and_write_frame_to_USB_host
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
typedef struct TestResult TestResult;
struct TestResult
{
    uint8_t const pcb_name[7];  // `simBrd` or `mBrd`
    uint8_t const test_name[81]; // names should be well under 80 char
    uint8_t const pass_fail[5]; // `PASS` or `FAIL`
};
uint8_t const text_color_reset[]    = "\033[39m";
uint8_t const text_color_grey[]     = "\033[30m"; // test_name
uint8_t const text_color_white[]    = "\033[37m"; // test_name, PASS, response
uint8_t const text_color_cyan[]     = "\033[36m"; // PASS
uint8_t const text_color_red[]      = "\033[31m"; // FAIL
uint8_t const text_color_yellow[]   = "\033[33m"; // simBrd
uint8_t const text_color_magenta[]  = "\033[35m"; // mBrd
uint8_t const * PassFailColor(TestResult test)
{
    int test_failed = strcmp((char *)test.pass_fail, "PASS");
    return test_failed ? text_color_red : text_color_white;
}
uint8_t AddTestResultStringLengths(TestResult test_result)
{
    return  strlen((char *)test_result.pcb_name) +
            strlen((char *)test_result.test_name) +
            strlen((char *)test_result.pass_fail);
}
void PrintTestResultInMono(TestResult this_test)
{
    size_t len =    AddTestResultStringLengths(this_test) +
                    strlen("`` test ``:\n") +
                    1;      // add one more byte for the NULL terminator
    uint8_t string[len];
    snprintf((char *)string, len,
        "`%s` test `%s`:%s\n",
        this_test.pcb_name,
        this_test.test_name,
        this_test.pass_fail
        );
    uint16_t num_bytes_to_send;
    /* num_bytes_to_send = sizeof(string);  // change sizeof to strlen to drop NULL */
    num_bytes_to_send = strlen((char *)string);
    UsbWrite(string, num_bytes_to_send);
}
void PrintTestResultInColor(TestResult this_test)
{
    size_t len =    AddTestResultStringLengths(this_test) +
                    strlen(" test :\n") +
                    20 +    // add 5 bytes for each color code
                    1;      // add one more byte for the NULL terminator
    uint8_t string[len];
    snprintf((char *)string, len,
        "%s%s test %s%s:%s%s%s\n",
        /* text_color_yellow, */
        text_color_white,
        this_test.pcb_name,
        text_color_grey,
        this_test.test_name,
        /* text_color_cyan, */
        PassFailColor(this_test), // PASS is text_color_white, FAIL is text_color_red
        this_test.pass_fail,
        text_color_reset
        );
    uint16_t num_bytes_to_send;
    /* num_bytes_to_send = sizeof(string);  // change sizeof to strlen to drop NULL */
    num_bytes_to_send = strlen((char *)string);
    UsbWrite(string, num_bytes_to_send);
}
void PrintSpiSlaveResponseInMono(uint8_t response)
{
    size_t len =    strlen("SPI slave responded with ``\n") +
                    1 + // response is one byte
                    1;  // NULL
    uint8_t string[len];
    snprintf((char *)string, len,
        "SPI slave responded with `%c`\n",
        response
        );
    uint16_t num_bytes_to_send;
    num_bytes_to_send = strlen((char *)string);
    UsbWrite(string, num_bytes_to_send);
}
void PrintSpiSlaveResponseInColor(uint8_t response)
{
    size_t len =    strlen("SPI slave responded with   \n") +
                    1 + // response is one byte
                    15+ // add 5 bytes for each color code
                    1;  // NULL
    uint8_t string[len];
    snprintf((char *)string, len,
        "%sSPI slave responded with %s %c %s\n",
        text_color_grey,
        text_color_white,
        response,
        text_color_reset
        );
    uint16_t num_bytes_to_send;
    num_bytes_to_send = strlen((char *)string);
    UsbWrite(string, num_bytes_to_send);
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
void UsbWrite_sends_bytes_to_the_USB_host(void)
{
    // Manually Run host application
    //
    /* =====[ USB Host Application ]===== */
    /* ```python */
    /* import serial */
    /* s=serial.Serial() */
    /* s.baudrate = 9600 */
    /* s.port = 'COM9' */
    /* s.open() */
    /* ``` */
    //
    // Write bytes to the USB Host by pressing the reset button on the PCB.
    // After power-up, this function is called.
    // This function writes 6 bytes to the USB host.
    // Manually run host `read` command.
    //
    /* ```python */
    /* s.read(s.inWaiting()) */
    /* ``` */
    //
    // Visually confirm that bytes received are these values in this order:
    // {0, 1, 2, 3, 4, 5}
    // >>> s.read(s.inWaiting())
    // '\x00\x01\x02\x03\x04\x05'
    //
    UsbWrite_took_the_happy_path_if_debug_led_is_green();
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
    /* UsbWrite_sends_bytes_to_the_USB_host(); // PASS 2018-07-28 */
}
//
void Turn_debug_led_red_when_there_is_a_byte_to_read(void)
{
    /* =====[ USB Host Application ]===== */
    /* ```python */
    /* import serial */
    /* s=serial.Serial() */
    /* s.baudrate = 9600 */
    /* s.port = 'COM6' */
    /* s.open() */
    /* s.write('\x00') */
    /* ``` */
    //
    // Loop forever checking MISO.
    // Expect MISO goes low when there is data in the USB rx buffer.
    //
    // Visually confirm the debug LED turns red when the host executes
    // `s.write`.
    //
    // Cycle power to the FTDI device to clear the data in the rx buffer.
    //
    while(1)
    {
        if ( UsbHasDataToRead() ) DebugLedTurnRed();
        /* UsbHasDataToRead_returns_true_if_the_rx_buffer_has_data */
    }
}
void Turn_debug_led_red_when_rx_byte_is_0x01(void)
{
    /* =====[ USB Host Application ]===== */
    /* ```python */
    /* import serial */
    /* s=serial.Serial() */
    /* s.baudrate = 9600 */
    /* s.port = 'COM12' */
    /* s.open() */
    /* s.write('\x01') */
    //
    // Read bytes as soon as they are available.
    // Turn the debug LED red if the byte is 0x01.
    //
    // Visually confirm the LED is green for `s.write('\x00')`, then the debug
    // LED turns red for `s.write('\x01')`.
    uint8_t expected_byte = 0x01;
    /* =====[ Operate ]===== */
    UsbInit();
    uint8_t read_buffer[] = {0x00};
    while(1)
    {
        if ( UsbHasDataToRead() )
        {
            UsbRead(read_buffer);
            if (read_buffer[0] == expected_byte) DebugLedTurnRed();
        }
    }
}
void test_UsbRead(void)
{
    //
    // Pick one test to run.
    // Uncomment that test.
    // Leave the other tests commented out.
    //
    /* Turn_debug_led_red_when_there_is_a_byte_to_read(); // PASS 2018-07-27 */
    /* Turn_debug_led_red_when_rx_byte_is_0x01(); // PASS 2018-07-28 */
}
//
void EchoByte(void)
{
    uint8_t read_buffer[1];
    uint16_t num_bytes_read = UsbRead(read_buffer);
    UsbWrite(read_buffer, num_bytes_read);
}
void EchoByte_reads_a_byte_and_writes_it_back_to_the_host(void)
{
    /* >>> s.write('\x00\x01\x02\x03\x04\x05\x06\x07\x08') */
    /* >>> s.read(s.inWaiting()) */
    /* '\x00\x01\x02\x03\x04\x05\x06\x07\x08' */
    //
    /* DebugLedTurnRed(); */
    /* =====[ Operate ]===== */
    UsbInit(); while(1) if ( UsbHasDataToRead() ) EchoByte();
}
void test_EchoByte(void)
{
    EchoByte_reads_a_byte_and_writes_it_back_to_the_host(); // PASS 2018-07-28
}
void SetupDebugLed(void)
{
    DebugLedInit(
        DebugLed_ddr,
        DebugLed_port,
        DebugLed_pin,
        debug_led
        );
}
/* void SpiMaster_sends_byte_0x01(void) */
/* { */
/*     /1* Call this function on the SPI Master *1/ */
/*     /1* when running this test on the SPI slave. *1/ */
/*     SpiMasterOpenSpi(); */
/*     // Load SPI tx buffer with a byte to send. */
/*     *Spi_spdr = 0x01; */
/*     // Wait for SPI tranmission to complete. Version without interrupts. */
/*     while( BitIsClear(Spi_spsr, Spi_InterruptFlag) ); */
/*     SpiMasterCloseSpi(); */
/* } */
void SpiMaster_sends_a_byte_and_slave_debug_leds_show_lower_nibble(void)
{
    /* Connect to `mBrd`. `mBrd` is the SpiSlave. */
    /* SpiSlave runs `Show_received_SPI_data_on_debug_leds()`. */
    /* After downloading flash to the `simBrd`, */
    /* move `SW2` from `ISP` to `SPI` and press the PCB reset button. */
    //
    /* =====[ Setup ]===== */
    SpiMasterInit();
    /* =====[ Operate ]===== */
    uint8_t byte_to_send = 0x0B;
    SpiMasterWrite(byte_to_send);
    /* =====[ Test ]===== */
    // Visually confirm data on slave debug LEDs is:
    // led number: 4  3  2  1
    // led color:  R  G  R  R
}
void Slave_receives_request_and_sends_response_when_ready(void)
{
    /* =====[ Setup ]===== */
    SpiMasterInit();
    /* =====[ Operate ]===== */
    uint8_t const cmd_slave_respond_0xBA = 0xAB;
    SpiMasterWrite(cmd_slave_respond_0xBA);
    /* =====[ Test ]===== */
    SpiMasterWaitForResponse(); // Slave signals when the response is ready.
    uint8_t const expected_response = 0xBA;
    if ( SpiMasterRead() == expected_response ) DebugLedTurnRed();
    // Visually confirm the debug LED turns red.
}
void Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(cmd_send_dummy_byte);
    SpiMasterWaitForResponse(); // Slave signals when the response is ready.
    DebugLedTurnRed(); // for manual testing
    // Visually confirm the debug LED turns red.
    uint8_t response = SpiMasterRead();
    /* =====[ Test ]===== */
    if ( response == cmd_send_dummy_byte )
    {
        TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host",
        .pass_fail = "PASS"
        /* .pass_fail = "FAIL"  // example of a failing test */
        };
        // Example of printing with color:
        PrintTestResultInColor(this_test);
        PrintSpiSlaveResponseInColor(response);
        // Example of printing without color:
        /* PrintTestResultInMono(this_test); */
        /* PrintSpiSlaveResponseInMono(response); */
        //
        // Manually Run host application and confirm master received 0x01.
        // Confirm slave leds 1, 2, and 3 turn red.
    }
    /* =====[ USB Host Application ]===== */
    /* PS> &$python2_os_Windows */
    /* ```python */
    /* import serial */
    /* s=serial.Serial() */
    /* s.baudrate = 9600 */
    /* s.port = 'COM12' */
    /* s.open() */
    /* ``` */
    // Move `SW2` to `SPI`. Press the reset button.
    /* ```python */
    /* a = s.read(s.inWaiting()) */
    /* a */
    /* ``` */
    // View the raw bytes.
        // The `\x00` is the NULL terminator in the `test_passed` message.
        // The `\x01` is the slave response.
        // "\n" is printed as "\n"
    /* ```python */
    /* a = s.read(s.inWaiting()) */
    /* print a */
    /* ``` */
    // View the print-formatted test result.
        // the `NULL` terminator prints as a single space
        // the `\x01` shows up as a smiley face
        // "\n" is interpreted as a newline
}
void Get_several_bytes_from_slave_and_write_bytes_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(cmd_send_four_dummy_bytes);
    uint8_t fake_data[4]; uint8_t * pfake_data = fake_data;
    uint16_t nbytes = sizeof(fake_data);
    uint16_t byte_counter = 0;
    while (byte_counter++ < nbytes)
    {
        SpiMasterWaitForResponse(); // Slave signals when the response is ready.
        *(pfake_data++) = SpiMasterRead();
    }
    DebugLedTurnRed();
    /* =====[ Test ]===== */
    // Visually confirm debug LED turned red: slave responded with four bytes.
    TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "Get_several_bytes_from_slave_and_write_bytes_to_USB_host",
        .pass_fail = "PASS"
        };
    PrintTestResultInColor(this_test);
    for (uint16_t i=0; i<nbytes; i++) PrintSpiSlaveResponseInColor(fake_data[i]);
    //
    // Manually Run host application and confirm master received 1, 2, 3, 4.
    // simBrd test Get_several_bytes_from_slave_and_write_bytes_to_USB_host:PASS
    // SPI slave responded with  ☺
    // SPI slave responded with  ☻
    // SPI slave responded with  ♥
    // SPI slave responded with  ♦
    // Confirm slave leds 1, 2, and 3 turn red.
}
void Get_a_frame_from_slave_and_write_frame_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(cmd_send_dummy_frame);
    uint8_t fake_data[num_bytes_in_a_dummy_frame];
    uint8_t * pfake_data = fake_data;
    uint16_t byte_counter = 0;
    while (byte_counter++ < num_bytes_in_a_dummy_frame)
    {
        SpiMasterWaitForResponse(); // Slave signals when the response is ready.
        *(pfake_data++) = SpiMasterRead();
    }
    DebugLedTurnRed();
    /* =====[ Test ]===== */
    // Visually confirm debug LED turned red: slave responded with a full frame.
    TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "Get_a_frame_from_slave_and_write_frame_to_USB_host",
        .pass_fail = "PASS"
        };
    PrintTestResultInColor(this_test);
    // Print the fake data sent by the slave.
    byte_counter = 0; pfake_data = fake_data;
    while (byte_counter++ < num_bytes_in_a_dummy_frame)
    {
        PrintSpiSlaveResponseInColor( *(pfake_data++) );
    }
    // for loop version:
    /* for (byte_counter = 0; byte_counter < num_bytes_in_a_dummy_frame; byte_counter++) */
    /* { */
    /*     PrintSpiSlaveResponseInColor(fake_data[byte_counter]); */
    /* } */
    // Expect the alphabet:
        // SPI slave responded with  A
        // SPI slave responded with  B
        // SPI slave responded with  C
        // SPI slave responded with  D
        // SPI slave responded with  E
        // SPI slave responded with  F
        // SPI slave responded with  G
        // SPI slave responded with  H
        // SPI slave responded with  I
        // SPI slave responded with  J
        // SPI slave responded with  K
        // SPI slave responded with  L
        // SPI slave responded with  M
        // SPI slave responded with  N
        // SPI slave responded with  O
        // SPI slave responded with  P
        // SPI slave responded with  Q
        // SPI slave responded with  R
        // SPI slave responded with  S
        // SPI slave responded with  T
        // SPI slave responded with  U
        // SPI slave responded with  V
        // SPI slave responded with  W
        // SPI slave responded with  X
        // SPI slave responded with  Y
        // SPI slave responded with  Z
}
void SpiMaster_detects_when_slave_is_ready_to_send_data(void)
{
    /* =====[ Setup ]===== */
    SpiMasterInit();
    SpiMasterWrite(0x00);
    /* =====[ Operate ]===== */
    SpiMasterWaitForResponse();
    /* =====[ Test 1 ]===== */
    // The master should hang here until the slave pulls MISO low.
    //
    // Visually confirm LED is green if `SW2` is set to `ISP`.
    // This disconnects the master and slave `SCK`, preventing communication.
    /* =====[ Test 2 ]===== */
    // Move `SW2` to `SPI` and press the reset button.
    // The master is able to communicate with the slave.
    //
    // Now the slave responds by pulling MISO low and execution continues.
    DebugLedTurnRed();
    // Visually confirm LED turns red.
    //
    // Measure MISO.
    // A voltmeter will not show the voltage go low because it happens very
    // quickly.
    // A digital oscilloscope set to trigger on `Slave Select` should show a
    // brief dip on MISO just after `Slave Select` goes high, but I have not
    // done this test.
    while(1);
}
void test_SpiMaster(void)
{
    /* SpiMaster_sends_a_byte_and_slave_debug_leds_show_lower_nibble(); // PASS 2018-07-31 */
    /* Slave_receives_request_and_sends_response_when_ready(); // PASS 2018-08-02 */
    /* SpiMaster_detects_when_slave_is_ready_to_send_data();  // PASS 2018-08-03 */
    /* Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host(); // PASS 2018-08-08 */
    /* Get_several_bytes_from_slave_and_write_bytes_to_USB_host(); // PASS 2018-08-08 */
    Get_a_frame_from_slave_and_write_frame_to_USB_host(); // PASS 2018-08-09
}
int main()
{
    //
    // Pick one test group to run.
    // Uncomment that test group.
    // Leave the other test groups commented out.
    //
    SetupDebugLed();
    /* test_UsbRead(); // All test pass 2018-07-28 */
    /* test_UsbWrite();   // All tests pass 2018-07-28 */
    /* test_EchoByte(); // All tests pass 2018-07-30 */
    test_SpiMaster(); // All test pass 2018-08-03
}
