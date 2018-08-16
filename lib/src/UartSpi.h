#ifndef _UARTSPI_H
#define _UARTSPI_H

#include <stdint.h>
#include <stdbool.h>

/* =====[ AVR asm macros ]===== */
extern void (*Delay3CpuCyclesPerTick)(uint8_t);

/* =====[ UartSpi API ]===== */
void UartSpiInit(void);
uint16_t UartSpiRead(void);

/* =====[ Hardware dependencies to be resolved in UartSpi-Hardware.h ]===== */
/* ---I/O Registers--- */
extern uint8_t volatile * const UartSpi_ddr;
extern uint8_t volatile * const UartSpi_port;
extern uint8_t volatile * const UartSpi_csra;
extern uint8_t volatile * const UartSpi_csrb;
extern uint8_t volatile * const UartSpi_csrc;
extern uint8_t volatile * const UartSpi_data;
extern uint16_t volatile * const UartSpi_br;
/* ---Pin Names--- */
extern uint8_t const UartSpi_Miso;      // UART RX
extern uint8_t const UartSpi_Mosi;      // not used because ADC is read-only
extern uint8_t const UartSpi_AdcConv;   // high to ADC convert, low to readout
extern uint8_t const UartSpi_Sck;       // UART XCK
/* =====[ UartSpi control and status register bits ]===== */
extern uint8_t const UartSpi_ModeSelect0;   // set for spi-master
extern uint8_t const UartSpi_ModeSelect1;   // set for spi-master
extern uint8_t const UartSpi_RxEnable;      // set to enable
extern uint8_t const UartSpi_TxEnable;      // set to enable
extern uint8_t const UartSpi_ClockPolarity; // set for CPOL=1
extern uint8_t const UartSpi_ClockPhase;    // set for CPHA=1
extern uint8_t const UartSpi_DataOrder;     // clear for MSB first
extern uint8_t const UartSpi_DataRegEmpty;  // set when tx is done
extern uint8_t const UartSpi_RxComplete;    // set when rx is done

/* =====[ Exposed to mock for testing ]===== */
extern bool (*UartSpiTxBufferIsEmpty)(void);
extern bool (*UartSpiTransferIsDone)(void);

#endif // _UARTSPI_H
