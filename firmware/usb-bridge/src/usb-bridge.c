// This PCB is the bridge from the USB host to the Chromation spectrometer.
//
#include <BiColorLed.h>               // controls the debug LED
#include "BiColorLed-Hardware.h"      // map debug LED to actual hardware
#include <Ft1248.h>                 // supports Usb -- TODO: does app need this?
#include "Ft1248-Hardware.h"        // map FT1248 (USB) I/O to actual hardware
#include <Usb.h>                    // USB host communication
#include <Spi.h>                    // Chromation spectrometer is a SPI slave
#include "../../vis-spi-out/src/Spi-Hardware.h" // map SPI I/O to actual hardware
#include "../../vis-spi-out/src/Spi-Commands.h" // commands understood by the SPI slave
#include <ReadWriteBits.h>
#include <stdio.h>  // snprintf()
#include <string.h> // strlen()
#include "AvrAsmMacros.h"       // resolve lib dependencies on AVR asm macros
#include "Bridge.h" // bridge cmd stuff

/* =====[ Required Hardware Settings in FT_Prog ]===== */
/* - Hardware Specific -> Ft1248 Settings */
/* - Clock Polarity High: unchecked */
/* - Bit Order LSB: unchecked */
/* - Flow Ctrl not selected: checked */
/* - serial number: CHROMATION123456 */

/* =====[ Cfg on Windows for customer's optional access as a COM Port ]===== */
/* - check box `Load VCP` */

// ---Allocate memory for one frame with no pixel binning---
#define npixels 784
uint8_t frame[npixels*2];

/* =====[ WIP ]===== */
void Bridge_data_flow_between_USB_Host_and_SpiSlave(void)
{
    bridge_cmd_key cmd;
    // Read USB buffer if there is any data to read.
    if (UsbReadOneByte(&cmd))
    {
        // Pass byte to Sensor
        SpiWriteByte(cmd);
        /* This first byte is *always* a command from the USB host. */
        BridgeCmd* CmdFn = LookupBridgeCmd(cmd);
        /* Tell UsbHost if the command is invalid. */
        /* if (CmdFn == NULL) UsbWriteStatusInvalid(cmd); */
        /* if (CmdFn == NULL) UsbWriteStatusInvalid(); */
        if (CmdFn == NULL) FlushInvalidCommand();
        /* Do command if it is valid. */
        else CmdFn();
        /* It is up to CmdFn to let the UsbHost know */
        /* command was successfully carried out. */
    }
}
int main()
{
    /* =====[ Setup ]===== */
    BiColorLedOn(status_led);
    BiColorLedGreen(status_led);
    SpiMasterInit();
    UsbInit();
    /* =====[ Loop ]===== */
    while(1){ Bridge_data_flow_between_USB_Host_and_SpiSlave(); }
}
