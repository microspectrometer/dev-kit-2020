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
#include "AvrAsmMacros.h"       // resolve lib dependencies on AVR asm macros
//
// Comparing Compiler Optimization levels:
    // -O1
    // ||    text	   data	    bss	    dec	    hex	filename
    // ||    3724	   1002	      7	   4733	   127d	build/simBrd.elf
    // -O3
    // ||    text	   data	    bss	    dec	    hex	filename
    // ||    3330	    996	      7	   4333	   10ed	build/simBrd.elf
    //
    // -Os
    // ||    text	   data	    bss	    dec	    hex	filename
    // ||    3690	   1002	      7	   4699	   125b	build/simBrd.elf
/* =====[ Required Hardware Settings in FT_Prog ]===== */
/* - Hardware Specific -> Ft1248 Settings */
/* - Clock Polarity High: unchecked */
/* - Bit Order LSB: unchecked */
/* - Flow Ctrl not selected: checked */
//
/* =====[ List of manual tests ]===== */
// test_EchoByte
    // [x] EchoByte_reads_a_byte_and_writes_it_back_to_the_host
// test_SpiMaster
    // [x] SpiMaster_detects_when_slave_is_ready_to_send_data
    // [x] Slave_receives_request_and_sends_response_when_ready
        // Master sends the request.
        // Slave parses the request.
        // Slave signals to master it has a response ready.
        // Master gets response from slave.
    // [x] Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host
    // [x] Get_several_bytes_from_slave_and_write_bytes_to_USB_host
    // [x] Get_a_frame_from_slave_and_write_frame_to_USB_host
    // [x] Slave_ignores_cmd_slave_ignore
    // [x] Slave_indicates_unknown_cmd_on_led_4

/* =====[ List of automated tests ]===== */
    // [x] SpiMaster_get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host
    // [x] SpiMaster_get_a_frame_from_slave_and_write_frame_to_USB_host
    // [x] SpiMaster_get_frame_of_adc_readings_and_write_to_USB_host

/* =====[ List of application examples ]===== */
    // [ ] SpiMaster_get_Lis_frame_and_write_to_USB_host

// Spi-master application-level API
uint16_t SpiMasterPassFakeSensorData(void);  // example for getting a frame
uint16_t SpiMasterPassAdcFrame(void);        // another example getting a frame
uint16_t SpiMasterPassLisFrame(void);        // get the real Lis frame
uint16_t nbytes_in_frame;

// Automated Testing API
typedef struct TestResult TestResult;
#define max_pass_fail_string_length 4   // `PASS` or `FAIL
#define max_pcb_name_string_length  6   // `simBrd` or `mBrd`
#define max_test_name_string_length 80  // `Function_does_foo_when_bar`
void RunTest(TestResult *result, bool assert_true);  // record and print result
bool Failed(TestResult *result);                     // return true if failed
// Check for failure to print additional information only for failing tests.

// Automated Testing Details
void PrintTestResultInColor(TestResult this_test);
struct TestResult
{
    uint8_t const pcb_name[max_pcb_name_string_length+1];
    uint8_t const test_name[max_test_name_string_length+1];
    bool passed;
    /* uint8_t const pass_fail[5]; // `PASS` or `FAIL` */
};
void RunTest(TestResult * result, bool assert_true)
{
    // asssert_true := boolean condition that determines pass/fail
    result->passed = assert_true;
    PrintTestResultInColor(*result);
}
bool Failed(TestResult *result) { return result->passed ? false : true; }
#define max_string_length_for_color_info 20
#define max_string_length_for_other_text 12
#define max_test_result_string_length \
    max_pass_fail_string_length         + \
    max_pcb_name_string_length          + \
    max_test_name_string_length         + \
    max_string_length_for_color_info    + \
    max_string_length_for_other_text
