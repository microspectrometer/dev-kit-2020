/** \file
  * **Required Hardware Settings in FT_Prog**\n 
  * - Hardware Specific -> Ft1248 Settings
  *   - Clock Polarity High: **unchecked**
  *   - Bit Order LSB: **unchecked**
  *   - Flow Ctrl not selected: **checked**
  * - serial number: `CHROMATION123456`
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
{}
void StatusLEDon(void)
{
    //! Initialize PCB indicator LED
    BiColorLedOn(status_led);    // sbi	0x07, 3
    BiColorLedGreen(status_led); // cbi	0x08, 3
}
