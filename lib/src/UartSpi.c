#include "UartSpi.h"
#include "ReadWriteBits.h"


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
static void (*AdcConvIdleLow)(void) = StartAdcReadout;
static void StartAdcConversion(void)
{
    SetBit(UartSpi_port, UartSpi_AdcConv);
}
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
static void SpiMasterEnable(void)
{
    EnableAtmega328UsartInSpiMasterMode();
    UseSpiDataModeCpol1CPha1();
    ClearBit(UartSpi_csrc, UartSpi_DataOrder);    // MSB is transmitted first
    /* ---Enabling Rx and Tx overrides normal port operation--- */
    // Enable the receiver. The UART Rx pin becomes the SPI Miso.
    SetBit(UartSpi_csrb, UartSpi_RxEnable);
    // Enable the transmitter. The UART Tx pin becomes the SPI Mosi.
    SetBit(UartSpi_csrb, UartSpi_TxEnable);
}
void UartSpiInit(void)
{
    RunSpiAt5Mhz(); // datasheet says to call this first
    SetSckAsOutput();
    AdcConvIdleLow(); SetAdcConvAsOutput();
    SpiMasterEnable();
    RunSpiAt5Mhz(); // datasheet says to call this again after enable
}
static bool TxBufferIsEmpty(void)
{
    // Check `data register empty` flag UDRE0
    return BitIsSet(UartSpi_csra, UartSpi_DataRegEmpty);
}
static bool UartSpiTransferIsDone(void)
{
    // Check `data receive done` flag RXC0
    return BitIsSet(UartSpi_csra, UartSpi_RxComplete);
}
static uint8_t ReadUartSpiDataRegister(void)
{
    return *UartSpi_data;
}
uint16_t UartSpiRead(void)
{
    StartAdcConversion(); // conversion takes 4.66us max; one loop iter = 3 cpu
    /* uint8_t tconv = 15; _delay_loop_1(tconv); // 3*1.0e-7s*15 = 4.5us */
    // Wait for conversion to finish
    uint8_t ticks = 15; Delay3CpuCyclesPerTick(ticks); // 3*1.0e-7s*15 = 4.5us
    StartAdcReadout();
    while (!TxBufferIsEmpty());
    uint8_t byte_to_send = 0x00; // arbitrary choice -- write zeroes
    *UartSpi_data = byte_to_send;  // load tx buffer and start SPI transmission
    *UartSpi_data = byte_to_send;  // this makes it a 16-bit transmission ?
    while (!UartSpiTransferIsDone()) ;
    uint16_t adc_reading;
    adc_reading = ReadUartSpiDataRegister();    // MSB
    adc_reading = adc_reading << 8;
    while (!UartSpiTransferIsDone()) ;
    adc_reading |= ReadUartSpiDataRegister();   // LSB
    return adc_reading;
}

