#include <Mock.h>
#include "test_Spi_MockUps.h"
#include "test_Spi.h"
#include "Spi.h"                // lib under test
#include "fake/Spi-Hardware.h"  // fake hardware dependencies in Spi.h
#include <unity.h>              // unit testing framework
#include "ReadWriteBits.h"
#include "mock_Spi.h"
#include "AvrAsmMacros.h"       // fake AVR asm macro dependencies

/* =====[ List of SPI Device Plumbing Tests ]===== */
    // [x] ClearPendingSpiInterrupt_reads_SPSR_and_SPDR
/* =====[ List of SPI Master Tests ]===== */
// [x] SpiMasterInit_cfg
    // [x] SpiMasterInit_configures_pins_Ss_Mosi_Sck_as_outputs
        // - SpiMaster must drive Ss, Mosi, and Sck hard
        // - these pins cannot be tri-stated
        // - the slave responds to *all* SPI communications
        // - if these pins are tri-stated, noise trips the SPI slave hardware
        // into communication
    // [x] SpiMasterInit_pulls_Ss_high
    // [x] SpiMasterInit_makes_this_mcu_the_SPI_master
    // [x] SpiMasterInit_sets_the_clock_rate_to_fosc_divided_by_8
    // [x] SpiMasterInit_enables_the_SPI_hardware_module
    // [x] SpiMasterInit_clears_pending_SPI_interrupt
// [x] SpiMasterInit_protects_against_false_SpiResponseIsReady_signals
    // - [x] SetMisoAsPullupInput_configure_Miso_as_an_input
    // - [x] SetMisoAsPullupInput_enable_its_pullup
// SpiMasterWrite and SpiMasterRead
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
    // [x] SpiMasterRead_waits_for_transmission_to_complete
    // [x] SpiMasterRead_loads_SPI_tx_buffer_with_slave_ignore
/* =====[ List of SPI Slave Tests ]===== */
// [x] SpiSlaveInit_cfg
    // [x] SpiSlaveInit_configures_pin_Miso_as_an_output
    // [x] SpiSlaveInit_makes_Miso_a_pullup_input_when_no_transfer_is_in_progress
    // [x] SpiSlaveInit_enables_the_SPI_hardware_module
    // [x] SpiSlaveInit_clears_pending_SPI_interrupt
// SpiSlaveRead and SpiSlaveSignalDataIsReady or NotReady
    // [x] SpiEnableInterrupt_enables_the_transfer_is_done_interrupt
        // - not used if interrupts are not used
        // I decided not to use interrupts:
        // - master has no time to do other stuff, may as well just poll
        // - the slave is either waiting for the master or doing what the master
        // said, and if it is in the middle of doing what the master said, it
        // should not be interrupted -- unless I decide to implement
        // functionality for the master to tell the slave to abort its current
        // task
        //
    // [x] SpiSlaveRead_waits_until_transfer_is_done
    // [x] SpiSlaveRead_returns_the_SPI_data_register_byte
    // [x] SpiSlaveSignalDataIsReady_pulls_Miso_low
// SpiSlaveProvideSensorData
    // [ ] SpiSlaveProvideSensorData_gets_the_data_to_send
// SpiSlaveSendBytes
    // [x] SpiSlaveSendBytes_loads_each_byte_into_its_tx_buffer
    // [ ] SpiSlaveSendBytes_signals_ready_after_loading_a_byte
    // [ ] SpiSlaveSendBytes_waits_for_master_read_after_signaling_ready

//
/* =====[ Plumbing for all SPI devices ]===== */
//
void SetUp_ClearPendingSpiInterrupt(void)
{
    SetUpMock_ClearPendingSpiInterrupt();    // create the mock object to record calls
    // other setup code
}
void TearDown_ClearPendingSpiInterrupt(void)
{
    TearDownMock_ClearPendingSpiInterrupt();    // destroy the mock object
    // other teardown code
}
/* =====[ ClearPendingSpiInterrupt ]===== */
void ClearPendingSpiInterrupt_reads_SPSR_and_SPDR(void)
{
    /* =====[ Operate ]===== */
    ClearPendingSpiInterrupt();
    Expect_ReadSpiStatusRegister();
    Expect_ReadSpiDataRegister();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}

