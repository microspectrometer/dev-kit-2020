// libs and the headers that resolve their hardware dependencies
/* #include "SensorVis.h"          // command library for the VIS Sensor */
#include "SensorVis.c"          // command library for the VIS Sensor
#include "Queue.h"              // SPI Rx Buffer is a queue
#include <stdlib.h>             // defines NULL
#include <ReadWriteBits.h>      // SetBit, ClearBit, etc.
#include <BiColorLed.h>         // controls the bicolor LEDs on the PCB
#include "BiColorLed-Hardware.h"// map bicolor LEDs to actual hardware
#include <Spi.h>                // Chromation spectrometer is a SPI slave
#include "Spi-Hardware.h"       // map SPI I/O to actual hardware
#include <UartSpi.h>            // USART in MSPIM mode for ADC readout
#include "UartSpi-Hardware.h"   // map UART MSPIM I/O to actual hardware
#include <Lis.h>                // photodiode array I/O pins and functions
#include "Lis-Hardware.h"       // map photodiode array I/O to actual AVR I/O
/* #include <Pwm.h>                // lib `Lis` uses PWM for the clock signal */
#include "Pwm-Hardware.h"       // map `Pwm` I/O to actual AVR I/O
#include "AvrAsmMacros.h"       // resolve lib dependencies on AVR asm macros
// avr libs
#include <avr/interrupt.h>      // defines macro ISR()

static void Get_commands_from_SpiMaster(void);
/* =====[ Allocate memory for the Spi Rx Queue variables ]===== */
volatile Queue_s * SpiFifo;
// Define the maximum number of bytes the SPI FIFO Rx Buffer can hold.
#define max_length_of_queue 5 // bytes
volatile uint8_t spi_rx_buffer[max_length_of_queue];
/* =====[ Allocate memory for the Photodiode Array Config ]===== */
uint8_t binning; uint8_t gain; uint8_t active_rows;
/* =====[ Allocate memory for the Exposure Time ]===== */
uint16_t exposure_ticks;
/* =====[ Allocate memory for one Frame of pixel data ]===== */
/* #define npixels 784 */
uint8_t frame[npixels*2];

int main()
{
    /* =====[ SETUP ]===== */
    // Turn on LEDs TxRx and Done.
    BiColorLedOn(led_0);
    BiColorLedOn(led_1);
    // Turn both LEDs green.
    BiColorLedGreen(led_0);
    BiColorLedGreen(led_1);
    // Sensor is a SPI slave.
    // See SPI interrupt routine at `ISR(SPI_STC_vect)`.
    SpiSlaveInit();
    // Create a FIFO buffer to queue bytes incoming over SPI.
    SpiFifo = QueueInit(spi_rx_buffer, max_length_of_queue);
    // Use UART to talk to ADC with SPI interface.
    UartSpiInit();
    // Power up the linear array. Start 50kHz clock.
    LisInit();
    /* =====[ Initialize Global SPI Flags and Data Register Buffer ]===== */
    // Use globals because it is an easy way to share data with an ISR.
    /* HasSpiData = false; // global flag to track if there is SpiData */
    /* SpiData = 0x00; // global one-byte register to store SpiData */
    /* =====[ Initialize Globals: Photodiode Array Config ]===== */
    // Use globals because it is an easy way to share data with lib SensorVis
    binning = binning_on; // default to 392 pixels
    gain = gain1x; // default to 1x gain
    active_rows = all_rows_active; // default to using all 5 pixel rows
    exposure_ticks = 50; // multiply by 20us to get integration time in seconds
    /* =====[ LOOP ]===== */
    // Loop forever acting on commands from the SPI Master.
    while(1) Get_commands_from_SpiMaster();
}
void Get_commands_from_SpiMaster(void)
{
    while (QueueIsEmpty(SpiFifo)); // idle until a command is received
    // Queue is no longer empty once a command byte is received over SPI.
    // The SPI ISR pushes the command byte onto the SPI Rx Queue.
    // Pop the command and execute it.
    SensorCmd* SensorCmdFn = LookupSensorCmd(QueuePop(SpiFifo));
    if (SensorCmdFn == NULL) ReplyCommandInvalid();
    else SensorCmdFn();
}
ISR(SPI_STC_vect)
{
    // SPI transfer of one byte just finished.
    // Program counter jumped from "client" code.
    // ISR is the same routine, whether "client" is in a SPI read or a SPI write.

    // Client stuck in loop `while (QueueIsEmpty(SpiFifo));` when ISR is called
    //
    if (QueueIsFull(SpiFifo)) DEBUG_LedsShowError(); // TODO: add error handler
    else
    {
        QueuePush(SpiFifo, *Spi_spdr); // "client" must pop data from SpiFifo queue
    }

    // NOTE:
    // Even though Sensor did not need data, the "client" code is still
    // responsible to pop the garbage byte from the queue.
    //
    // NOTE:
    // The SPI Master *must* look for Spi_DataReady to return HIGH to confirm
    // the SPI Slave finished this receive/transmit *before* the SPI Master
    // checks for Spi_DataReady LOW as a signal that it's OK to start the next
    // transmission.
    //
    // RATIONALE:
    // Calling any non-inline functions in an ISR requires stacking many
    // registers before the ISR C code starts.
    // Even if Spi_DataReady is driven HIGH as the first line of C in the ISR,
    // it does not drive HIGH in time. If SPI Master only looks for
    // Spi_DataReady LOW, it reads a false LOW.
    //
    // The Master is throttled by having to first wait for Spi_DataReady
    // to go HIGH.
    //
    // Driving Spi_DataReady HIGH is safely moved out of the ISR to the client
    // code for execution after returning from the ISR.
    // This throttles the Master even in the corner case where the Master just
    // finished a read and then writes another byte. The Master needs throttling
    // so that it does not send the byte before the Slave has a chance to see
    // the queue is empty. The Slave waits for QueueIsEmpty to be true after
    // writing each byte to the Master (to avoid overwriting SPDR during a
    // transmission). So the Slave would hang if it does not get a chance to see
    // the queue is empty after the final byte is transmitted.
}
