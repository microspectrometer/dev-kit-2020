/** \file
  * **FT_Prog Hardware Settings**\n 
  * - *USB String Descriptors*
  *   - *Manufacturer*: `CH`
  *   - *Product Description*: `ChromationSpect-1234-56`
  *   - *Serial Number*: `CHROMATION123456`
  * - *Hardware Specific*
  *   - *Ft1248 Settings*
  *     - *Clock Polarity High*: `uncheck`
  *     - *Bit Order LSB*: `check`
  *     - *Flow Ctrl not selected*: `check`
  *   - *CBUS Signals*
  *     - *C3*: `GPIO`
  *
  * **Windows Device Manager Settings**\n
  * - Under *Universal Serial Bus controllers*
  * - Right-click *USB Serial Converter*
  *   - select *Properties*
  * - Go to the *Advanced* tab
  *   - *Load VCP*: `check`
  *
  * TODO:\n 
  * - Is *Load VCP* checked by default on a fresh install?
  * */
// app libs
#include "Example.h"
// prj libs
#include "BiColorLed.h"
#include <stdlib.h> // defines NULL
// hardware i/o definitions
#include "Hardware.h"
// Python-to-Firmware communication status codes
#include "StatusCodes.h"
#include "UsbCmd.h"

static void setup(void);
static void loop(void);
static void setup_StatusLED(void);
int main()
{
    setup();
    while(1) loop();
}
void setup(void)
{
    setup_StatusLED();
    // Setup USB
    // TODO: clean this into a unit tested UsbInit() in
    // ./firmware/lib/
    ClearBit(FtCtrl_port, FtMiso); // FtMiso is an input
    SetBit(FtCtrl_port, FtMiso); // pull up FtMiso
    _FtDatabusPinDirection(FtIn); // FtData lines are inputs
    *FtData_port = 0xFF; // pull up FtData lines
    ClearBit(FtCtrl_port, FtClock);  // drive FtClock low
    SetBit(FtCtrl_ddr, FtClock);     // FtClock is an output
    SetBit(FtCtrl_port, FtChipSelect); // drive FtChipSelect high
    SetBit(FtCtrl_ddr, FtChipSelect);  // FtChipSelect is an output
}
void loop(void)
{
    while (UsbRxbufferIsEmpty()); // loop until cmd received
    uint8_t cmd = 0; // initialize cmd as "Null"
    UsbReadByte(&cmd); // read cmd
    switch(cmd) // look up cmd
    {
        // Null gets no response, but turns status LED green for now.
        case 0: NullCommand();      BiColorLedGreen(status_led); break;
        case 1: GetBridgeLED(); break;
        //  case 8: SetSensorConfig(); break;
        // default: ReplyCommandInvalid(); break;
        // Turn status LED red if cmd is not recognized.
        default:                    BiColorLedRed(status_led);   break;
    }
}
void setup_StatusLED(void)
{
    //! Initialize PCB indicator LED
    BiColorLedOn(status_led);   // sbi	0x07, 3
    BiColorLedGreen(status_led);  // cbi	0x08, 3
}
