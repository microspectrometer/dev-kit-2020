#include <Mock.h>
#include "test_UartSpi_MockUps.h"
#include "test_UartSpi.h"
#include "UartSpi.h"                // lib under test
#include "fake/UartSpi-Hardware.h"  // fake hardware dependencies in UartSpi.h
#include <unity.h>                  // unit testing framework
#include "mock_UartSpi.h"

/* =====[ List of UartSpi Plumbing Tests ]===== */
// [x] UartSpiInit
    // [x] UartSpiInit_loads_0_into_baud_rate_register
        // this runs the SPI clock at 5MHz for a 10MHz CPU clock
    // [x] UartSpiInit_sets_Sck_as_an_output
    // [x] UartSpiInit_sets_AdcConv_to_idle_low
    // [x] UartSpiInit_sets_AdcConv_as_an_output
    // [x] UartSpiInit_enables_the_UART_in_Master_SPI_Mode
    // [x] UartSpiInit_uses_SPI_data_mode_CPOL_1_CPHA_1
    // [x] UartSpiInit_cfgs_SPI_to_transfer_MSB_first
    // [x] UartSpiInit_gives_SPI_control_over_Miso_and_Mosi_pin_behavior
// [x] UartSpiRead
    // [x] UartSpiTransferIsDone_returns_true_when_the_transfer_is_done
    // [x] UartSpiRead_initiates_adc_conversion_and_readout
    // [x] UartSpiRead_does_a_16bit_SPI_transfer_with_the_ADC
    // [x] UartSpiRead_writes_the_16bit_adc_reading_to_the_input_address
        // - mock ReadUartSpiDataRegister to return 0x12 then 0x34
        // - check you get the value 0x1234
        // It is a waste of time to smash bytes together only to have the caller
        // rip them back apart.
// [ ] ADC application examples
    // [ ] StoreAdcReadingsInFrame_fills_an_array_with_adc_readings

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
    // Sanity-check that the stub code is correct.
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
    bool TxBufferIsEmpty_returns[] = {true};
    UartSpiTxBufferIsEmpty_StubbedReturnValue = TxBufferIsEmpty_returns;
    //
    bool UartSpiTransferIsDone_returns[] = {true, true};
    UartSpiTransferIsDone_StubbedReturnValue = UartSpiTransferIsDone_returns;
    /* =====[ Operate ]===== */
    uint8_t adc_reading[2];
    UartSpiRead(adc_reading);
    /* =====[ Set up expected calls ]===== */
    /* ---The two calls I am testing for--- */
    Expect_UartSpiStartAdcConversion();
    Expect_UartSpiStartAdcReadout();
    /* ---Plumbing for this test--- */
    // Since the blocking calls are stubbed, I have to *expect* them too.
    Expect_UartSpiTxBufferIsEmpty(); // wait for TxBuffer empty, then
    Expect_UartSpiTransfer16bits();      // transmit 16 bits
    Expect_UartSpiTransferIsDone();  // wait for receive to complete
    Expect_UartSpiReadDataRegister();   // read 8-bit MSByte
    Expect_UartSpiTransferIsDone();  // wait for receive to complete
    Expect_UartSpiReadDataRegister();   // read 8-bit LSByte
    /* =====[ Test: compare call lists ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        /* ListAllCalls(mock)          // print all calls. */
        );
}
void UartSpiRead_does_a_16bit_SPI_transfer_with_the_ADC(void)
{
    /* =====[ Setup ]===== */
    // Mock functions that make the FUT block until they return true.
    //
    bool TxBufferIsEmpty_returns[] = {true};
    UartSpiTxBufferIsEmpty_StubbedReturnValue = TxBufferIsEmpty_returns;
    //
    bool UartSpiTransferIsDone_returns[] = {true, true};
    UartSpiTransferIsDone_StubbedReturnValue = UartSpiTransferIsDone_returns;
    /* =====[ Operate ]===== */
    uint8_t adc_reading[2];
    UartSpiRead(adc_reading);
    /* =====[ Set list of expected calls ]===== */
    /* ---Plumbing for this test--- */
    Expect_UartSpiStartAdcConversion();
    Expect_UartSpiStartAdcReadout();
    /* ---The call I am testing for--- */
    Expect_UartSpiTxBufferIsEmpty();// wait for TxBuffer empty, then
    /* ---More plumbing for this test--- */
    Expect_UartSpiTransfer16bits();     // transmit 16 bits
    Expect_UartSpiTransferIsDone();     // wait for 8-bit receive to complete
    Expect_UartSpiReadDataRegister();   // read 8-bit MSByte
    Expect_UartSpiTransferIsDone();     // wait for 8-bit receive to complete
    Expect_UartSpiReadDataRegister();   // read 8-bit LSByte
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void UartSpiRead_writes_the_16bit_adc_reading_to_the_input_address(void)
{
    /* =====[ Inject stubbed return values for data register reads ]===== */
    uint8_t expected_MSB = 0x12; uint8_t expected_LSB = 0x34;
    uint8_t ReadDataRegister_returns[] = {expected_MSB, expected_LSB};
    UartSpiReadDataRegister_StubbedReturnValue = ReadDataRegister_returns;
    /* =====[ Setup ]===== */
    uint8_t adc_reading[] = {0, 0};
    /* =====[ Operate ]===== */
    UartSpiRead(adc_reading);
    /* =====[ Test ]===== */
    uint8_t actual_MSB = adc_reading[0];
    uint8_t actual_LSB = adc_reading[1];
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        expected_MSB, actual_MSB, "Failed for MSB."
        );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(
        expected_LSB, actual_LSB, "Failed for LSB."
        );
}

//
/* =====[ ADC application examples ]===== */
//
#define num_bytes_in_frame 2*4 // holds four adc readings
uint8_t actual_frame[num_bytes_in_frame];
void StoreAdcReadingsInFrame(void)
{
    // Write ADC readings to the global frame array.
    uint8_t byte_count = 0;
    uint8_t *pframe = actual_frame;
    while (byte_count < num_bytes_in_frame)
    {
        UartSpiRead(pframe);
        pframe++; pframe++; byte_count++; byte_count++;
    }
}
void StoreAdcReadingsInFrame_fills_an_array_with_adc_readings(void)
{
    /* =====[ Inject stubbed return values for data register reads ]===== */
    uint8_t ReadDataRegister_returns[] = {0,1,2,3,4,5,6,7};
    UartSpiReadDataRegister_StubbedReturnValue = ReadDataRegister_returns;
    /* =====[ Setup ]===== */
    /* =====[ Operate ]===== */
    StoreAdcReadingsInFrame();
    /* =====[ Test ]===== */
    uint8_t *expected_frame = ReadDataRegister_returns;
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_frame, actual_frame, num_bytes_in_frame);
}
