#ifndef _UARTSPI_H
#define _UARTSPI_H
#include <stdint.h>
#include "ReadWriteBits.h"
//---Hardware types: register addresses, pin numbers, bit numbers---
typedef uint8_t volatile * const uspi_ptr; // i/o reg address
typedef uint16_t volatile * const uspi_ptr16; // i/o reg address
typedef uint8_t const uspi_pin; // bit index into i/o reg for an i/o pin
typedef uint8_t const uspi_bit; // bit index into i/o reg
// Register address, pin number, and bit definitions depend on compiler:
    // "gcc" uses test/HardwareFake.h
    // "avr-gcc" uses src/UartSpi-Hardware.h
// ---Registers---
extern uspi_ptr UartSpi_ddr;
extern uspi_ptr UartSpi_port;
extern uspi_ptr UartSpi_UCSR0A;
extern uspi_ptr UartSpi_UCSR0B;
extern uspi_ptr UartSpi_UCSR0C;
extern uspi_ptr UartSpi_UDR0;
extern uspi_ptr16 UartSpi_UBRR0;
// ---Pins---
extern uspi_pin UartSpi_Miso; // UART RX
extern uspi_pin UartSpi_AdcConv; // high: convert, low: readout
extern uspi_pin UartSpi_Sck; // UART XCK
// ---Bits---
extern uspi_bit UartSpi_UMSEL00; // set: mode select 0: SPI Master
extern uspi_bit UartSpi_UMSEL01; // set: mode select 1: SPI Master
extern uspi_bit UartSpi_RXEN0; // set: enable Rx
extern uspi_bit UartSpi_TXEN0; // set: enable Tx
extern uspi_bit UartSpi_UCPOL0; // set: CPOL=1 (Clock Polarity)
extern uspi_bit UartSpi_UCPHA0; // set: CPHA=1 (Clock Phase)
extern uspi_bit UartSpi_UDORD0; // clear: Data Order is MSB first
extern uspi_bit UartSpi_UDRE0; // (DataRegEmtpy) is set on Tx done
extern uspi_bit UartSpi_RXC0; // (RxComplete) is set on Rx done

// ---Private---
inline void RunSpiAt5Mhz(void)
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
inline void SetSckAsOutput(void)
{
    //! Cfg XCK as an output. CPOL=1 makes the clock idle high.
    SetBit(UartSpi_ddr, UartSpi_Sck);
    // ---Expected Assembly---
    // sbi	0x0a, 4	; 10
}
inline void AdcConvIdleLow(void)
{
    ClearBit(UartSpi_port, UartSpi_AdcConv);
    // ---Expected Assembly---
    // cbi	0x0b, 2	; 11
}
inline void SetAdcConvAsOutput(void)
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
inline void EnableAtmega328UsartInSpiMasterMode(void)
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
inline void UseSpiDataModeCpol1CPha1(void)
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
inline void CfgSpiToTransferMsbFirst(void)
{
    /** Transfer MSB first */
    ClearBit(UartSpi_UCSR0C, UartSpi_UDORD0);
    // ---Expected Assembly---
    // reg UCSR0C (reg 0xC2), UDORD0 (bit 2)
    // - clear bit 2
    // andi	r24, 0xFB	; 251
    // st	Z, r24
}
inline void GiveSpiControlOverMisoAndMosiPins(void)
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
inline void SpiMasterCfg(void)
{
    EnableAtmega328UsartInSpiMasterMode();
    UseSpiDataModeCpol1CPha1();
    CfgSpiToTransferMsbFirst();
    GiveSpiControlOverMisoAndMosiPins();
}
// ---API---
/** \file UartSpi.h
 * # API
 * void UartSpiInit(void);\n 
 * void StartAdcConversion(void);\n 
 * */
inline void UartSpiInit(void)
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
    RunSpiAt5Mhz();
    SetSckAsOutput();
    AdcConvIdleLow();
    SetAdcConvAsOutput();
    SpiMasterCfg();
    RunSpiAt5Mhz();
}
inline void StartAdcConversion(void)
{
    SetBit(UartSpi_port, UartSpi_AdcConv);
    // ---Expected Assembly---
    // sbi	0x0b, 2	; 11
}
inline void StartAdcReadout(void)
{
    ClearBit(UartSpi_port, UartSpi_AdcConv);
    // write two dummy bytes to transfer 16 bits
    *UartSpi_UDR0 = 0x00; *UartSpi_UDR0 = 0x00;
    // ---Expected Assembly---
    // cbi	0x0b, 2	; 11
}

#endif // _UARTSPI_H