#define max_test_result_bytes           max_test_result_string_length+1
uint8_t const text_color_reset[]    = "\033[39m";
uint8_t const text_color_grey[]     = "\033[30m"; // test_name
uint8_t const text_color_white[]    = "\033[37m"; // test_name, PASS, response
uint8_t const text_color_cyan[]     = "\033[36m"; // PASS
uint8_t const text_color_red[]      = "\033[31m"; // FAIL
uint8_t const text_color_yellow[]   = "\033[33m"; // simBrd
uint8_t const text_color_magenta[]  = "\033[35m"; // mBrd
/* uint8_t const * PassFailColor(TestResult test) */
/* { */
/*     int test_failed = strcmp((char *)test.pass_fail, "PASS"); */
/*     return test_failed ? text_color_red : text_color_white; */
/* } */
uint8_t const * PassFailColor(TestResult test)
{
    return test.passed ? text_color_white : text_color_red;
}
/* uint8_t AddTestResultStringLengths(TestResult test_result) */
/* { */
/*     return  strlen((char *)test_result.pcb_name) + */
/*             strlen((char *)test_result.test_name) + */
/*             strlen((char *)test_result.pass_fail); */
/* } */
uint8_t AddTestResultStringLengths(TestResult test_result)
{
    return  strlen((char *)test_result.pcb_name) +
            strlen((char *)test_result.test_name) +
            4; // `PASS` or `FAIL`
}
void PrintTestResultInMono(TestResult this_test)
{
    uint8_t string[max_test_result_bytes];
    size_t len =    AddTestResultStringLengths(this_test) +
                    strlen("`` test ``:\n") +
                    1;      // add one more byte for the NULL terminator
    if (len > max_test_result_bytes)
    {
        snprintf((char *)string, len,
            "Test result is too long to print"
            );
    }
    else
    {
        snprintf((char *)string, len,
            "`%s` test `%s`:%s\n",
            this_test.pcb_name,
            this_test.test_name,
            this_test.passed ? "PASS" : "FAIL"
            );
    }
    uint16_t num_bytes_to_send;
    /* num_bytes_to_send = sizeof(string);  // change sizeof to strlen to drop NULL */
    num_bytes_to_send = strlen((char *)string);
    UsbWrite(string, num_bytes_to_send);
}
void PrintTestResultInColor(TestResult this_test)
{
    uint8_t string[max_test_result_bytes];
    size_t len =    AddTestResultStringLengths(this_test) +
                    strlen("\n test :\n") +
                    20 +    // add 5 bytes for each color code
                    1;      // add one more byte for the NULL terminator
    if (len > max_test_result_bytes)
    {
        snprintf((char *)string, len,
            "Test result is too long to print"
            );
    }
    else
    {
        snprintf((char *)string, len,
            "\n%s%s test %s%s:%s%s%s\n",
            /* text_color_yellow, */
            text_color_white,
            this_test.pcb_name,
            text_color_grey,
            this_test.test_name,
            /* text_color_cyan, */
            PassFailColor(this_test), // PASS is text_color_white, FAIL is text_color_red
            this_test.passed ? "PASS" : "FAIL",
            text_color_reset
            );
    }
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
void PrintSizeOfSpiSlaveResponse(uint16_t nbytes)
{
    size_t len =    strlen("%sSPI slave responded with %s %u bytes %s\n") +
                    4 + // size will take four characters
                    15+ // add 5 bytes for each color code
                    1;  // NULL
    uint8_t string[len];
    snprintf((char *)string, len,
        "%sSPI slave responded with %s %u bytes %s\n",
        text_color_grey,
        text_color_white,
        nbytes,
        text_color_reset
        );
    uint16_t num_bytes_to_send;
    num_bytes_to_send = strlen((char *)string);
    UsbWrite(string, num_bytes_to_send);
}
void PrintAdcReading(uint16_t adc_reading)
{
    size_t len =    strlen("\nADC reading: 0x0000\n") +
                    15+ // add 5 bytes for each color code
                    1;  // NULL
    uint8_t string[len];
    snprintf((char *)string, len,
        "\n%sADC reading: %s0x%.4X%s\n",
        text_color_grey,
        text_color_white,
        adc_reading,
        text_color_reset
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
        .passed = true
        /* .pass_fail = "PASS" */
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
        .passed = true
        /* .pass_fail = "PASS" */
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
void SpiMasterReadFakeSensorData(void)
{
    // TODO: split frame in half, use this function to read from the slave a
    // half-frame at a time
    SpiMasterWrite(cmd_send_dummy_frame);
    uint8_t * pfake_data = full_frame; uint16_t byte_counter = 0;
    while (byte_counter++ < sizeof_full_frame)
    {
        SpiMasterWaitForResponse(); // Slave signals when the response is ready.
        *(pfake_data++) = SpiMasterRead();
    }
}
void Get_a_frame_from_slave_and_write_frame_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    // This shall all become a `SpiMasterReadSensorData()`.
    // Method 1:
        // spi-master grabs half-frame and sends over USB
        // grabs other half using the same mem
        // purpose: reduce load on SRAM without sacrificing framerate
    /* SpiMasterReadFakeSensorData(); */
    /* UsbWrite(dummy_frame, sizeof_dummy_frame); */
    //
    // Method 2:
        // spi-master grabs a byte and sends over USB until the frame is done
        // purpose: consumes *no SRAM*, but it is slow
    uint16_t nbytes = SpiMasterPassFakeSensorData();
    //
    /* =====[ Test ]===== */
    // Visually confirm debug LED turned red: slave responded with a full frame.
    DebugLedTurnRed();
    /* =====[ Test ]===== */
    // USB host reads the frame.
    // ```python REPL
    // a = s.read(s.inWaiting())
    // ```
    // Visually confirm 1540 bytes of repeated ABCs, ending in letter `F`
    TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "Get_a_frame_from_slave_and_write_frame_to_USB_host"
    };
    this_test.passed = (nbytes == sizeof_full_frame);
    PrintTestResultInColor(this_test);
    // If test fails, report number of bytes.
    if (!this_test.passed) PrintSizeOfSpiSlaveResponse(nbytes);
}
void Slave_ignores_cmd_slave_ignore(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(slave_ignore);
    DebugLedTurnRed(); // for manual testing
    // Visually confirm the debug LED turns red.
    // Visually confirm the red slave debug LEDs are 1 and 3
    // Expect 1 red:    hears master
    // Expect 2 green:  sent a response
    // Expect 3 red:    done responding
    // Expect 4 green:  unknown command
}
void Slave_indicates_unknown_cmd_on_led_4(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    // Send test_unknown_cmd to see slave's response to an unknown command.
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(test_unknown_cmd);
    DebugLedTurnRed(); // for manual testing
    // Visually confirm the debug LED turns red.
    // Visually confirm the red slave debug LEDs are 1, 3, and 4
    // Expect 1 red:    hears master
    // Expect 2 green:  sent a response
    // Expect 3 red:    done responding
    // Expect 4 green:  unknown command
}
void test_SpiMaster(void)
{
    /* Slave_receives_request_and_sends_response_when_ready(); // PASS 2018-08-02 */
    /* SpiMaster_detects_when_slave_is_ready_to_send_data();  // PASS 2018-08-03 */
    /* Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host(); // PASS 2018-08-08 */
    /* Get_several_bytes_from_slave_and_write_bytes_to_USB_host(); // PASS 2018-08-08 */
    /* Get_a_frame_from_slave_and_write_frame_to_USB_host(); // PASS 2018-08-15 */
    /* Slave_ignores_cmd_slave_ignore();  // PASS 2018-08-09 */
    /* Slave_indicates_unknown_cmd_on_led_4(); // PASS 2018-08-09 */
}

/* =====[ The automated testing starts here ]===== */
void SpiMaster_get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(cmd_send_dummy_byte);
    SpiMasterWaitForResponse(); // Slave signals when the response is ready.
    uint8_t response = SpiMasterRead();
    /* =====[ Test ]===== */
    // USB host reads the byte.
    // ```python REPL
    // a = s.read(s.inWaiting())
    // ```
    TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "SpiMaster_get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host"
    };
    bool assert_true = (response == cmd_send_dummy_byte);
    TestResult *result = &this_test; RunTest(result, assert_true);
    // Expect `0x01`.
    // If test fails, print the received byte. `0x01` prints as a smiley face.
    if (Failed(result)) PrintSpiSlaveResponseInColor(response);

    /* this_test.passed = (response == cmd_send_dummy_byte); */
    /* PrintTestResultInColor(this_test); */
    /* // Expect `0x01`. */
    /* // If test fails, print the byte sent. `0x01` prints as a smiley face. */
    /* if (!this_test.passed) PrintSpiSlaveResponseInColor(response); */
}

