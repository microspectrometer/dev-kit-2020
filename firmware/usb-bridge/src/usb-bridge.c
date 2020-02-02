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
  * - If not:
  *   - instead of requiring Windows users to do this step
  *   - see if this has the same effect:
  *   - under `Hardware Specific`, `Port A`, `Driver`
  *   - change `D2XX Direct` to `Virtual COM Port`
  * */
// app libs
#include "Example.h"
// prj libs
#include "BiColorLed.h"
#include <stdlib.h> // defines NULL
// hardware i/o definitions
#include "Hardware.h"

static void setup(void);
static void loop(void);
static void StatusLEDon(void);
int main()
{
    setup();
    while(1) loop();
}
void setup(void)
{
    StatusLEDon();
}
void loop(void)
{
    while (UsbRxbufferIsEmpty());
    // switch(UsbRxbufferPop())
    // {
    //  case 0: NullCommand(); break;
    //  case 8: SetSensorConfig(); break;
    //  default: ReplyCommandInvalid(); break;
}
void StatusLEDon(void)
{
    //! Initialize PCB indicator LED
    BiColorLedOn(status_led);   // sbi	0x07, 3
    BiColorLedGreen(status_led);  // cbi	0x08, 3
}
