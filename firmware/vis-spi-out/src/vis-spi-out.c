// app libs
#include "Example.h"
// prj libs
#include "BiColorLed.h"
#include "SpiSlave.h"
// hardware i/o definitions
#include "Hardware.h"
#include "Queue.h"

// Allocate memory for the FIFO SPI Buffer
volatile Queue_s * SpiFifo;
// Define maximum size (bytes) of the SPI Rx Buffer
#define max_length_of_queue 11 // bytes
volatile uint8_t spi_rx_buffer[max_length_of_queue];

static void setup(void);
static void loop(void);
int main()
{
    setup();
    while(1) loop();
}
void setup(void)
{
    // Initialize PCB indicator LEDs
    BiColorLedOn(led_0); // sbi	0x07, 0
    BiColorLedOn(led_1); // sbi	0x07, 1
    BiColorLedGreen(led_0); // cbi	0x08, 0
    BiColorLedGreen(led_1); // cbi	0x08, 1
    // Configure as SPI slave, interrupts run `ISR(SPI_STC_vect)`
    SpiSlaveInit(); // call	0xaa	; 0xaa <SpiSlaveInit>
    // Queue incoming SPI bytes in a FIFO buffer.
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Talk to ADC with SPI interface using UART SPIM
    //
    // Power up the linear array and drive with a 50kHz clock
    //
    // Initialize linear array configuration globals
    //
}
// ---Global Flags---
volatile bool byte_received = false;
volatile bool ERROR_byte_lost = false;
void loop(void)
{
    /* example_function(); */
    /* example_inline_function(); */
    //
    // Catch errors
    if (ERROR_byte_lost)
    {
        // TODO: replace with an error handler
        BiColorLedRed(led_0);
    }
    else if (QueueIsFull(SpiFifo))
    {
        // TODO: replace with an error handler
        BiColorLedRed(led_0);
    }
    // Idle until a byte is received from the SPI Master.
    while (!byte_received);
    // Execute the command.
    byte_received = false;
    /* SensorCmd* SensorCmdFn = LookupSensorCmd(*Spi_spdr); */
    /* if (SensorCmdFn == NULL) ReplyCommandInvalid(); */
    /* else SensorCmdFn(); */
    // Why buffer the byte? This makes sense in mid-execution of
    // a command, not when first receiving a command.
    /* // Buffer the received byte in the queue */
    QueuePush(SpiFifo, *Spi_spdr);
    /* // Process the Queue */
    /* if (!QueueIsEmpty(SpiFifo)) */
    /* { */
    /*     // Pop the command and execute it. */
    /*     // placeholder for some code to execute */
    /*     BiColorLedGreen(led_0); // cbi	0x08, 0 */
    /* } */
}
ISR(SPI_STC_vect)
{
    if (byte_received)
    {
        // Another transfer finished before reading previous byte
        ERROR_byte_lost = true;
    }
    else
    {
        // Set flag: received byte over SPI
        byte_received = true;
    }
}
