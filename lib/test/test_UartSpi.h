#ifndef _TEST_UARTSPI_H
#define _TEST_UARTSPI_H

void UartSpiInit_loads_0_into_baud_rate_register(void);
void UartSpiInit_sets_Sck_as_an_output(void);
void UartSpiInit_sets_AdcConv_to_idle_low(void);
void UartSpiInit_sets_AdcConv_as_an_output(void);
void UartSpiInit_enables_the_UART_in_Master_SPI_Mode(void);
void UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1(void);
void UartSpiInit_cfgs_SPI_to_transfer_MSB_first(void);
void UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior(void);

void SetUp_UartSpiRead(void); void TearDown_UartSpiRead(void);
void UartSpiRead_initiates_adc_conversion_and_readout(void);
void UartSpiTransferIsDone_returns_true_when_the_transfer_is_done(void);
void UartSpiRead_does_a_16bit_SPI_transfer_with_the_ADC(void);

#endif // _TEST_UARTSPI_H
