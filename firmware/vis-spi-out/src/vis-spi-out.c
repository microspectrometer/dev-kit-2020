/** \file */
// app libs
#include "Example.h"
// prj libs
#include "BiColorLed.h"
#include "Spi.h"
#include "SpiSlave.h"
#include "UartSpi.h"
#include "Lis.h"
#include "Cmd.h"
#include <stdlib.h> // defines NULL
// hardware i/o definitions
#include "Hardware.h"
// SPI communication queue
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
static void setup_IndicatorLEDs(void);
static void setup_SpiCommunication(void);
static void setup_DetectorReadout(void);
int main()
{
    setup();
    while(1) loop();
}
void setup(void)
{
    setup_IndicatorLEDs();
    setup_SpiCommunication();
    setup_DetectorReadout();
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
        case 3: GetSensorLED(); break;
        case 4: SetSensorLED(); break;
        case 7: GetSensorConfig(); break;
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
ISR(SPI_STC_vect) // Serial Transfer Complete
{
    /** Interrupt when a SPI transfer completes:\n
     * - push received byte onto SpiFifo (Queue head advances)\n
     * - return from interrupt\n
     *\n
     * **Interrupt disabled** during `SpiSlaveTxByte`.\n
     * **Interrupt enabled** all other times.
     * */
    QueuePush(SpiFifo, *Spi_SPDR);
}
void setup_IndicatorLEDs(void)
{
    //! Initialize PCB indicator LEDs
    BiColorLedOn(led_0); // sbi	0x07, 0
    BiColorLedOn(led_1); // sbi	0x07, 1
    BiColorLedGreen(led_0); // cbi	0x08, 0
    BiColorLedGreen(led_1); // cbi	0x08, 1
}
void setup_SpiCommunication(void)
{
    /** Set up **SPI hardware module** and a **FIFO** to buffer
     * bytes sent from the SPI master.\n
     * - Configure SPI hardware module:\n
     *   - configure microcontroller as a **SPI slave**\n
     *   - pin `Spi_DataReady`: **output**, idle `HIGH`\n
     *   - pin `Spi_Miso`: **output**\n
     *   - enable **interrupt** on **SPI Serial Transfer Complete**
     *     (`SPI_STC_vect`)\n
     *   - see details in SpiSlaveInit()\n
     * - Create a **FIFO Queue** to buffer up to *11 bytes* of
     *   SPI data:\n
     *   - see details in Queue_s
     * */
    // Configure as SPI slave, interrupts run `ISR(SPI_STC_vect)`
    SpiSlaveInit();
    // Queue incoming SPI bytes in a FIFO buffer.
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
}
void setup_DetectorReadout(void)
{
    /** setup_ control of the **ADC** and the **LIS-770i
     * detector**.\n
     * - Configure UART module as a **SPI Master** to the ADC:\n
     *   - use a **5MHz clock** to pull bits out of ADC
     *   - pin `UartSpi_AdcConv`: **output**, idle `LOW`\n
     *   - see details in UartSpiInit()\n 
     * - Configure I/O pins to control LIS-770i detector:\n
     *   - clock LIS-770i at **50kHz**\n
     *   - pins `Lis_PixSelect` and `Lis_Rst`: **output**, idle `LOW`\n
     *   - see details in LisInit()\n
     * */
    // Talk to ADC with SPI interface using UART SPIM
    UartSpiInit();
    // Power up the LIS-770i and drive with a 50kHz clock
    LisInit();
    // Initialize LIS-770i configuration globals
    binning = BINNING_ON;
    gain = GAIN_1X;
    active_rows = ALL_ROWS_ACTIVE;
}
