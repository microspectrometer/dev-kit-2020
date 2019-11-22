/** \file */
#include <stdio.h>
#include "Lis.h"
#include "UartSpi.h"
#include "Pwm.h"
#include "ReadWriteBits.h"
extern uint8_t frame[npixels*2];
uint16_t NumPixelsInFrame(void);

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
    /* LisWaitForClkFallEdge(); // uses macro version */
    LisWaitForClockFallingEdge();
    // sbi	0x15, 2	; 21
    // sbis	0x15, 2	; 21
    // rjmp .-4         ;
    // sbi  0x15, 2 ; 21
    /* Assert Pix_Select to program the LIS */
    SetBit(Lis_port2, Lis_PixSelect);
    // sbi	0x05, 0	; 5
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
    /* *Lis_clktop = 255;           // slowest: about 39 kHz */
    *Lis_clktop = 200;           // 10MHz / 50kHz = 200 ticks
    /* *Lis_clktop = 100;           // fastest: 100 kHz */
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

// ---2019-11-12 correct use of inline for optimized assembly code---
inline void LisWaitForClockFallingEdge(void)
{
    WaitForPwmFallingEdge(); // Wait for Lis clock falling edge
}
inline void LisWaitForClockRisingEdge(void)
{
    // Wait for Lis clock rising edge
    WaitForPwmRisingEdge();
}
inline void LisStartExposure(void)
{
    // &PORTD (0x0B), PD6 (6)
    // sbi	0x0b, 6;
    SetBit(Lis_port1, Lis_Rst); // RST high
}
inline void LisStopExposure(void)
{
    // &PORTD (0x0B), PD6 (6)
    // cbi	0x0b, 6;
    ClearBit(Lis_port1, Lis_Rst); // RST low
}
inline void LisWaitForSyncRisingEdge(void)
{
    // &PORTD (0x0B), PD7 (7)
    // sbis	0x09, 7	; 9
    // rjmp	.-4      	;
    while(BitIsClear(Lis_pin1, Lis_Sync));
}
inline void LisWaitForSyncFallingEdge(void)
{
    // &PORTD (0x0B), PD7 (7)
    // sbic	0x09, 7	; 9
    // rjmp	.-4      	;
    while(BitIsSet(Lis_pin1, Lis_Sync));
}
// If I make this the implementation of a function pointer, can avr-gcc still do
// inline without overhead?
inline void LisWaitForReadoutToStart(void)
{
    // ---Watch Sync Pulse---
    // Lis_pin1: PIND: 0x09, Lis_Sync: DDR7: 7
    // sbis	0x09, 7	; 9
    // rjmp	.-4      	;
    // sbic	0x09, 7	; 9
    // rjmp	.-4      	;
    LisWaitForSyncRisingEdge();
    LisWaitForSyncFallingEdge();
    // ---Alternative Method without Sync: Watch for two Clock falling edges---
    /* LisWaitForClockRisingEdge(); // exposure officially stops on rising edge */
    /* // Watching two falling edges *should* be the same as watching SYNC pulse. */
    /* LisWaitForClockFallingEdge(); LisWaitForClockFallingEdge(); */
}
inline void LisReadout(void)
{
    // Prepare pixel counter and frame pointer
    uint8_t *pframe = frame; // point to start of frame
    /* pframe = frame; // point to start of frame */
    uint16_t pixel_count = 0; // track number of pixels read
    // Get total number of pixels in this frame
    uint16_t npixels_in_frame = NumPixelsInFrame();
    while (pixel_count++ < npixels_in_frame)
    {
        /* LisReadOnePixel(); */
        LisWaitForClockRisingEdge();    // sbis	0x15, 1	; 21
        StartAdcConversion();           // sbi	0x0b, 2	; 11
        WaitForConversion(14); // ldi	r22, 0x10	; 14 hard-coded delay of 14*0.3us=4.2us
        /* LisWaitForClockFallingEdge(); // 10us delay (half-clock period) */
        StartAdcReadout(); // cbi	0x0b, 2	; 11
        WaitForEmptyTxBuffer(); // lds r24, 0x00C0; sbrs r24, 5
        *UartSpi_data = 0x00; // sts	0x00C6, r1	;
        *UartSpi_data = 0x00; // sts	0x00C6, r1	;
        WaitForByteFromAdc(); // lds	r24, 0x00C0	; sbrs	r24, 7
        /* *UartSpi_data; *(pframe++) =  pixel_count >> 8; // ramp for system testing */ 
        *(pframe++) =  *UartSpi_data; // lds	r24, 0x00C6	;
        WaitForByteFromAdc(); // lds	r24, 0x00C0	; sbrs	r24, 7
        /* *UartSpi_data; *(pframe++) =  pixel_count & 0xFF; // ramp for system testing */
        *(pframe++) =  *UartSpi_data; // lds	r24, 0x00C6	;
    }
}

