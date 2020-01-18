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
// Python-to-Firmware communication status codes
#include "StatusCodes.h"
#include "VisCmd.h"
// LIS-770i configuration
#include "LisConfigs.h"

// Allocate memory for the FIFO SPI Buffer
volatile Queue_s * SpiFifo;
// Define maximum size (bytes) of the SPI Rx Buffer
#define max_length_of_queue 11 // bytes
volatile uint8_t spi_rx_buffer[max_length_of_queue];

static void setup(void);
static void loop(void);
static void IndicatorLEDsOn(void);
int main()
{
    setup();
    while(1) loop();
}
void setup(void)
{
    IndicatorLEDsOn();
    // Configure as SPI slave, interrupts run `ISR(SPI_STC_vect)`
    SpiSlaveInit();
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
    // Catch errors
    if (QueueIsFull(SpiFifo)) // 7 cycles
    {
        // TODO: replace with an error handler
        BiColorLedRed(led_0);
    }
    // Idle until a command is received
    while (QueueIsEmpty(SpiFifo)); // 5 cycles
    // Execute the command.
    switch(QueuePop(SpiFifo))
    {
        case 0: NullCommand(); break;
        case 8: SetSensorConfig(); break;
        default: ReplyCommandInvalid(); break;
        // ---Expected Assembly---
        // Context:
        // 1. 0x19e is the start of loop()
        // 2. QueuePop puts `cmd` byte in r24
        // Handle case 0:
        //  1de:	and	r24, r24; `cmd` is in r24
        //  1e0:	breq	.-68     	; 0x19e <main+0xf8>
        // Handle case 8:
        //  1e2:	cpi	r24, 0x08	; 8
        //  1e4:	breq	.+34     	; 0x208 <main+0x162>
        // Default case:
        //  1e6:	out	0x2e, r19	; 46
        // Total number of cycles: 7
        // Total number of instructions: 5
    }
}
ISR(SPI_STC_vect)
{
    //! Interrupt disabled during during `SpiSlaveTxByte`.
    //! Interrupt enabled all other times.
    QueuePush(SpiFifo, *Spi_SPDR);
}
void IndicatorLEDsOn(void)
{
    //! Initialize PCB indicator LEDs
    BiColorLedOn(led_0); // sbi	0x07, 0
    BiColorLedOn(led_1); // sbi	0x07, 1
    BiColorLedGreen(led_0); // cbi	0x08, 0
    BiColorLedGreen(led_1); // cbi	0x08, 1
}