void SpiMaster_get_a_frame_from_slave_and_write_frame_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    uint16_t nbytes = SpiMasterPassFakeSensorData();
    //
    /* =====[ Test ]===== */
    // USB host reads the frame.
    // ```python REPL
    // a = s.read(s.inWaiting())
    // ```
    TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "Get_a_frame_from_slave_and_write_frame_to_USB_host"
    };
    bool assert_true = (nbytes == sizeof_full_frame);
    TestResult *result = &this_test; RunTest(result, assert_true);
    // Expect 1540 bytes of repeated ABCs, ending in letter `F`
    if (Failed(result)) PrintSizeOfSpiSlaveResponse(nbytes);
}
void SpiMaster_get_adc_reading_from_slave_and_write_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(cmd_send_adc_reading);
    uint8_t adc_bytes[2]; uint8_t * padc_bytes = adc_bytes;
    uint16_t nbytes = sizeof(adc_bytes);
    uint16_t byte_counter = 0;
    while (byte_counter++ < nbytes)
    {
        SpiMasterWaitForResponse(); // Slave signals when the response is ready.
        *(padc_bytes++) = SpiMasterRead();
    }
    uint16_t adc_reading = (adc_bytes[0] << 8) + adc_bytes[1];
    PrintAdcReading(adc_reading);
    DebugLedTurnRed();

    TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "SpiMaster_get_adc_reading_from_slave_and_write_to_USB_host"
    };
    bool assert_true = (nbytes == 2);
    TestResult *result = &this_test; RunTest(result, assert_true);
    if (Failed(result)) PrintAdcReading(adc_reading);
}
void SpiMaster_get_fake_adc_reading_from_slave_and_write_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(cmd_send_fake_adc_reading);
    uint8_t adc_bytes[2]; uint8_t * padc_bytes = adc_bytes;
    uint16_t nbytes = sizeof(adc_bytes);
    uint16_t byte_counter = 0;
    while (byte_counter++ < nbytes)
    {
        SpiMasterWaitForResponse(); // Slave signals when the response is ready.
        *(padc_bytes++) = SpiMasterRead();
    }
    uint16_t adc_reading = (adc_bytes[0] << 8) + adc_bytes[1];
    DebugLedTurnRed();

    TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "SpiMaster_get_fake_adc_reading_from_slave_and_write_to_USB_host"
    };
    bool assert_true = (adc_reading == fake_adc_reading);
    TestResult *result = &this_test; RunTest(result, assert_true);
    if (Failed(result)) PrintAdcReading(adc_reading);
}

