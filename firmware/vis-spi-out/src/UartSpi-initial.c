/* =====[ UartSpi preliminary work ]===== */
//  =====[ I/O Register ]=====          =====[ Register's Purpose ]=====
uint8_t volatile * const UartSpi_ddr    =   &DDRD;  // data direction in/out
uint8_t volatile * const UartSpi_port   =   &PORTD;  // data direction in/out
uint8_t volatile * const UartSpi_data   =   &UDR0;  // data reg
uint8_t volatile * const UartSpi_csra   =   &UCSR0A;// ctrl and status reg C
uint8_t volatile * const UartSpi_csrb   =   &UCSR0B;// ctrl and status reg C
uint8_t volatile * const UartSpi_csrc   =   &UCSR0C;// ctrl and status reg C
uint16_t volatile * const UartSpi_br     =   &UBRR0; // baud rate register
/* =====[ UartSpi pin connections on `mBrd` ]===== */
uint8_t const UartSpi_Miso    =   PD0; // to adc spi-slave data out
uint8_t const UartSpi_AdcConv =   PD2; // to adc spi-slave conversion start
uint8_t const UartSpi_Mosi    =   PD1; // not connected! ADC is read-only
uint8_t const UartSpi_Sck     =   PD4; // to adc spi-slave clock in
/* =====[ UartSpi control and status register bits ]===== */
uint8_t const UartSpi_ModeSelect0   =   UMSEL00;  // set for spi-master
uint8_t const UartSpi_ModeSelect1   =   UMSEL01;  // set for spi-master
uint8_t const UartSpi_RxEnable      =   RXEN0;    // set to enable
uint8_t const UartSpi_TxEnable      =   TXEN0;    // set to enable
uint8_t const UartSpi_ClockPolarity =   UCPOL0;   // set for CPOL=1
uint8_t const UartSpi_ClockPhase    =   UCPHA0;   // set for CPHA=1
uint8_t const UartSpi_DataOrder     =   UDORD0;   // clear for MSB first
uint8_t const UartSpi_DataRegEmpty  =   UDRE0;    // set when tx is done
uint8_t const UartSpi_RxComplete    =   RXC0;     // set when rx is done
/* =====[ Hardware dependencies to be resolved in Spi-Hardware.h ]===== */
/* ---Pin Direction and I/O Registers--- */
extern uint8_t volatile * const UartSpi_ddr;
extern uint8_t volatile * const UartSpi_port;
extern uint8_t volatile * const UartSpi_data;
extern uint8_t volatile * const UartSpi_csra;
extern uint8_t volatile * const UartSpi_csrb;
extern uint8_t volatile * const UartSpi_csrc;
extern uint16_t volatile * const UartSpi_br;
/* ---Pin Names--- */
extern uint8_t const UartSpi_Miso;
extern uint8_t const UartSpi_AdcConv;  // more than a slave select
extern uint8_t const UartSpi_Mosi;
extern uint8_t const UartSpi_Sck;
/* =====[ UartSpi control and status register bits ]===== */
extern uint8_t const UartSpi_ModeSelect0;
extern uint8_t const UartSpi_ModeSelect1;
extern uint8_t const UartSpi_RxEnable;
extern uint8_t const UartSpi_TxEnable;
extern uint8_t const UartSpi_ClockPolarity;
extern uint8_t const UartSpi_ClockPhase;
extern uint8_t const UartSpi_DataOrder;

/* =====[ UartSpi API ]===== */
void UartSpiInit(void);
uint16_t UartSpiRead(void);

static void RunSpiAt5Mhz(void)
{
    // Configure USART to clock ADC SCK at fosc/2 = 5MHz:
        // UBRR0    = fosc/(2*baudrate) -1
        // baudrate = fosc/(2*(UBBR0+1)); // baud rate is bits per second
        // fosc=10MHz, so for UBBR0=0, baudrate = fosc/2 = 5MHz
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
static void EnableSpiMode(void)
{
    // Enable ATmega328 USART in SPI master mode.
    SetBit(UartSpi_csrc, UartSpi_ModeSelect0);
    SetBit(UartSpi_csrc, UartSpi_ModeSelect1);
    // Use SPI data mode: CPOL=1, CPHA=1
        // clock idles high
        // load data on falling  clock edge
        // sample data on rising clock edge
    SetBit(UartSpi_csrc, UartSpi_ClockPolarity);  // clock idles high
    SetBit(UartSpi_csrc, UartSpi_ClockPhase);     // load data then sample data
    ClearBit(UartSpi_csrc, UartSpi_DataOrder);    // MSB is transmitted first
    /* ---Enabling Rx and Tx overrides normal port operation--- */
    // Enable the receiver. The UART Rx pin becomes the SPI Miso.
    SetBit(UartSpi_csrb, UartSpi_RxEnable);
    // Enable the transmitter. The UART Tx pin becomes the SPI Mosi.
    SetBit(UartSpi_csrb, UartSpi_TxEnable);
}

void UartSpiInit(void)
{
    RunSpiAt5Mhz();
    SetSckAsOutput();
    AdcConvIdleLow(); SetAdcConvAsOutput();
    EnableSpiMode();
    RunSpiAt5Mhz(); // Do I need to call this again after the enable?
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
    uint8_t tconv = 15; _delay_loop_1(tconv); // 3*0.1e-7s*15 = 4.5us
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

