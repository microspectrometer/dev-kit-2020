/** \file */
#include "UartSpi.h"
static void RunSpiAt5Mhz(void)
{
    /** Configure USART to clock ADC SCK at `fosc/2 = 5MHz`
     *  - `fosc = 10MHz` (10MHz external oscillator)
     *  - **baudrate** in bits per second `= fosc/(2*(UBBR0+1))`
     *  - set `UBBR0=0` for **baudrate** `= fosc/2`
     * */
    *UartSpi_UBRR0 = 0;
    // iom328p.h: #define UBRR0 _SFR_MEM16(0xC4)
    // ---Expected Assembly---
    // sts	0x00C5, r1	; 0x8000c5 <__TEXT_REGION_LENGTH__+0x7e00c5>
    // sts	0x00C4, r1	; 0x8000c4 <__TEXT_REGION_LENGTH__+0x7e00c4>
}
static void SetSckAsOutput(void)
{
    //! Cfg XCK as an output. CPOL=1 makes the clock idle high.
    SetBit(UartSpi_ddr, UartSpi_Sck);
    // ---Expected Assembly---
    // sbi	0x0a, 4	; 10
}
void UartSpiInit(void)
{
    RunSpiAt5Mhz();
    SetSckAsOutput();
}
