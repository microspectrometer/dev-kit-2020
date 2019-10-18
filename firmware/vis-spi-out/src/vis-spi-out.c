// libs and the headers that resolve their hardware dependencies
#include "SensorVis.h"          // command library for the VIS Sensor
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
#include <Pwm.h>                // lib `Lis` uses PWM for the clock signal
#include "Pwm-Hardware.h"       // map `Pwm` I/O to actual AVR I/O
#include "AvrAsmMacros.h"       // resolve lib dependencies on AVR asm macros
// avr libs
#include <avr/interrupt.h>      // defines macro ISR()

static void Get_commands_from_SpiMaster(void);
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
    // For rx bytes, see SPI interrupt routine at `ISR(SPI_STC_vect)`.
    // For tx bytes, disable SPI interrupts.
    SpiSlaveInit();
    // Use UART to talk to ADC with SPI interface.
    UartSpiInit();
    // Power up the linear array. Start 50kHz clock.
    LisInit();
    /* =====[ Initialize Global SPI Flags and Data Register Buffer ]===== */
    // Use globals because it is an easy way to share data with an ISR.
    HasSpiData = false; // global flag to track if there is SpiData
    SpiData = 0x00; // global one-byte register to store SpiData
    /* =====[ LOOP ]===== */
    // Loop forever acting on commands from the SPI Master.
    while(1) Get_commands_from_SpiMaster();
    // The following line of code should *never* be called.
    LedsShowError();
}
void Get_commands_from_SpiMaster(void)
{
    while (!HasSpiData); // wait until there is SPI data to process
    // The SPI data is a command to execute.
    SensorCmd* SensorCmdFn = LookupSensorCmd(SpiData);
    HasSpiData = false; // consumed the data, so clear the flag
    if (SensorCmdFn == NULL) // Command is invalid.
    {
        ReplyCommandInvalid(); // tell SpiMaster command is invalid
        LedsShowError(); // indicate invalid command error on LEDs
        // No command to execute.
    }
    else // Command is valid.
    {
        SensorCmdFn(); // execute command
        // Command is done.
    }
    // Processor is idle.
}
ISR(SPI_STC_vect)
{
    HasSpiData = true;
    SpiData = *Spi_spdr;
}
