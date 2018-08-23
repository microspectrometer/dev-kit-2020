#include "Lis.h"
#include "Pwm.h"
#include "ReadWriteBits.h"

static void SetPixSelectAsOutput(void)
{
    SetBit(Lis_ddr2, Lis_PixSelect);
}
static void SetClkAsOutput(void)
{
    SetBit(Lis_ddr1, Lis_Clk);
}
static void SetRstAsOutput(void)
{
    SetBit(Lis_ddr1, Lis_Rst);
}
static void SetSyncAsInput(void)
{
    ClearBit(Lis_ddr1, Lis_Sync);
}
static void PixSelectIdleLow(void)
{
    ClearBit(Lis_port2, Lis_PixSelect);
}
static void RstIdleLowAfterPowerUp(void)
{
    ClearBit(Lis_port1, Lis_Rst);
}
void LisInit(void)
{
    SetPixSelectAsOutput();
    PixSelectIdleLow();
    SetClkAsOutput();
    SetRstAsOutput();
    RstIdleLowAfterPowerUp();
    SetSyncAsInput();
    LisRunClkAt50kHz();
    LisClkOn();
}
static void LisRunClkAt50kHz_Implementation(void)
{
    PwmResetCounterAtTop();
    PwmTopIsOcr0a();
    PwmClkIsCpuClk();
    *Lis_clktop = 200;           // 10MHz / 50kHz = 200 ticks
    *Lis_clkth = *Lis_clktop/2;  // 50% dutcy cycle: PASS 2018-08-21
    /* *Lis_clkth = *Lis_clktop/10; // 10% dutcy cycle: PASS 2018-08-21*/
}
void (*LisRunClkAt50kHz)(void) = LisRunClkAt50kHz_Implementation;

static void LisClkOn_Implementation(void)
{
    PwmEnableOutputSetUntilMatch();
}
void (*LisClkOn)(void) = LisClkOn_Implementation;

static void ClkIdleLow(void)
{
    ClearBit(Lis_port1, Lis_Clk);
}
void LisClkOff(void)
{
    ClkIdleLow();
    PwmDisableOutput();
}
bool Lis_exposure_is_done = true;
void LisExpose(uint16_t nticks)
{
    // Abandoned. Dev done directly on embedded target.
    // Function call/return overhead is too slow to use function calls.
    // If I had more time, I'd develop with function calls then insert macro
    // substitutions, but I do not. The major time sink there is all the
    // stubbing to fake the hardware behavior in flag checking and clearing.
    // Stubbing requires creating functions for those behaviors. And that
    // requires cleverly bypassing all those functions with the macros. I need
    // to move quick, so I am short-circuiting all of that by working directly
    // in macros and manually confirming behavior by measuring on an
    // oscilloscope.
    nticks++;
}
