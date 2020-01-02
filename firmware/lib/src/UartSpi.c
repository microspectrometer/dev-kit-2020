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
static void AdcConvIdleLow(void)
{
    ClearBit(UartSpi_port, UartSpi_AdcConv);
    // ---Expected Assembly---
    // cbi	0x0b, 2	; 11
}
static void SetAdcConvAsOutput(void)
{
    /** AdcConv is a general purpose i/o pin.
     *  - configure as an output
     *  - controls start of conversion
     *  - controls start of readout
     * */
    SetBit(UartSpi_ddr, UartSpi_AdcConv);
    // ---Expected Assembly---
    // sbi	0x0a, 2	; 10
}
static void EnableAtmega328UsartInSpiMasterMode(void)
{
    SetBit(UartSpi_UCSR0C, UartSpi_UMSEL00);
    SetBit(UartSpi_UCSR0C, UartSpi_UMSEL01);
    // ---Expected Assembly---
    // &UCSR0C (reg 0xC2), UMSEL00 (bit 6) and UMSEL01 (bit 7)
    // Set bits 6 and 7 in register 16-bit register 0xC2.
    //
    // Register Z is the indirect address 16-bit register:
    // - r31 is the msb
    // - r30 is the lsb
    // ldi	r30, 0xC2	; 194
    // ldi	r31, 0x00	; 0
    // ld	r24, Z
    // - Set bit 6
    // ori	r24, 0x40	; 64
    // st	Z, r24
    // ld	r24, Z
    // - Set bit 7
    // ori	r24, 0x80	; 128
    // st	Z, r24
}
static void UseSpiDataModeCpol1CPha1(void)
{
    /** Use SPI data mode: CPOL=1, CPHA=1
     *  - clock idles high
     *  - load data on falling  clock edge
     *  - sample data on rising clock edge
     * */
    SetBit(UartSpi_UCSR0C, UartSpi_UCPOL0); // clock idles high
    SetBit(UartSpi_UCSR0C, UartSpi_UCPHA0); // load data then sample data
    // ---Expected Assembly---
    // reg: UCSR0C (reg 0xC2)
    // set bit 0: UCPOL0
    // set bit 1: UCPHA0
    // - set bit 1
    // ori	r24, 0x01	; 1
    // st	Z, r24
    // ld	r24, Z
    // - set bit 2
    // ori	r24, 0x02	; 2
    // st	Z, r24
}
static void CfgSpiToTransferMsbFirst(void)
{
    /** Transfer MSB first */
    ClearBit(UartSpi_UCSR0C, UartSpi_UDORD0);
    // ---Expected Assembly---
    // reg UCSR0C (reg 0xC2), UDORD0 (bit 2)
    // - clear bit 2
    // andi	r24, 0xFB	; 251
    // st	Z, r24
}
static void GiveSpiControlOverMisoAndMosiPins(void)
{
    /** Override normal port operation for SPI MOSI/MISO:
     * - enable the transmitter:
     *   - the UART Tx pin becomes the SPI MOSI
     * - enable the receiver:
     *   - the UART Rx pin becomes the SPI MISO
     * */
    SetBit(UartSpi_UCSR0B, UartSpi_RXEN0);
    SetBit(UartSpi_UCSR0B, UartSpi_TXEN0);
    // ---Expected Assembly---
    // reg UCSR0B (0xC1), RXEN0 (bit 4), TXEN0 (bit 3)
    //
    // ldi	r30, 0xC1	; 193
    // ldi	r31, 0x00	; 0
    // - set bit 4
    // ld	r24, Z
    // ori	r24, 0x10	; 16
    // st	Z, r24
    // - set bit 3
    // ld	r24, Z
    // ori	r24, 0x08	; 8
    // st	Z, r24
}
static void SpiMasterCfg(void)
{
    EnableAtmega328UsartInSpiMasterMode();
    UseSpiDataModeCpol1CPha1();
    CfgSpiToTransferMsbFirst();
    GiveSpiControlOverMisoAndMosiPins();
}

void UartSpiInit(void)
{
    /** UartSpiInit behavior:\n 
      * - clocks SPI bus at 5MHz\n 
      * - sets Sck as an output\n 
      * - sets AdcConv to idle low\n 
      * - sets AdcConv as an output\n 
      * - enables the UART in Master SPI Mode\n 
      * - uses SPI data mode CPOL 1 CPHA 1\n 
      * - cfgs SPI to transfer MSB first\n 
      * - gives SPI control over Miso and Mosi pin behavior\n 
      * */
    // ---Expected Assembly---
    // call	0x1d0	; 0x1d0 <UartSpiInit>
    RunSpiAt5Mhz();
    SetSckAsOutput();
    AdcConvIdleLow();
    SetAdcConvAsOutput();
    SpiMasterCfg();
    RunSpiAt5Mhz();
}
void StartAdcReadout(void);

