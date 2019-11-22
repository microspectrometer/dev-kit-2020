#include "UartSpi.h"
#include "ReadWriteBits.h"
#include "AvrAsmMacros.h"

//
/* =====[ UartSpiInit ]===== */
//
static void RunSpiAt5Mhz(void)
{
    // Configure USART to clock ADC SCK at fosc/2 = 5MHz:
        // baudrate in bits per second = fosc/(2*(UBBR0+1))
        // for (baudrate = fosc/2) set UBBR0=0
        // Check:
        // UBRR0 (baud rate register)  = ( fosc/(2*baudrate) ) -1
        // yes, UBBR0 is 0 if fosc=10MHz and baudrate=5MHz
    *UartSpi_br = 0;
    // Write 0x00 to UBBR0 (0xC4)
    // Load address 0xC4 into indirect addressing register Y
        // ldi	r28, 0xC4	; 196
        // ldi	r29, 0x00	; 0
    // r1 typically has the value 0x00
    // avr-gcc assembly output starts with <__ctors_end> section
    // For example, first two lines in __ctors_end clear the Status Register:
        // eor	r1, r1 (exclusive or r1 with itself clears all bits in r1)
        // out	0x3f, r1	; 63
    // I guess r1 never takes on other values, because <UartSpiInit> just
    // assumes r1 is 0x00.
    // Write r1 to UBRR0
        // std	Y+1, r1	; 0x01
        // st	Y, r1
    //
    //
}
static void SetSckAsOutput(void)
{
    // Cfg XCK as an output. CPOL=1 makes the clock idle high.
    // &DDRD, PD4
    // sbi	0x0a, 4	; 10
    SetBit(UartSpi_ddr, UartSpi_Sck);
}
inline void StartAdcReadout(void)
{
    // &PORTD, PD2
    // cbi	0x0b, 2	; 11
    ClearBit(UartSpi_port, UartSpi_AdcConv);
}
void (*UartSpiStartAdcReadout)(void) = StartAdcReadout;
static void (*AdcConvIdleLow)(void) = StartAdcReadout;
void StartAdcConversion(void);
void (*UartSpiStartAdcConversion)(void) = StartAdcConversion;
static void SetAdcConvAsOutput(void)
{
    // cfg general purpose output for conversion-start and readout-start
    // sbi	0x0a, 2	; 10
    SetBit(UartSpi_ddr, UartSpi_AdcConv);
}
static void EnableAtmega328UsartInSpiMasterMode(void)
{
    // &UCSR0C (reg 0xC2), UMSEL00 (bit 6) and UMSEL01 (bit 7)
    // Set bits 6 and 7 in register 0xC2.
    // Load the register address into Z.
    // Register Z is the indirect address 16-bit register: r31 (msb) r30 (lsb)
        // ldi	r30, 0xC2	; 194
        // ldi	r31, 0x00	; 0
    // Indirect load Z into r24: load r24 with the 8-bit value at address 0x00C2
        // ld r24, Z
    // set bit 6: logical-OR UCSR0C with 0100 0000 and write to UCSR0C
        // ori	r24, 0x40	; 64
        // st	Z, r24
    // Indirect load Z into r24: load r24 with the 8-bit value at address 0x00C2
        // ld	r24, Z
    // set bit 7: logical-OR UCSR0C with 1000 0000 and write to UCSR0C
        // ori	r24, 0x80	; 128
        // st	Z, r24
    SetBit(UartSpi_csrc, UartSpi_ModeSelect0);
    SetBit(UartSpi_csrc, UartSpi_ModeSelect1);
}
static void UseSpiDataModeCpol1CPha1(void)
{
    // Use SPI data mode: CPOL=1, CPHA=1
        // clock idles high
        // load data on falling  clock edge
        // sample data on rising clock edge
    // reg UCSR0C (reg 0xC2), UCPOL0 (bit 0) and UCPHA0 (bit 1)
    // Z already has value 0x00C2
    // Load 8-bit value at address 0x00C2 into r24
        // ld	r24, Z
    // Set bit 1 and write back to UCSR0C
        // ori	r24, 0x01	; 1
        // st	Z, r24
    // Load 8-bit value at address 0x00C2 into r24
        // ld	r24, Z
    // Set bit 2 and write back to UCSR0C
        // ori	r24, 0x02	; 2
        // st	Z, r24
    SetBit(UartSpi_csrc, UartSpi_ClockPolarity);  // clock idles high
    SetBit(UartSpi_csrc, UartSpi_ClockPhase);     // load data then sample data
}
static void CfgSpiToTransferMsbFirst(void)
{
    // reg UCSR0C (reg 0xC2), UDORD0 (bit 2)
    // Z already has value 0x00C2
    // Load 8-bit value at address 0x00C2 into r24
        // ld	r24, Z
    // Clear bit 2 of UCSR0C: 0xFB is 1111 1011 and write to UCSR0C
        // andi	r24, 0xFB	; 251
        // st	Z, r24
    ClearBit(UartSpi_csrc, UartSpi_DataOrder);
}
static void GiveSpiControlOverMisoAndMosiPins(void)
{
    /* ---Enabling Rx and Tx overrides normal port operation--- */
    // Enable the receiver. The UART Rx pin becomes the SPI Miso.
    // reg UCSR0B (0xC1), RXEN0 (bit 4), TXEN0 (bit 3)
    // Load address 0xC1 into Z and load 8-bit value at Z into r24
        // ldi	r30, 0xC1	; 193
        // ldi	r31, 0x00	; 0
        // ld	r24, Z
    // Set bit 2 and write to UCSR0B
        // ori	r24, 0x10	; 16
        // st	Z, r24
    // Load 8-bit value at Z into r24
        // ld	r24, Z
    // Set bit 3 and write to UCSR0B
        // ori	r24, 0x10	; 16
        // ori	r24, 0x08	; 8
        // st	Z, r24
    SetBit(UartSpi_csrb, UartSpi_RxEnable);
    // Enable the transmitter. The UART Tx pin becomes the SPI Mosi.
    SetBit(UartSpi_csrb, UartSpi_TxEnable);
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
    RunSpiAt5Mhz(); // datasheet says to call this first
    SetSckAsOutput();
    AdcConvIdleLow(); SetAdcConvAsOutput();
    SpiMasterCfg();
    RunSpiAt5Mhz(); // datasheet says to call this again after enable
}

