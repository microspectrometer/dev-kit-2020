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
#include "Spi.h"
#include "SpiMaster.h"

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
    // Setup SPI
    SpiMasterInit();
}
void loop(void)
{
    while (UsbRxbufferIsEmpty()); // loop until cmd received
    uint8_t cmd = 0; // initialize cmd as "Null"
    UsbReadByte(&cmd); // read cmd
    switch(cmd) // look up cmd
    {
        /* // Turn status LED red if cmd is not recognized. */
        /* default:                    BiColorLedRed(status_led);   break; */
        // default: ReplyCommandInvalid(); break;
        /* // Null turns status LED green. */
        /* case  0: NullCommand();      BiColorLedGreen(status_led); break; */

        // Write ERROR if the cmd is not recognized.
        default: UsbWriteByte(ERROR); break;
        case  0: NullCommand();  break;
        case  1: GetBridgeLED(); break;
        case  2: SetBridgeLED(); break;
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
        case 15: GetSensorHash(); break;
    }
}
void setup_StatusLED(void)
{
    //! Initialize PCB indicator LED
    BiColorLedOn(status_led);   // sbi	0x07, 3
    BiColorLedGreen(status_led);  // cbi	0x08, 3
}
