/** \file */
#include <stdio.h>
#include "Lis.h"
#include "Pwm.h"
#include "ReadWriteBits.h"
/* =====[ Helper function for LisWriteCfg ]===== */
inline void OutputCfgByte(uint8_t const cfg_byte, uint8_t const nbits)
{
    /** Output `nbits` of cfg_byte, from bit[0] to bit[n-1].
     *  Output values sequentially on pin `Lis_Rst`.
     *  Do nothing if `nbits` is 0 or `nbits` is > 8.
     * */

    // This function does nothing if nbits is out of bounds.
    if ( (nbits < 1) || (nbits > 8) ) return;
    // For each bit in cfg_byte[0:nbits], output masked bit to Lis_Rst.
    for (uint8_t bit_index = 0; bit_index < nbits; bit_index++)
    {
        if ( cfg_byte & (1<<bit_index)) { SetBit(Lis_port1, Lis_Rst); }
        else { ClearBit(Lis_port1, Lis_Rst); }
        // Wait for Lis_Rst value to clock in before loading the next bit.
        LisWaitForClkRiseEdge(); // bit is read on rising edge
        LisWaitForClkFallEdge(); // hold bit until falling edge
    }
}
/* TODO: figure out how the SpiSlave gets cfg from SpiMaster, then come back to this */
void LisWriteCfg(uint8_t const * cfg)
{
    /** Program the Lis with the four `cfg` bytes sent from the SpiMaster.
     *  The programming sequence clocks 28 bits into the LIS-770i.
     *  `cfg` is the 28-bit sequence stored in four bytes.
     *  Bit[0] is bit index 0 of `cfg[3]`.
     *  Bit[27] is bit index 3 of `cfg[0]`.
     *
     *  The 28 bits are output sequentially on Lis_Rst, starting with bit 0.
     *  Bit 0 is binning on/off. Bits 1 and 2 set the gain. The remaining 25
     *  bits select which pixels are active.
     * */

    /* bit 0: bin_on_off */
    /*     bin_on: bin_on_off = 1 */
    /* bit 1: gain bit G2 */
    /* bit 2: gain bit G1 */
    /*     gain_1x: G2 G1 = 0 0 */
    /* next are 25 bits to select pixels by row in groups of 154 */
    /* use these 25 bits to select entire rows (groups of 784) */
    // Plan of attack:
        // Four bytes received from SpiMaster.
        // Most significant byte received first.
        // Start with the least significant byte (cfg[3]).
        // Load all of its bits onto Lis_Rst
        // do the same for the next two bytes
        // then only load the four least LSB of the most significant byte
    EnterLisProgrammingMode();
    // load bits
    // TODO: unit test this.
        // I only wrote this code to silence the -Wunused parameter warning:
        // Spectrometer configuration is four bytes.
    uint8_t const num_cfgbytes = 4;
    // Start at the least significant byte (the last byte in cfg).
    // Only do cfg[3] to cfg[1]. Byte cfg[0] is a special case.
    uint8_t byte_index = num_cfgbytes;
    while (--byte_index != 0)
    {
        uint8_t const nbits = 8;
        OutputCfgByte(cfg[byte_index], nbits);
        /* printf("\nByte cfg[%d] uses nbits=%d", byte_index, nbits); */
    }
    // Most significant byte (the first byte in cfg) only has four bits to output.
    uint8_t const nbits = 4;
    OutputCfgByte(cfg[0], nbits);
    /* printf("\nByte cfg[%d] uses nbits=%d\n", byte_index, nbits); */
    ExitLisProgrammingMode();
}

inline void EnterLisProgrammingMode(void)
{
    /* Do all setup of Lis_Rst while Lis_Clk is low */
    LisWaitForClkFallEdge();
    /* Assert Pix_Select to program the LIS */
    SetBit(Lis_port2, Lis_PixSelect);
}
inline void ExitLisProgrammingMode(void)
{
    /* Reset is low during normal (not programming mode) operation. */
    ClearBit(Lis_port1, Lis_Rst);
    /* De-assert Pix_Select to stop programming the LIS. */
    ClearBit(Lis_port2, Lis_PixSelect);
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