//
/* =====[ SPI Slave ]===== */
//
void SetUp_SpiSlaveInit(void)
{
    SetUpMock_SpiSlaveInit();    // create the mock object to record calls
    // other setup code
}
void TearDown_SpiSlaveInit(void)
{
    TearDownMock_SpiSlaveInit();    // destroy the mock object
    // other teardown code
}
void SpiSlaveInit_configures_pin_Miso_as_an_output(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_BIT_HIGH_MESSAGE(Spi_Miso,  *Spi_ddr, "Failed for pin Miso.");
}
void SpiSlaveInit_makes_Miso_a_pullup_input_when_no_transfer_is_in_progress(void)
{
    // This test does not actually test any production code.
    // But it captures *my* understanding of the behavior of the SPI hardware
    // module.
    // This understanding was hard earned after many hours of debug.
    // I felt it worth documenting in code.
    //
    /* =====[ Setup ]===== */
    *Spi_ddr = 0x00;
    *Spi_port = 0x00;
    // Simulate the PIN register sampling the pin value.
    *Spi_pin = *Spi_port;
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Test ]===== */
    // Simulate the alternate SPI function overriding the PORT value.
    if( BitIsSet(Spi_spcr, Spi_Enable) )
    {
        SetBit(Spi_port, Spi_Miso);
        // Simulate the PIN register sampling the pin value.
        *Spi_pin = *Spi_port;
    }
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Miso,
        *Spi_port,
        "Assert this test simulates the SPI hardware making MISO a pullup input."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Miso,
        *Spi_pin,
        "Assert this test simulates the PIN register sampling the MISO pin."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Miso,
        *Spi_ddr,
        "MISO must be configured as an output."
        );
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
void SpiSlaveInit_clears_pending_SPI_interrupt(void)
{
    /* =====[ Operate ]===== */
    SpiSlaveInit();
    /* =====[ Set exepctations ]===== */
    Expect_ClearPendingSpiInterrupt();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
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
void SetUp_SpiSlaveSignalDataIsReady(void)
{
    SetUpMock_SpiSlaveSignalDataIsReady();    // create the mock object to record calls
    // other setup code
}
void TearDown_SpiSlaveSignalDataIsReady(void)
{
    TearDownMock_SpiSlaveSignalDataIsReady();    // destroy the mock object
    // other teardown code
}
void SpiSlaveSignalDataIsReady_pulls_Miso_low(void)
{
    /* =====[ Setup ]===== */
    *Spi_port = 0xFF;
    /* =====[ Operate ]===== */
    SpiSlaveSignalDataIsReady();
    /* =====[ Test ]===== */
    // First just make sure the `Miso` PORT value is low.
    TEST_ASSERT_BIT_LOW(Spi_Miso, *Spi_port);
    /* =====[ Set expectations ]===== */
    // If `Miso` PORT value is low and SPI is disabled, the Miso pin is low.
    Expect_DisableSpi();
    // After SPI is enabled again, `Miso` goes back to being a pull-up.
    Expect_EnableSpi();
    // I don't know how to test for all that, but I can at least check that
    // `DisableSpi` and `EnableSpi` are called.
    // The expectation is that Miso dips low briefly.
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );

}
void SetUp_SpiSlaveSendBytes(void)
{
    SetUpMock_SpiSlaveSendBytes();
}
void TearDown_SpiSlaveSendBytes(void)
{
    TearDownMock_SpiSlaveSendBytes();
}
void SpiSlaveSendBytes_loads_each_byte_into_its_tx_buffer(void)
{
    Expect_WriteSpiDataRegister(uint8_t byte_to_write);
    Expect_SpiSlaveSignalDataIsReady();
    //=====[ Mock-up test scenario by defining return values ]=====
    // SPI hardware module sets SPIF to indicate the SPI transfer is done.
    // SPIF := SPI Interrupt Flag
    // SpiSlaveSendBytes checks SPIF to know when the transfer is done.
    // SpiSlaveSendBytes does not load the next byte until the transfer is done.
    // Return *true: transfer done* enough times to fake sending all the data.
    bool SPIF_sequence[] = {true, true, true, true, true}; // true:= flag is set
    int num_times_tx_is_done = sizeof(SPIF_sequence);
    SpiTransferIsDone_StubbedReturnValue = SPIF_sequence;

    /* =====[ Setup ]===== */
    *Spi_spdr = 0x00;
    uint8_t fake_data[] = {1, 2, 3, 4, 5};  // fakes getting sensor data frame
    uint16_t nbytes = sizeof(fake_data);    // fakes known frame size
    // Make sure the test scenario is correct.
    TEST_ASSERT_EQUAL_UINT16(num_times_tx_is_done, nbytes);
    /* =====[ Operate ]===== */
    SpiSlaveSendBytes(fake_data, nbytes);
    /* =====[ Test ]===== */
    // - just check that SPDR has the last byte of fake_data
    uint8_t last_byte_loaded = fake_data[nbytes-1];
    // - to check all bytes are loaded, I need to add a level of indirection:
        // I'm not sure this added indirection is worth it.
        // For now I'm just noting how I would do it:
        // - replace the direct write to Spi_spdr with a function call.
        // - call it WriteSpiDataRegister
        // - mock the function
        // - have its stub write each byte to an array
        // - the test creates this array
        // - the stub has a global array pointer the test uses to get the stub
        // to alter the array that it has created
        // - on each write, the stub increments the array pointer
    TEST_ASSERT_EQUAL_UINT8(last_byte_loaded, *Spi_spdr);
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
void SetUp_SpiMasterInit(void)
{
    SetUpMock_SpiMasterInit();    // create the mock object to record calls
    // other setup code
}
void TearDown_SpiMasterInit(void)
{
    TearDownMock_SpiMasterInit();    // destroy the mock object
    // other teardown code
}
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
void SpiMasterInit_protects_against_false_SpiResponseIsReady_signals(void)
{
    /* =====[ Setup ]===== */
    *Spi_ddr = 0xFF; *Spi_port = 0x00;
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test Miso is configured correctly ]===== */
    TEST_ASSERT_BIT_LOW_MESSAGE(
        Spi_Miso,
        *Spi_ddr,
        "DDR Miso bit must be low to be an input."
        );
    TEST_ASSERT_BIT_HIGH_MESSAGE(
        Spi_Miso,
        *Spi_port,
        "PORT Miso bit must be high to enable pullups."
        );
}
void SpiMasterInit_clears_pending_SPI_interrupt(void)
{
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Set expectations ]===== */
    Expect_ClearPendingSpiInterrupt();
    /* =====[ Test interrupts are cleared ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
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
    /* SetBit(Spi_port, Spi_Miso); // watch the test FAIL */
    SpiSlaveSignalDataIsReady(); // watch the test PASS
    *Spi_pin = *Spi_port; // Fake `PIN` register sampling the input
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(SpiResponseIsReady());
}
void SpiResponseIsReady_returns_false_when_slave_signals_data_not_ready(void)
{
    /* =====[ Setup ]===== */
    /* SpiSlaveSignalDataIsReady(); // watch the test FAIL */
    SetBit(Spi_port, Spi_Miso); // watch the test PASS
    *Spi_pin = *Spi_port; // Fake `PIN` register sampling the input
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
void SetUp_SpiMasterRead(void)
{
    SetUpMock_SpiMasterRead();    // create the mock object to record calls
    // other setup code
}
void TearDown_SpiMasterRead(void)
{
    TearDownMock_SpiMasterRead();    // destroy the mock object
    // other teardown code
}
void SpiMasterRead_waits_for_transmission_to_complete(void)
{
    //=====[ Mock-up test scenario by defining return values ]=====
    // SPI hardware module sets SPIF to indicate the SPI transfer is done.
    // SPIF := SPI Interrupt Flag
    // SpiMasterWrite checks SPIF to know when the transfer is done.
    bool SPIF_sequence[] = {false, false, true}; // true:= flag is set
    int num_times_SPIF_is_checked = sizeof(SPIF_sequence);
    SpiTransferIsDone_StubbedReturnValue = SPIF_sequence;
    /* =====[ Operate ]===== */
    SpiMasterRead();
    /* =====[ Set up expected calls ]===== */
    for (int i=0; i<num_times_SPIF_is_checked; i++)
    {
        Expect_SpiTransferIsDone();
    }
    Expect_ReadSpiDataRegister();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE_MESSAGE(
        RanAsHoped(mock),           // If this is false,
        WhyDidItFail(mock)          // print this message.
        );
}
void SpiMasterRead_returns_the_SPI_data_register(void)
{
    /* =====[ Setup ]===== */
    uint8_t expect_byte = 0x0A;
    ReadSpiDataRegister_StubbedReturnValue = expect_byte;
    /* =====[ Operate ]===== */
    uint8_t actual_byte = SpiMasterRead();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(expect_byte, actual_byte);
}
void SpiMasterRead_loads_SPI_tx_buffer_with_slave_ignore(void)
{
    /* =====[ Setup ]===== */
    *Spi_spdr = 0x00;  // start with 0x00 in the tx buffer
    /* =====[ Operate ]===== */
    SpiMasterRead();
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_UINT8(slave_ignore, *Spi_spdr);
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
