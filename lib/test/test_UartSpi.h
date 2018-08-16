#ifndef _TEST_UARTSPI_H
#define _TEST_UARTSPI_H

void UartSpiInit_loads_0_into_baud_rate_register(void);
void UartSpiInit_sets_Sck_as_an_output(void);
void UartSpiInit_sets_AdcConv_to_idle_low(void);
void UartSpiInit_sets_AdcConv_as_an_output(void);
void UartSpiInit_enables_the_UART_in_Master_SPI_Mode(void);
void UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1(void);
void UartSpiInit_cfgs_SPI_to_transfer_MSB_first(void);

#endif // _TEST_UARTSPI_H
