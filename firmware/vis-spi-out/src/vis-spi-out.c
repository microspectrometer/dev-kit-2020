// app libs
#include "Example.h"
// prj libs
#include "BiColorLed.h"
// hardware i/o definitions
#include "Hardware.h"

void setup(void);
static void loop(void);
int main()
{
    setup();
    while(1) loop();
}
void setup(void)
{
    BiColorLedOn(led_0);
}
void loop(void)
{
    example_function();
    example_inline_function();
}