//
/* =====[ UartSpiRead ]===== */
//
inline void WaitForEmptyTxBuffer(void)
{
    while (!BitIsSet(UartSpi_csra, UartSpi_DataRegEmpty));
    /* lds	r24, 0x00C0	; 0x8000c0 <__TEXT_REGION_LENGTH__+0x7e00c0> */
    /* sbrs	r24, 5 */
    /* rjmp	.-8      	; 0x2d4 <GetFrame_Implementation+0x7c> */
}
static bool TxBufferIsEmpty(void)
{
    // Check `data register empty` flag UDRE0
    return BitIsSet(UartSpi_csra, UartSpi_DataRegEmpty);
}
inline void WaitForByteFromAdc(void)
{
    // Wait for a byte of ADC readout to finish
    while (!BitIsSet(UartSpi_csra, UartSpi_RxComplete));
}
bool (*UartSpiTxBufferIsEmpty)(void) = TxBufferIsEmpty;
static bool Received8bits(void)
{
    // Check `data receive done` flag RXC0
    return BitIsSet(UartSpi_csra, UartSpi_RxComplete);
}
bool (*UartSpiTransferIsDone)(void) = Received8bits;
static uint8_t ReadDataRegister(void)
{
    return *UartSpi_data;
}
uint8_t (*UartSpiReadDataRegister)(void) = ReadDataRegister;
static void Transfer16bits(void)
{
    uint8_t byte_to_send = 0x00; // arbitrary choice -- write zeroes
    *UartSpi_data = byte_to_send;  // load tx buffer and start SPI transmission
    *UartSpi_data = byte_to_send;  // this makes it a 16-bit transmission ?
}
void (*UartSpiTransfer16bits)(void) = Transfer16bits;
inline void WaitForConversion(uint8_t ticks)
{
    Delay3ClocksPerTick(ticks); // temporary inline replacement
    // less than 11 and the output is constant at mid-scale, flat on all pixels.
    // more than about 20 or so and pixels start to drop.
    // 16 is the calculated value.
    // Or shold I just wait for the next clock rising edge?
    //
    // Wait for at least t_CONV (4.66us max) for conversion to finish.
    /* Delay3CpuCyclesPerTick(11); // definition in AvrAsmMacro.c, Makefile picks real/fake version of AvrAsmMacros.c */
    // inline delay compiles to this:
    /* ldi	r25, 0x10	; 16 */
    /* mov	r24, r25 ; loop bck to here*/
    /* dec	r24 */
    /* brne	.-4      	; 0x2ce <GetFrame_Implementation+0x76> */
    // There is no overhead. Calculate exact time needed.
    // Wait for conversion. This takes 4.66us max.
    // one cpu cycle is 1.0e-7 seconds
    // three cpu cyclces (one loop iteration) is 3.0e-7 seconds
    // 4.66 us/ 0.3 us = 15.533333 ticks
    // Round up to 16 ticks
    // Then delay lasts for 4.8us
}
void WaitForConversionToFinish(void)
{
    // conversion takes 4.66us max; one loop iter = 3 cpu
    uint8_t fifteen_ticks = 15;
    Delay3CpuCyclesPerTick(fifteen_ticks);
    // one cpu cycle is 1.0e-7 seconds
    // three cpu cyclces is 3.0e-7 seconds
    // 15 ticks is 3.0e-7s*15 = 4.5 microseconds
    // Adding in the overhead of function calls and setup, this should be well
    // over the 4.66us max.
    // TODO: measure actual AdcConv high-time on an oscilloscope.
}
void UartSpiRead(uint8_t *two_bytes)
{
    UartSpiStartAdcConversion();
    WaitForConversionToFinish();
    UartSpiStartAdcReadout();
    while (!UartSpiTxBufferIsEmpty());
    UartSpiTransfer16bits();
    while (!UartSpiTransferIsDone()) ;
    *(two_bytes++)  = UartSpiReadDataRegister();   // MSB
    while (!UartSpiTransferIsDone()) ;
    *two_bytes      = UartSpiReadDataRegister();   // LSB
    // Original version returning a 16-bit word:
        // This sucked.
        // It is a waste of time to smash bytes together only to have the caller
        // rip them back apart.
    /* uint16_t adc_reading; */
    /* adc_reading = UartSpiReadDataRegister();    // MSB */
    /* adc_reading = adc_reading << 8; */
    /* adc_reading |= UartSpiReadDataRegister();   // LSB */
    /* return adc_reading; */
}

