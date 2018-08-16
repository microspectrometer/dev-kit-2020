#include <Mock.h>
#include "test_UartSpi_MockUps.h"
#include "test_UartSpi.h"
#include "UartSpi.h"                // lib under test
#include "fake/UartSpi-Hardware.h"  // fake hardware dependencies in UartSpi.h
#include <unity.h>                  // unit testing framework
#include "mock_UartSpi.h"

/* =====[ List of UartSpi Plumbing Tests ]===== */
// [ ] UartSpiInit
    // [x] UartSpiInit_loads_0_into_baud_rate_register
        // this runs the SPI clock at 5MHz for a 10MHz CPU clock
    // [x] UartSpiInit_sets_Sck_as_an_output
    // [x] UartSpiInit_sets_AdcConv_to_idle_low
    // [x] UartSpiInit_sets_AdcConv_as_an_output
    // [x] UartSpiInit_enables_the_UART_in_Master_SPI_Mode
    // [x] UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1
    // [x] UartSpiInit_cfgs_SPI_to_transfer_MSB_first
    // [x] UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior
// [ ] UartSpiRead
    // [x] UartSpiTransferIsDone_returns_true_when_the_transfer_is_done
    // [x] UartSpiRead_initiates_adc_conversion_and_readout
    // [ ] UartSpiRead_does_a_16bit_SPI_transfer_with_the_ADC
    // [ ] UartSpiRead_reads_the_ADC_MSB_first
        // a.k.a. big-endian
        // mock ReadUartSpiDataRegister to return 0x12 then 0x34
        // check you get the value 0x1234
        // This test might go away -- I don't want to return a 16-bit value.
        // It is a waste of time to smash bytes together only to have the caller
        // rip them back apart.
        // And this way I can write directly to the frame array.
    // [ ] UartSpiRead_writes_the_16bit_adc_reading_to_the_input_address
        // same stubbed return values but now testing that memory is written
        // mock ReadUartSpiDataRegister to return 0x12 then 0x34
        // check you get the value 0x1234

//
/* =====[ UartSpiInit ]===== */
//
void UartSpiInit_loads_0_into_baud_rate_register(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_br = 1;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8( 0, *UartSpi_br);
}
void UartSpiInit_sets_Sck_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_ddr = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_Sck, *UartSpi_ddr, "Failed for pin Sck."
        );
}
void UartSpiInit_sets_AdcConv_to_idle_low(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_port = 0xFF;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        UartSpi_AdcConv, *UartSpi_port, "Failed for pin AdcConv."
        );
}
void UartSpiInit_sets_AdcConv_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_ddr = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_AdcConv, *UartSpi_ddr, "Failed for pin AdcConv."
        );
}
void UartSpiInit_enables_the_UART_in_Master_SPI_Mode(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_csrc = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_ModeSelect0, *UartSpi_csrc, "Failed for bit ModeSelect0."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_ModeSelect1, *UartSpi_csrc, "Failed for bit ModeSelect1."
        );
}
void UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_csrc = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_ClockPolarity, *UartSpi_csrc, "Failed for bit ClockPolarity."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_ClockPhase, *UartSpi_csrc, "Failed for bit ClockPhase."
        );
}
void UartSpiInit_cfgs_SPI_to_transfer_MSB_first(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_csrc = 0xFF;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        UartSpi_DataOrder, *UartSpi_csrc, "Failed for bit DataOrder."
        );
}
void UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior(void)
{
    /* =====[ Setup ]===== */
    *UartSpi_csrb = 0x00;
    /* =====[ Operate ]===== */
    UartSpiInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_RxEnable, *UartSpi_csrb, "Failed for bit RxEnable."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        UartSpi_TxEnable, *UartSpi_csrb, "Failed for bit TxEnable."
        );
}

//
/* =====[ UartSpiRead ]===== */
//
void SetUp_UartSpiRead(void)
{
    SetUpMock_UartSpiRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_UartSpiRead(void)
{
    TearDownMock_UartSpiRead();    // destroy the mock object
    // other teardown code
}
void UartSpiTransferIsDone_returns_true_when_the_transfer_is_done(void)
{
    // This is weird. I am testing a stub.
    // I have to do this. The stub is behaving really strange.
    bool UartSpiTransferIsDone_returns[] = {false, false, true};
    UartSpiTransferIsDone_StubbedReturnValue = UartSpiTransferIsDone_returns;
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(UartSpiTransferIsDone());
    TEST_ASSERT_FALSE(UartSpiTransferIsDone());
    TEST_ASSERT_TRUE(UartSpiTransferIsDone());
    /* =====[ Set expected calls ]===== */
    Expect_UartSpiTransferIsDone();
    Expect_UartSpiTransferIsDone();
    Expect_UartSpiTransferIsDone();
    /* =====[ Confirm call list matches ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void UartSpiRead_initiates_adc_conversion_and_readout(void)
{
    /* =====[ Setup ]===== */
    // Mock functions that make the FUT block until they return true.
    //
    /* bool TxBufferIsEmpty_returns[] = {false, false, true}; */
    bool TxBufferIsEmpty_returns[] = {true};
    int num_times_TxBuffer_is_checked = sizeof(TxBufferIsEmpty_returns);
    UartSpiTxBufferIsEmpty_StubbedReturnValue = TxBufferIsEmpty_returns;
    //
    /* bool UartSpiTransferIsDone_returns[] = {false, false, true}; */
    /* bool UartSpiTransferIsDone_returns[] = {false, true}; */
    bool UartSpiTransferIsDone_returns[] = {false, false, true, false, false, true};
    UartSpiTransferIsDone_StubbedReturnValue = UartSpiTransferIsDone_returns;
    int num_times_RxDone_is_checked = sizeof(UartSpiTransferIsDone_returns);
    /* =====[ Operate ]===== */
    UartSpiRead();
    /* =====[ Set up expected calls ]===== */
    /* Expect_StartAdcConversion(); */
    /* Expect_StartAdcReadout(); */
    // Since the blocking calls are stubbed, I have to *expect* them too.
    // First wait for the TxBuffer to be empty before starting a SPI transfer.
    for (int i=0; i<num_times_TxBuffer_is_checked; i++)
    {
        Expect_UartSpiTxBufferIsEmpty();
    }
    // UartSpiRead loads 16 bits into the tx buffer and the SPI transfer starts.
    for (int i=0; i<num_times_RxDone_is_checked; i++)
    {
        Expect_UartSpiTransferIsDone();
    }
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        ListAllCalls(mock)          // print all calls.
        );

}
