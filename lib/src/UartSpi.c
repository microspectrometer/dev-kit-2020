#include "UartSpi.h"
#include "ReadWriteBits.h"

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
}
static void SetSckAsOutput(void)
{
    // Cfg XCK as an output. CPOL=1 makes the clock idle high.
    SetBit(UartSpi_ddr, UartSpi_Sck);
}
static void StartAdcReadout(void)
{
    ClearBit(UartSpi_port, UartSpi_AdcConv);
}
void (*UartSpiStartAdcReadout)(void) = StartAdcReadout;
static void (*AdcConvIdleLow)(void) = StartAdcReadout;
static void StartAdcConversion(void)
{
    SetBit(UartSpi_port, UartSpi_AdcConv);
}
void (*UartSpiStartAdcConversion)(void) = StartAdcConversion;
static void SetAdcConvAsOutput(void)
{
    // cfg general purpose output for conversion-start and readout-start
    SetBit(UartSpi_ddr, UartSpi_AdcConv);
}
static void EnableAtmega328UsartInSpiMasterMode(void)
{
    SetBit(UartSpi_csrc, UartSpi_ModeSelect0);
    SetBit(UartSpi_csrc, UartSpi_ModeSelect1);
}
static void UseSpiDataModeCpol1CPha1(void)
{
    // Use SPI data mode: CPOL=1, CPHA=1
        // clock idles high
        // load data on falling  clock edge
        // sample data on rising clock edge
    SetBit(UartSpi_csrc, UartSpi_ClockPolarity);  // clock idles high
    SetBit(UartSpi_csrc, UartSpi_ClockPhase);     // load data then sample data
}
static void CfgSpiToTransferMsbFirst(void)
{
    ClearBit(UartSpi_csrc, UartSpi_DataOrder);
}
static void GiveSpiControlOverMisoAndMosiPins(void)
{
    /* ---Enabling Rx and Tx overrides normal port operation--- */
    // Enable the receiver. The UART Rx pin becomes the SPI Miso.
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
static bool TxBufferIsEmpty(void)
{
    // Check `data register empty` flag UDRE0
    return BitIsSet(UartSpi_csra, UartSpi_DataRegEmpty);
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

