#include <Mock.h>
#include "test_Spi_MockUps.h"
#include "test_Spi.h"
#include "Spi.h"                // lib under test
#include "fake/Spi-Hardware.h"  // fake hardware dependencies in Spi.h
#include <unity.h>              // unit testing framework
#include "ReadWriteBits.h"
#include "mock_Spi.h"
#include "AvrAsmMacros.h"       // fake AVR asm macro dependencies

/* =====[ List of SPI Master Tests ]===== */
    // [x] SpiMasterInit_pulls_Ss_high
    // [x] SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs
    // [x] SpiMasterInit_makes_this_mcu_the_SPI_master
    // [x] SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8
    // [x] SpiMasterInit_enables_the_SPI_hardware_module
    // [x] SpiMasterOpenSpi_selects_the_SPI_slave
    // [x] SpiMasterCloseSpi_unselects_the_SPI_slave
    // [x] SpiTransferIsDone_returns_true_when_the_transfer_is_done
    // [x] SpiTransferIsDone_returns_false_when_the_transfer_is_not_done
    // [x] SpiResponseIsReady_returns_true_when_slave_signals_data_is_ready
    // [x] SpiResponseIsReady_returns_false_when_slave_signals_data_not_ready
    // [x] SpiMasterWrite_byte_loads_SPI_tx_buffer_with_byte
        //  This also starts the transmission.
        //  No reason to unit test that the SPI hardware module starts the
        //  transmission.
        //  Just test that the transmission completes.
    // [x] SpiMasterWrite_byte_waits_for_transmission_to_complete
        //
        // I added a level of indirection to `SpiMasterWrite()`.
        // I used to check the flag directly, this check is refactored into
        // `!SpiTransferIsDone()`. The refactoring makes the code more readable,
        // and it enables testing.
        // I mock `SpiTransferIsDone()` to return a sequence of bit checks of
        // the SPI Interrupt Flag.
        //
    // [x] SpiMasterWaitForResponse_waits_until_slave_signals_ready
    // [x] SpiMasterRead_returns_the_SPI_data_register
    // [ ] SpiMasterRead_waits_for_transmission_to_complete
/* =====[ List of SPI Slave Tests ]===== */
    // [x] SpiSlaveInit_configures_pin_Miso_as_an_output
    // [x] SpiSlaveInit_pulls_Miso_high
    // [x] SpiSlaveInit_enables_the_SPI_hardware_module
    // [x] SpiEnableInterrupt_enables_the_transfer_is_done_interrupt
    // [x] SpiSlaveRead_waits_until_transfer_is_done
    // [x] SpiSlaveRead_returns_the_SPI_data_register_byte
    // [x] SpiSlaveSignalDataIsReady_pulls_Miso_low
    // [x] SpiSlaveSignalDataIsNotReady_pulls_Miso_high

