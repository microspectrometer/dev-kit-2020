#include "AvrAsmMacros.h"
#include <avr/interrupt.h>      // defines macros `sei()` and `cli()`

// sei()
static void GlobalInterruptEnable_Implementation(void)
{ sei(); }
void (*GlobalInterruptEnable)(void) = GlobalInterruptEnable_Implementation;

// cli()
static void GlobalInterruptDisable_Implementation(void)
{ cli(); }
void (*GlobalInterruptDisable)(void) = GlobalInterruptDisable_Implementation;


