#include "AvrAsmMacros.h"
/* =====[ Fake the AVR asm macros with empty definitions ]===== */
// sei()
static void GlobalInterruptEnable_Implementation(void) {}
void (*GlobalInterruptEnable)(void) = GlobalInterruptEnable_Implementation;
//
// cli()
static void GlobalInterruptDisable_Implementation(void) {}
void (*GlobalInterruptDisable)(void) = GlobalInterruptDisable_Implementation;