void SpiMaster_get_frame_of_adc_readings_and_write_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    uint16_t nbytes = SpiMasterPassAdcFrame();

    TestResult this_test = {
        .pcb_name = "simBrd",
        .test_name = "SpiMaster_get_frame_of_adc_readings_and_write_to_USB_host"
    };
    bool assert_true = (nbytes == sizeof_full_frame);
    TestResult *result = &this_test; RunTest(result, assert_true);
    // Expect 1540 bytes of repeated ABCs, ending in letter `F`
    if (Failed(result)) PrintSizeOfSpiSlaveResponse(nbytes);
}
void SpiMaster_get_Lis_frame_and_write_to_USB_host(void)
{
    /* Pairs with App_version_of_Slave_RespondToRequestsForData */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterPassLisFrame();
}
void DoCmdSendFourDummyBytes(void)
{
    SpiMasterWrite(cmd_send_four_dummy_bytes);
    uint8_t fake_data[4]; uint8_t * pfake_data = fake_data;
    uint16_t nbytes = sizeof(fake_data);
    uint16_t byte_counter = 0;
    while (byte_counter++ < nbytes)
    {
        SpiMasterWaitForResponse(); // Slave signals when the response is ready.
        *(pfake_data++) = SpiMasterRead();
    }
    UsbWrite(fake_data, nbytes);
    DebugLedTurnRed();
}
#define DebugPinInit() do { \
    /* =====[ Make SCL an output for debug on the oscilloscope ]===== */ \
    MacroSetBit(&DDRC,  PC5); \
    /* =====[ Init SCL high ]===== */ \
    MacroSetBit(&PORTC, PC5); \
} while (0)
#define DebugPinLow()  MacroClearBit(&PORTC, PC5)
#define DebugPinHigh() MacroSetBit(&PORTC, PC5)
#define MacroDebugLedRed()          MacroSetBit(DebugLed_port, debug_led)
#define MacroDebugLedGreen()        MacroClearBit(DebugLed_port, debug_led)
#define MacroDebugLedToggleColor()  MacroToggleBit(DebugLed_port, debug_led)
void slowSetExposureTime(uint8_t *pnticks)
{
    // Debug: echo the bytes back.
    UsbWrite(pnticks, 2);
    // Pass the exposure time along to the SPI slave.
    SpiMasterWrite(cmd_set_exposure_time);
    SpiMasterWrite(*(pnticks++));
    SpiMasterWrite(*pnticks);
}
// 20 ticks gives the slave 7.5us of Spi_Ss high.
// This is enough time for the slave to prepare to receive the next byte.
// A smaller delay might be possible, but use 20 ticks for safety.
/* #define SpiWriteDelayTicks 20 */
// Use 50 ticks to be extra safe. Why be safe when you can be extra safe.
// OK, well it turns out the large number of ticks is to deal with function
// calls. If I eliminate function calls, do I need any delay?
/* void SetGain5x(void) */
/* { */
/*     MacroSpiMasterWriteAndDelay(cmd_set_gain_5x); */
/* } */

