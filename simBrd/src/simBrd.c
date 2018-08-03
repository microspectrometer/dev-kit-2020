// This PCB is the bridge from the USB host to the Chromation spectrometer.
//
#include <DebugLed.h>               // controls the debug LED
#include "DebugLed-Hardware.h"      // map debug LED to actual hardware
#include <Ft1248.h>                 // supports Usb -- TODO: does app need this?
#include "Ft1248-Hardware.h"        // map FT1248 (USB) I/O to actual hardware
#include <Usb.h>                    // USB host communication
#include <Spi.h>                    // Chromation spectrometer is a SPI slave
#include "Spi-Hardware.h"           // map SPI I/O to actual hardware
#include "../../mBrd/src/Spi-Commands.h" // commands understood by the SPI slave
#include <ReadWriteBits.h>
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
    // [x] Slave_receives_request_and_sends_response_when_ready
        // Master sends the request.
        // Slave parses the request.
        // Slave signals to master it has a response ready.
        // Master gets response from slave.
    // [x] Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host
    // [ ] Get_several_bytes_from_slave_and_write_bytes_to_USB_host
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
    /* Pairs with App_version_of_Slave_receives_request_without_interrupts */
    /* =====[ Setup ]===== */
    SpiMasterInit();
    UsbInit();
    /* =====[ Operate ]===== */
    SpiMasterWrite(cmd_send_dummy_data_0xDB);
    SpiMasterWaitForResponse(); // Slave signals when the response is ready.
    uint8_t response = SpiMasterRead();
    /* =====[ Test ]===== */
    uint8_t const expected_response = 0xDB;
    if ( response == expected_response )
    {
        DebugLedTurnRed();
        // Visually confirm the debug LED turns red.
        //
        uint8_t write_buffer[] = {response};
        uint16_t num_bytes_to_send = sizeof(write_buffer);
        UsbWrite(write_buffer, num_bytes_to_send);
        // Manually Run host application and confirm master received 0xDB.
    }
    /* =====[ USB Host Application ]===== */
    /* PS> &$python2_os_Windows */
    /* ```python */
    /* import serial */
    /* s=serial.Serial() */
    /* s.baudrate = 9600 */
    /* s.port = 'COM12' */
    /* s.open() */
    /* s.read(s.inWaiting()) */
    /* ``` */
}

void test_SpiMaster(void)
{
    /* SpiMaster_sends_a_byte_and_slave_debug_leds_show_lower_nibble(); // PASS 2018-07-31 */
    /* Slave_receives_request_and_sends_response_when_ready(); // PASS 2018-08-02 */
    Get_dummy_byte_from_slave_and_write_dummy_byte_to_USB_host();
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
    test_SpiMaster(); // All test pass 2018-08-02
}
