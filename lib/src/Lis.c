#include "Lis.h"
#include "Pwm.h"
#include "ReadWriteBits.h"

void LisWriteCfg(uint32_t cfg)
{
    /* Cfg needs 28 bits: use 4 bytes */
    /* bit 0: bin_on_off */
    /*     bin_on: bin_on_off = 1 */
    /* bit 1: gain bit G2 */
    /* bit 2: gain bit G1 */
    /*     gain_1x: G2 G1 = 0 0 */
    /* next are 25 bits to select pixels by row in groups of 154 */
    /* use these 25 bits to select entire rows (groups of 784) */
    EnterLisProgrammingMode();
    uint8_t ncfgbits = 28;
    for (uint8_t cfgbit_i = 0; cfgbit_i < ncfgbits; cfgbit_i++)
    {
        LoadNextCfgBit(cfg);
        /* Then wait for the bit to clock in before loading the next bit. */
        LisWaitForClkRiseEdge(); // bit is read on rising edge
        LisWaitForClkFallEdge(); // hold bit until falling edge
    }
    ExitLisProgrammingMode();
}

static inline void LoadNextCfgBit_Implementation(uint32_t cfg)
{
// TODO: is this still going to be inlined now that the call must go through the
// function pointer?
/* expect this function to be inlined when called within this translation unit */
/* to allow for inlining *outside*: */
/*     - move this body to the header */
/*     - move declaration into this file */
/*     - prefix name with `Lis` */
    if (cfg & (1<<0)) { MacroSetBit(Lis_port1, Lis_Rst); }
    else { MacroClearBit(Lis_port1, Lis_Rst); }
}
void (*LoadNextCfgBit)(uint32_t) = LoadNextCfgBit_Implementation;

inline void EnterLisProgrammingMode(void)
{
    /* Do all setup of Lis_Rst while Lis_Clk is low */
    LisWaitForClkFallEdge();
    /* Assert Pix_Select to program the LIS */
    MacroSetBit(Lis_port2, Lis_PixSelect);
}
inline void ExitLisProgrammingMode(void)
{
    /* Reset is low during normal (not programming mode) operation. */
    MacroClearBit(Lis_port1, Lis_Rst);
    /* De-assert Pix_Select to stop programming the LIS. */
    MacroClearBit(Lis_port2, Lis_PixSelect);
}

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

