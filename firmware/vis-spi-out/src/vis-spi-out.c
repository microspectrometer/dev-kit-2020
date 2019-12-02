// avr libs
#include <avr/interrupt.h>      // defines macro ISR()
// app libs
#include "Example.h"

static void loop(void);
int main()
{
    while(1) loop();
}
void loop(void)
{
    example_function();
    example_inline_function();
}