void AutoExpose(void)
{
    // stub the actual call, just test USB communication 
    /* // send dummy result back to host */
    /* uint8_t nticks[2]; */
    /* nticks[0]=0xAB; nticks[1]=0xCD; */
    /* UsbWrite(nticks, 2); */

    // actual call
    MacroSpiMasterWrite(cmd_auto_expose);
    // Slave echoes back the new exposure time value.
    uint8_t nticks[2];
    MacroSpiMasterWaitForResponse();
    MacroSpiMasterWrite(slave_ignore);      // transfer msb
    nticks[0] = *Spi_spdr;               // store msb
    MacroSpiMasterWaitForResponse();
    MacroSpiMasterWrite(slave_ignore);      // transfer lsb
    nticks[1] = *Spi_spdr;               // store lsb
    // Debug: echo the bytes back.
    UsbWrite(nticks, 2);
}
void SetExposureTime(uint8_t *pnticks)
{
    /* // Debug: echo the bytes back. */
    /* UsbWrite(pnticks, 2); */
    // Pass the exposure time along to the SPI slave.
    // Delay after each write to let the slave catch-up before the next byte.
    MacroSpiMasterWriteAndDelay(cmd_set_exposure_time);
    MacroSpiMasterWriteAndDelay(*(pnticks++));
    MacroSpiMasterWrite(*(pnticks--));
    // Slave echoes back the new exposure time value.
    MacroSpiMasterWaitForResponse();
    MacroSpiMasterWrite(slave_ignore);      // transfer msb
    *(pnticks++) = *Spi_spdr;               // store msb
    MacroSpiMasterWaitForResponse();
    MacroSpiMasterWrite(slave_ignore);      // transfer lsb
    *(pnticks--) = *Spi_spdr;               // store lsb
    // Debug: echo the bytes back.
    UsbWrite(pnticks, 2);
}
void SpiMaster_pass_commands_from_USB_Host_pass_data_from_slave(void)
{
    while(1) // loop forever responding to the USB host
    {
        uint8_t cmd;
        if (UsbReadOneByte(&cmd))
        {
            if (cmd == cmd_write_cfg_to_lis)
            {
              /* TODO: change this to two bytes: */
              /* bin_on_off : 1 (on or off) */
              /* gain_select: 2 (2 bits for four values) */
              /* row_select: 5 (1 bit per row) */
                // Expect three more bytes: bin on/off, gain, row select
                typedef struct {
                    uint8_t bin_on_off;
                    uint8_t gain_select;
                    uint8_t row_select;
                } CfgParams_s;
                CfgParams_s cfg;

                /* Wait until each byte is available, then read it. */
                // TODO: add timeout to while looping
                while (!UsbReadOneByte(&cfg.bin_on_off));
                while (!UsbReadOneByte(&cfg.gain_select));
                while (!UsbReadOneByte(&cfg.row_select));
                // TODO: guard against invalid bytes
                    // exit if any byte is not valid
                // TODO: gain and row select are themselves structs
                /* =====[ Quick dumb test for now ]===== */
                if (cfg.bin_on_off == cmd_cfg_summing_mode_on)
                {
                    MacroSpiMasterWriteAndDelay(cmd_cfg_summing_mode_on);
                }
                if (cfg.gain_select == cmd_cfg_lis_gain_1x)
                {
                    MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_1x);
                }
                if (cfg.row_select == cmd_cfg_lis_rowselect_12345)
                {
                    MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_12345);
                }
/* TODO: the if == blah then do this command move to a jump-table */
/* TODO: turn cmd parameters directly into global state where applicable */
/* result: there will be *no* conditionals here */
            }
        }
    }
}

