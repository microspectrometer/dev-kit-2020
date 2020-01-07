/** \file */
// app libs
#include "Example.h"
// prj libs
#include "BiColorLed.h"
#include "SpiSlave.h"
#include "UartSpi.h"
#include "Lis.h"
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
static Cmd* LookupSensorCmd(cmd const);
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
    UartSpiInit();
    // Power up the linear array and drive with a 50kHz clock
    LisInit();
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
    // TODO: Read SPDR directly?
    Cmd* DoSensorCmd = LookupSensorCmd(*Spi_SPDR);
    // TODO: Or pop here? Consumes 52 bytes, 38 cycles.
    /* Cmd* DoSensorCmd = LookupSensorCmd(QueuePop(SpiFifo)); */
    /* if (DoSensorCmd == NULL) ReplyCommandInvalid(); */
    // placeholder to test SpiSlaveTx (replace with above line when done)
    if (DoSensorCmd == NULL)
    {
        DisableSpiInterrupt();
        uint8_t const input_buffer[] = {0xFA, 0xFB, 0xFC};
        uint16_t nbytes = 3;
        SpiSlaveTx(input_buffer, nbytes);
    }
    else DoSensorCmd();
}
ISR(SPI_STC_vect)
{
    // TODO: add GPIOR flag to check if listening?
    // Skipping QueuePush saves 61 out of 116 cycles.
    // Or enable/disable SpiInterrupt?
    QueuePush(SpiFifo, *Spi_SPDR);
}
Cmd* LookupSensorCmd(cmd const key)
{
    /** Return a pointer to the function that does command
     * identified by input `key`.
     *  - input `key` is defined in the USBProtocol
     *  - return `NULL` if `key` is not in the lookup table
     * */
    // pf is an array of pointers to SensorCmd functions
    // pf is in static memory, not in the stack frame
    static Cmd* const pf[] = {
        NULL,
        };
    // Return func ptr. 
    if (key < sizeof(pf)/sizeof(*pf)) return pf[key];
    // Out of bounds keys return a NULL pointer.
    else return NULL; // error
}

