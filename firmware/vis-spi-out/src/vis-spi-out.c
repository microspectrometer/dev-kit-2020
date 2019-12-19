/** \file */
// app libs
#include "Example.h"
// prj libs
#include "BiColorLed.h"
#include "SpiSlave.h"
#include "Cmd.h"
#include <stdlib.h> // defines NULL
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
    Cmd* DoSensorCmd = LookupSensorCmd(*Spi_spdr);
    if (SensorCmdFn == NULL) ReplyCommandInvalid();
    else DoSensorCmd();
    DoSensorCmd();
}
ISR(SPI_STC_vect)
{
    QueuePush(SpiFifo, *Spi_spdr);
}
Cmd* LookupSensorCmd(cmd const key)
{
    /** Look up command and return a pointer to the function
     *  that does the command.
     *  - input key is defined in the USBProtocol
     *  - bob
     * */
    // pf is an array of pointers to SensorCmd functions
    // pf lives in static memory, not on the `LookupSensorCmd` stack frame
    static Cmd* const pf[] = {
        NULL,
        };
    // Return func ptr. Prevent attempts at out-of-bounds access.
    if (key < sizeof(pf)/sizeof(*pf)) return pf[key];
    // Out of bounds keys return a NULL pointer.
    else return NULL; // error
}