//
/* =====[ SPI Slave ]===== */
//
void SpiSlaveInit_configures_pin_Miso_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(Spi_Miso,  *Spi_ddr, "Failed for pin Miso.");
}
void SpiSlaveInit_pulls_Miso_high(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_Miso, *Spi_port);
}
void SpiSlaveInit_enables_the_SPI_hardware_module(void)
{
    /* =====[ Setup ]===== */
    *Spi_spcr = 0x00;
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Enable,
        *Spi_spcr,
        "Bit must be high to enable the SPI."
        );
}
void SpiEnableInterrupt_enables_the_transfer_is_done_interrupt(void)
{
    /* =====[ Setup ]===== */
    *Spi_spcr = 0x00;
    /* =====[ Operate ]===== */
    SpiEnableInterrupt();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_InterruptEnable, *Spi_spcr);
}
void SpiSlaveSignalDataIsReady_pulls_Miso_low(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0xFF;
    /* =====[ Operate ]===== */
    SpiSlaveSignalDataIsReady();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Spi_Miso, *Spi_port);
}
void SpiSlaveSignalDataIsNotReady_pulls_Miso_high(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    /* =====[ Operate ]===== */
    SpiSlaveSignalDataIsNotReady();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_Miso, *Spi_port);
}
/* =====[ SpiSlaveRead ]===== */
void SetUp_SpiSlaveRead(void)
{
    SetUpMock_SpiSlaveRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_SpiSlaveRead(void)
{
    TearDownMock_SpiSlaveRead();    // destroy the mock object
    // other teardown code
}
void SpiSlaveRead_waits_until_transfer_is_done(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    // SPI hardware module sets SPIF to indicate the SPI transfer is done.
    // SPIF := SPI Interrupt Flag
    // SpiSlaveRead checks SPIF to know when the transfer is done.
    bool SPIF_sequence[] = {false, false, true}; // true:= flag is set
    int num_times_SPIF_is_checked = sizeof(SPIF_sequence);
    SpiTransferIsDone_StubbedReturnValue = SPIF_sequence;
    /* =====[ Operate ]===== */
    SpiSlaveRead();
    /* =====[ Set up expected calls ]===== */
    for (int i=0; i<num_times_SPIF_is_checked; i++)
    {
        Expect_SpiTransferIsDone();
    }
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void SpiSlaveRead_returns_the_SPI_data_register_byte(void)
{
    /* =====[ Setup ]===== */
    uint8_t expect_byte = 0x09;
    *Spi_spdr = expect_byte;
    //=====[ Mock-up test scenario by defining return values ]=====
    // SPI hardware module sets SPIF to indicate the SPI transfer is done.
    // SPIF := SPI Interrupt Flag
    // SpiSlaveRead checks SPIF to know when the transfer is done.
    bool SPIF_sequence[] = {false, false, true}; // true:= flag is set
    SpiTransferIsDone_StubbedReturnValue = SPIF_sequence;
    /* =====[ Operate ]===== */
    uint8_t actual_byte = SpiSlaveRead();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(expect_byte, actual_byte);
}

//
/* =====[ SPI Master ]===== */
//
void SpiMasterInit_pulls_Ss_high(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_Ss, *Spi_port);
}
void SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(Spi_Ss,    *Spi_ddr, "Failed for pin Ss.");
    TEST_ASSERT_BIT_HIGH_MESSAGE(Spi_Mosi,  *Spi_ddr, "Failed for pin Mosi.");
    TEST_ASSERT_BIT_HIGH_MESSAGE(Spi_Sck,   *Spi_ddr, "Failed for pin Sck.");
}
void SpiMasterInit_makes_this_mcu_the_SPI_master(void)
{
    /* =====[ Setup ]===== */
    *Spi_spcr = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_MasterSlaveSelect,
        *Spi_spcr,
        "Expect bit is high if this is the master."
        );
}
void SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8(void)
{
    /* =====[ Setup ]===== */
    ClearBit  (Spi_spcr, Spi_ClockRateBit0);
    SetBit(Spi_spcr, Spi_ClockRateBit1);
    ClearBit(Spi_spsr, Spi_DoubleClockRate);
    //
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_ClockRateBit0,
        *Spi_spcr,
        "Failed for bit: ClockRateBit0."
        );
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_ClockRateBit1,
        *Spi_spcr,
        "Failed for bit: ClockRateBit1."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_DoubleClockRate,
        *Spi_spsr,
        "Failed for bit: DoubleClockRate."
        );
}
void SpiMasterInit_enables_the_SPI_hardware_module(void)
{
    /* =====[ Setup ]===== */
    *Spi_spcr = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(
            Spi_Enable,
            *Spi_spcr,
            "Bit must be high to enable the SPI."
            );
}
void SpiMasterOpenSpi_selects_the_SPI_slave(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0xFF;
    /* =====[ Operate ]===== */
    SpiMasterOpenSpi();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_LOW(Spi_Ss, *Spi_port);
}
void SpiMasterCloseSpi_unselects_the_SPI_slave(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterCloseSpi();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH(Spi_Ss, *Spi_port);
}
void SpiTransferIsDone_returns_true_when_the_transfer_is_done(void)
{
    /* =====[ Setup ]===== */
    SetBit(Spi_spsr, Spi_InterruptFlag);
    /* =====[ Test and Operate ]===== */
    TEST_ASSERT_TRUE(SpiTransferIsDone());
}
void SpiTransferIsDone_returns_false_when_the_transfer_is_not_done(void)
{
    /* =====[ Setup ]===== */
    ClearBit(Spi_spsr, Spi_InterruptFlag);
    /* =====[ Test and Operate ]===== */
    TEST_ASSERT_FALSE(SpiTransferIsDone());
}
void SpiResponseIsReady_returns_true_when_slave_signals_data_is_ready(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveSignalDataIsReady();
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(SpiResponseIsReady());
}
void SpiResponseIsReady_returns_false_when_slave_signals_data_not_ready(void)
{
    /* =====[ Setup ]===== */
    SpiSlaveSignalDataIsNotReady();
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(SpiResponseIsReady());
}
/* =====[ SpiMasterWrite ]===== */
void SetUp_SpiMasterWrite(void)
{
    SetUpMock_SpiMasterWrite();    // create the mock object to record calls
    // other setup code
}
void TearDown_SpiMasterWrite(void)
{
    TearDownMock_SpiMasterWrite();    // destroy the mock object
    // other teardown code
}
void SpiMasterWrite_byte_loads_SPI_tx_buffer_with_byte(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    // SPI hardware module sets SPIF to indicate the SPI transfer is done.
    // SPIF := SPI Interrupt Flag
    // SpiMasterWrite checks SPIF to know when the transfer is done.
    bool SPIF_sequence[] = {false, false, true}; // true:= flag is set
    SpiTransferIsDone_StubbedReturnValue = SPIF_sequence;
    /* =====[ Setup ]===== */
    *Spi_spdr = 0x00;  // start with 0x00 in the tx buffer
    uint8_t expect_byte = 0x09;
    /* =====[ Operate ]===== */
    uint8_t byte_to_send = expect_byte;
    SpiMasterWrite(byte_to_send);
    /* =====[ Test ]===== */
    uint8_t actual_byte_loaded = *Spi_spdr;
    TEST_ASSERT_EQUAL_UINT8(expect_byte, actual_byte_loaded);
}
void SpiMasterWrite_byte_waits_for_transmission_to_complete(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    // SPI hardware module sets SPIF to indicate the SPI transfer is done.
    // SPIF := SPI Interrupt Flag
    // SpiMasterWrite checks SPIF to know when the transfer is done.
    bool SPIF_sequence[] = {false, false, true}; // true:= flag is set
    int num_times_SPIF_is_checked = sizeof(SPIF_sequence);
    SpiTransferIsDone_StubbedReturnValue = SPIF_sequence;
    /* =====[ Operate ]===== */
    uint8_t byte_to_send = 0xAB;
    SpiMasterWrite(byte_to_send);
    /* =====[ Set up expected calls ]===== */
    Expect_SpiMasterOpenSpi();
    /* I am not mocking the code to load the SPI data register! */
    for (int i=0; i<num_times_SPIF_is_checked; i++)
    {
        Expect_SpiTransferIsDone();
    }
    Expect_SpiMasterCloseSpi();
    //
    /* =====[ Test ]===== */
    //
    // Checks that the SPI master does not close SPI until the transmission
    // completes.
    // Check this by asserting the number of calls to `SpiTransferIsDone()`
    // matches the number of times the SPI Interrupt Flag is sampled until the
    // transfer is done.
    //
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
/* =====[ SpiMasterRead ]===== */
void SpiMasterRead_waits_for_transmission_to_complete(void)
{
}
void SpiMasterRead_returns_the_SPI_data_register(void)
{
    /* =====[ Setup ]===== */
    uint8_t expect_byte = 0x0A;
    *Spi_spdr = expect_byte;
    /* =====[ Operate ]===== */
    uint8_t actual_byte = SpiMasterRead();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(expect_byte, actual_byte);
}
/* =====[ SpiMasterWaitForResponse ]===== */
void SetUp_SpiMasterWaitForResponse(void)
{
    SetUpMock_SpiMasterWaitForResponse();    // create the mock object to record calls
    // other setup code
}
void TearDown_SpiMasterWaitForResponse(void)
{
    TearDownMock_SpiMasterWaitForResponse();    // destroy the mock object
    // other teardown code
}
void SpiMasterWaitForResponse_waits_until_slave_signals_ready(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    bool response_is_ready_sequence[] = {false, false, false, true};
    int num_times_signal_is_checked = sizeof(response_is_ready_sequence);
    SpiResponseIsReady_StubbedReturnValue = response_is_ready_sequence;
    /* =====[ Operate ]===== */
    SpiMasterWaitForResponse();
    /* =====[ Set expectations ]===== */
    for (int i=0; i<num_times_signal_is_checked; i++)
    {
        Expect_SpiResponseIsReady();
    }
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
