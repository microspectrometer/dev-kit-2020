/** \file */
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
/** Clear flag before writing a frame.
 *  Set flag again after writing a frame.
 *  - writing bytes over SPI triggers the SPI ISR
 *  - value received in SPDR is garbage on a write
 *  - if flag is clear, ISR does not push garbage byte onto the
 *  Queue
 *  - two benefits:
 *    - ISR execution time is cut in half
 *    - eliminates QueuePop (to toss garbage byte) after each
 *    write
 * */
volatile bool listening_for_SPIM = true;
void loop(void)
{
    /* example_function(); */
    /* example_inline_function(); */
    //
    // Catch errors
    if (QueueIsFull(SpiFifo))
    {
        // TODO: replace with an error handler
        BiColorLedRed(led_0);
    }
    // Idle until a command is received
    while (QueueIsEmpty(SpiFifo));
    // Execute the command.
    /* SensorCmd* SensorCmdFn = LookupSensorCmd(*Spi_spdr); */
    /* if (SensorCmdFn == NULL) ReplyCommandInvalid(); */
    /* else SensorCmdFn(); */
}
ISR(SPI_STC_vect)
{
    // Push data onto Queue when listening for SPI Master.
    if (listening_for_SPIM) QueuePush(SpiFifo, *Spi_spdr);
}
