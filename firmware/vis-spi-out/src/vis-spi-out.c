/** \file */
// app libs
#include "Example.h"
// prj libs
#include "BiColorLed.h"
#include "Spi.h"
#include "SpiSlave.h"
#include "UartSpi.h"
#include "Lis.h"
// #include "Cmd.h" - old typedefs from jump table days
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

//! Allocate static memory for the SPI Rx Queue.
volatile Queue_s * SpiFifo;
//! Maximum size of the Queue's FIFO buffer is 24 bytes.
#define max_length_of_queue 24 // bytes
//! Allocate static memory for the Queue's FIFO buffer.
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
    // BUSY
    BiColorLedRed(led_0);
    switch(QueuePop(SpiFifo))
    {
        case  0: NullCommand(); break;
        case  3: GetSensorLED(); break;
        case  4: SetSensorLED(); break;
        case  7: GetSensorConfig(); break;
        case  8: SetSensorConfig(); break;
        case  9: GetExposure(); break;
        case 10: SetExposure(); break;
        case 11: CaptureFrame(); break;
        case 12: AutoExposure(); break;
        case 13: GetAutoExposeConfig(); break;
        case 14: SetAutoExposeConfig(); break;
        /* default: ReplyCommandInvalid(); break; */
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
    // DONE
    BiColorLedGreen(led_0);
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
    /** Setup control of the **ADC** and the **LIS-770i
     * detector**.
     * - Configure UART module as a **SPI Master** to the ADC:
     *   - use a **5MHz clock** to pull bits out of ADC
     *   - pin `UartSpi_AdcConv`: **output**, idle `LOW`
     *   - see details in UartSpiInit()
     * - Configure I/O pins to control LIS-770i detector:
     *   - clock LIS-770i at **50kHz**
     *   - pins `Lis_PixSelect` and `Lis_Rst`: **output**, idle `LOW`
     *   - see details in LisInit()
     * - Set initial exposure time to 1 millisecond.
     * - Set initial LIS-770i configuration:
     *   - binning on
     *   - gain 1x
     *   - all rows active
     *   - see LIS-770i cfg byte code definitions in LisConfigs.h
     * - Program LIS-770i with initial configuration.
     * */
    // Talk to ADC with SPI interface using UART SPIM
    UartSpiInit();
    // Power up the LIS-770i and drive with a 50kHz clock
    LisInit();

    // Initialize exposure time to 1 millisecond
    exposure_ticks = 50; // 50 ticks = (1.0e-3 s)/(20.0e-6 s/tick)
    // ---Expected Assembly---
    // ldi	r24, 0x32	; 50
    // ldi	r25, 0x00	; 0
    // sts	0x011D, r25	; 0x80011d <exposure_ticks+0x1>
    // 1c8:	sts	0x011C, r24	; 0x80011c <exposure_ticks>

    // Initialize LIS-770i configuration globals
    binning = BINNING_ON;
    gain = GAIN_1X;
    active_rows = ALL_ROWS_ACTIVE;
    // ---Expected Assembly---
    // ldi	r24, 0x01	; 1
    // sts	0x011E, r24	; 0x80011e <binning>
    // sts	0x0140, r24	; 0x800140 <gain>
    // ldi	r24, 0x1F	; 31
    // sts	0x013B, r24	; 0x80013b <active_rows>

    // Program LIS-770i with above configuration
    LisWriteConfig();

    /* ------------------------------- */
    /* | Initialize AutoExposeConfig | */
    /* ------------------------------- */
    // Initialize default auto-expose maximum number of tries
    max_tries = 12;
    // Initialize default auto-expose pixel range to all 392 pixels.
    // Recommend user application trims pixel range to match wavelength map.
    start_pixel = 7; // first 6 pixels are optically meaningless
    stop_pixel = 392;
    // Initialize default auto-expose target peak range to 46420 ± 3277 counts
    target = 46420;
    target_tolerance = 3277;
    // Hard-code conservative estimate on dark offset
    // AutoExpose calculates gain ONLY when signal is above max_dark.
    // SetAutoExposeConfig guarantees target is not below max_dark.
    max_dark = 4500;
    // Hard-code minimum exposure time used by auto-expose.
    // keep lower limit well-away from 1 cycle
    min_exposure = 5; // cycles
    // Initialize default auto-expose maximum exposure time to try.
    // upper limit is 1.3s, but 200ms is a practical default limit
    max_exposure = 10000; // cycles
}