void SpiMaster_pass_commands_from_USB_Host_pass_data_from_slave_old(void)
{
    while(1) // loop forever responding to the USB host
    {
        if (UsbHasDataToRead())
        {
            /* MacroDebugLedToggleColor(); */
            /* MacroDebugLedRed(); */
            uint8_t read_buffer[4];
            uint8_t nbytes_in_cmd = UsbRead(read_buffer);
            uint8_t cmd = read_buffer[0];
            if (1 == nbytes_in_cmd)
            {
                if      (cmd == cmd_send_lis_frame) { MacroDebugLedRed(); SpiMasterPassLisFrame(); MacroDebugLedGreen(); }
                // for using Arduino as the SPI master via the ISP header:
                else if (cmd == cmd_disable_spi_master) { SpiMasterDisable(); MacroDebugLedRed(); }
                else if (cmd == cmd_enable_spi_master)  { SpiMasterInit(); MacroDebugLedGreen(); }
                // test commands
                else if (cmd == cmd_send_four_dummy_bytes) DoCmdSendFourDummyBytes();
                else if (cmd == cmd_auto_expose) AutoExpose();
            }
            else if (2 == nbytes_in_cmd)
            {
                uint8_t gain_setting = read_buffer[1];
                if      (gain_setting == cmd_cfg_lis_gain_5x)  MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_5x);
                else if (gain_setting == cmd_cfg_lis_gain_4x)  MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_4x);
                else if (gain_setting == cmd_cfg_lis_gain_2pt5x)  MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_2pt5x);
                else if (gain_setting == cmd_cfg_lis_gain_1x)  MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_1x);
                if (cmd == cmd_write_cfg_to_lis) MacroSpiMasterWriteAndDelay(cmd_write_cfg_to_lis);
            }
            else if (3 == nbytes_in_cmd)
            {
                uint8_t *pnticks = &read_buffer[1]; // two bytes of nticks_exposure
                if      (cmd == cmd_set_exposure_time) SetExposureTime(pnticks);
            }
            else if (4 == nbytes_in_cmd)
            {
                uint8_t summing_mode    = read_buffer[1];
                uint8_t gain_setting    = read_buffer[2];
                uint8_t rowselect       = read_buffer[3];
                if      (summing_mode == cmd_cfg_summing_mode_on)   MacroSpiMasterWriteAndDelay(cmd_cfg_summing_mode_on);
                else                                                MacroSpiMasterWriteAndDelay(cmd_cfg_summing_mode_off);
                if      (summing_mode == cmd_cfg_summing_mode_on)   nbytes_in_frame = sizeof_half_frame;
                else                                                nbytes_in_frame = sizeof_full_frame;
                if      (gain_setting == cmd_cfg_lis_gain_5x)       MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_5x);
                else if (gain_setting == cmd_cfg_lis_gain_4x)       MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_4x);
                else if (gain_setting == cmd_cfg_lis_gain_2pt5x)    MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_2pt5x);
                else                                                MacroSpiMasterWriteAndDelay(cmd_cfg_lis_gain_1x);
                if      (rowselect == cmd_cfg_lis_rowselect_1)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_1);
                else if (rowselect == cmd_cfg_lis_rowselect_2)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_2);
                else if (rowselect == cmd_cfg_lis_rowselect_3)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_3);
                else if (rowselect == cmd_cfg_lis_rowselect_4)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_4);
                else if (rowselect == cmd_cfg_lis_rowselect_5)      MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_5);
                else if (rowselect == cmd_cfg_lis_rowselect_12)     MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_12);
                else if (rowselect == cmd_cfg_lis_rowselect_123)    MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_123);
                else if (rowselect == cmd_cfg_lis_rowselect_1234)   MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_1234);
                else if (rowselect == cmd_cfg_lis_rowselect_124)    MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_124);
                else                                                MacroSpiMasterWriteAndDelay(cmd_cfg_lis_rowselect_12345);
                if (cmd == cmd_write_cfg_to_lis) MacroSpiMasterWriteAndDelay(cmd_write_cfg_to_lis);
            }
            /* MacroDebugLedGreen(); */
        }
    }
}
#define UsbWriteDelayTicks 1
#define MacroUsbWriteByte(pbyte) do { \
    /* =====[ FtSendCommand(FtCmd_Write); \ ]===== */ \
    /* FtActivateInterface(); \ */ \
    MacroClearBit(Ft1248_port, Ft1248_Ss); \
    Delay3CpuCyclesPerTick(UsbWriteDelayTicks); \
    /* FtPushData(); \ */ \
    MacroSetBit(Ft1248_port, Ft1248_Sck); \
    /* FtLetMasterDriveBus(); \ */ \
    *FtMiosio_ddr = 0xFF; \
    /* FtOutputByte(FtCmd); \ */ \
    *FtMiosio_port = FtCmd_Write; \
    /* FtPullData(); \ */ \
    MacroClearBit(Ft1248_port, Ft1248_Sck); \
    /* if (!FtBusTurnaround()) DebugLedTurnRedToShowError(); \ */ \
    /* FtLetSlaveDriveBus(); */ \
    *FtMiosio_ddr = 0x00; \
    /* FtPushData(); \ */ \
    MacroSetBit(Ft1248_port, Ft1248_Sck); \
    /* FtPullData(); \ */ \
    MacroClearBit(Ft1248_port, Ft1248_Sck); \
    /* return FtIsBusOk(); */ \
    /* =====[ If bus is not OK, turn LED red and do nothing ]===== */ \
    if (!MacroBitIsClear(Ft1248_pin, Ft1248_Miso)) MacroDebugLedRed(); \
    else \
    { \
    /* =====[ FtWrite(pbyte); ]===== */ \
    /* FtLetMasterDriveBus(); \ */ \
    *FtMiosio_ddr = 0xFF; \
    /* FtPushData(); \ */ \
    MacroSetBit(Ft1248_port, Ft1248_Sck); \
    /* FtWriteData(*write_buffer); */ \
    *FtMiosio_port = *pbyte; \
    /* FtPullData(); \ */ \
    MacroClearBit(Ft1248_port, Ft1248_Sck); \
    /* if (!FtIsBusOk()) return false; */ \
    /* =====[ If write failed, turn LED red ]===== */ \
    if (!MacroBitIsClear(Ft1248_pin, Ft1248_Miso)) MacroDebugLedRed(); \
    } \
    /* =====[ No matter what happened, deactivate the interface. ]===== */ \
    /* FtDeactivateInterface(); \ */ \
    MacroSetBit(Ft1248_port, Ft1248_Ss); \
} while (0)
int main()
{
    //
    // Pick one test group to run.
    // Uncomment that test group.
    // Leave the other test groups commented out.
    //
    SetupDebugLed();
    /* test_EchoByte(); // All tests pass 2018-07-30 */
    /* test_SpiMaster(); // All test pass 2018-08-15 */
    /* =====[ Automated tests ]===== */
    /* SpiMaster_get_a_frame_from_slave_and_write_frame_to_USB_host(); // prints a lot */
    /* SpiMaster_get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host(); */
    /* SpiMaster_get_fake_adc_reading_from_slave_and_write_to_USB_host(); */
    /* SpiMaster_get_adc_reading_from_slave_and_write_to_USB_host(); */
    /* SpiMaster_get_frame_of_adc_readings_and_write_to_USB_host(); */
    /* SpiMaster_get_Lis_frame_and_write_to_USB_host(); */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    DebugPinInit();
    nbytes_in_frame = sizeof_full_frame; // initialize global `nbytes_in_frame`
    SpiMaster_pass_commands_from_USB_Host_pass_data_from_slave();
}
uint16_t SpiMasterPassFakeSensorData(void)
{
    // TODO: refactor the loop to eliminate repetition outside the loop
    // Pass each byte out as soon as you get it.
    // Return the number of bytes passed from spi-slave to USB host.
    SpiMasterWrite(cmd_send_dummy_frame);
    uint16_t byte_counter = 0;
    uint8_t byte_buffer;
    SpiMasterWaitForResponse(); // Slave signals when the response is ready.
    while (++byte_counter < sizeof_full_frame)
    {
        byte_buffer = SpiMasterRead(); // read first byte
        // must look for slave response right away or you'll miss it
        SpiMasterWaitForResponse(); // Slave signals the 2nd byte is ready
        UsbWrite(&byte_buffer, 1); // send the first byte out before reading the next
    }
    byte_buffer = SpiMasterRead(); // read last byte
    UsbWrite(&byte_buffer, 1); // send last byte out
    return byte_counter;
}
uint16_t SpiMasterPassAdcFrame(void)
{
    // Like SpiMasterPassFakeSensorData but it sends ADC readings, not ABCs.
    // Host should print raw bytes, not print unicode characters.
    // Pass each byte out as soon as you get it.
    // Return the number of bytes passed from spi-slave to USB host.
    SpiMasterWrite(cmd_send_adc_frame);
    uint16_t byte_counter = 0;
    uint8_t byte_buffer;
    SpiMasterWaitForResponse(); // Slave signals when the response is ready.
    while (++byte_counter < sizeof_full_frame)
    {
        byte_buffer = SpiMasterRead(); // read first byte
        // must look for slave response right away or you'll miss it
        SpiMasterWaitForResponse(); // Slave signals the 2nd byte is ready
        UsbWrite(&byte_buffer, 1); // send the first byte out before reading the next
    }
    byte_buffer = SpiMasterRead(); // read last byte
    UsbWrite(&byte_buffer, 1); // send last byte out
    return byte_counter;
}
uint16_t SpiMasterPassLisFrame(void)
{
    // Send the frame of Lis pixel data.
    // The slave just finished the readout.
    // Fetch each byte from the slave and send it to the USB host.
    // Return the number of bytes passed from spi-slave to USB host.
    DebugPinLow();
    MacroSpiMasterWrite(cmd_send_lis_frame);
    DebugPinHigh();
    uint16_t byte_counter = 0;
    uint8_t byte_buffer;
    MacroSpiMasterWaitForResponse(); // Slave signals when the response is ready.
    DebugPinLow();
    while (++byte_counter < nbytes_in_frame)
    {
        /* =====[ byte_buffer = SpiMasterRead(); // read this byte ]===== */
        MacroSpiMasterWrite(slave_ignore);  // fetch this byte
        byte_buffer = *Spi_spdr;            // store this byte for sending
        MacroSpiMasterWaitForResponse();    // slave signals the next byte is ready
        DebugPinHigh();
        /* =====[ MacroUsbWriteByte time is 4.5us. ]===== */
        MacroUsbWriteByte(&byte_buffer); // send this byte before fetching next
    }
    /* =====[ byte_buffer = SpiMasterRead(); // read last byte ]===== */
    MacroSpiMasterWrite(slave_ignore);      // fetch last byte
    byte_buffer = *Spi_spdr;                // store this byte for sending
    /* =====[ MacroUsbWriteByte time is 4.5us. ]===== */
    MacroUsbWriteByte(&byte_buffer);        // send last byte
    return byte_counter;
}

