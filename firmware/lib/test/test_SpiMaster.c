#include <glib.h>  // GString, GList
#include "unity.h" // unit test framework
#include "Mock.h" // function faking framework
#include "test_SpiMaster.h"
#include "SpiMaster.h"
#include "ReadWriteBits.h"

/* =====[ Test Helpers ]===== */
static void _AssertCall(uint16_t num, char const * name)
{
    //! Assert call number **num** is named **name**.
    // Put num and name in the message displayed if test fails
    GString *message = g_string_new(NULL);
    g_string_append_printf(message, "`%s` is not call %d", name, num);
    // Perform the test
    TEST_ASSERT_TRUE_MESSAGE(
        AssertCall(mock, num, name),
        message->str
        );
    // Free memory used by GString
    g_string_free(message, true);
}

/* =====[ setup_bit_val ]===== */
enum bit_val {LOW, HIGH}; typedef enum bit_val bit_val;
static void setup_bit_val( spi_reg reg, spi_bit bit, bit_val v )
{
    //! Set up for test to check bit value. Use generic fail message.
    /** - make bit `LOW` if function is supposed to make it `HIGH`
      * - make bit `HIGH` if function is supposed to make it `LOW`
      * - TEST_ macro double-checks the setup is correct
      *   - silent if setup is correct:
      *     - test continues
      *     - nothing printed in test results
      *   - fails if setup is not correct
      *     - test stops
      *     - print why the setup failed
      * */
    GString *msg = g_string_new(NULL);
    g_string_printf(msg, "Bit must be %s when the test starts", v ? "HIGH" : "LOW");
    if (HIGH == v)
    {
        SetBit(reg, bit);
        TEST_ASSERT_BIT_HIGH_MESSAGE(bit, *reg, msg->str);
    }
    else if (LOW == v)
    {
        ClearBit(reg, bit);
        TEST_ASSERT_BIT_LOW_MESSAGE(bit, *reg, msg->str);
    }
    else
    {
        g_string_printf(msg, "Test setup with invalid bit value: %d? ", v);
        g_string_append_printf(msg, "Bit value must be LOW or HIGH.");
        TEST_FAIL_MESSAGE(msg->str);
    }
    g_string_free(msg, true);
}

/* =====[ test_bit_val_msg ]===== */
static void test_bit_val_msg(
    spi_reg reg,
    spi_bit bit,
    bit_val v,
    char * bit_name )
{
    //! Check bit value. Use custom fail message with `bit_name`.
    // Put bit_name in the message displayed if test fails
    GString *msg = g_string_new(NULL);
    // Fail test if v is not HIGH or LOW
    if ((HIGH != v) && (LOW != v))
    {
        // Invalid test fail message
        g_string_printf(
            msg, "Invalid bit value `%s` = %d", bit_name, v
            );
        g_string_append_printf(msg, "Bit value must be LOW or HIGH.");
        TEST_FAIL_MESSAGE(msg->str);
    }
    else
    {
        // Test fail message
        g_string_printf(msg, "Expect `%s` (bit %d) ", bit_name, bit);
        g_string_append_printf(
            msg, "%s, was %s",
            v ? "HIGH" : "LOW",
            v ? "LOW" : "HIGH"
            );
        if ((HIGH == v) && BitIsSet(reg, bit)) TEST_PASS();
        else if ((LOW == v) && BitIsClear(reg, bit)) TEST_PASS();
        else TEST_FAIL_MESSAGE(msg->str);
    }
    g_string_free(msg, true);
}

/* =====[ SpiMasterInit ]===== */
void SpiMasterInit_idles_SlaveSelect_high(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val( Spi_PortOutput, Spi_Ss, LOW );
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_PortOutput, Spi_Ss, HIGH, "Spi_Ss");
}
void SpiMasterInit_makes_SlaveSelect_an_output(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val( Spi_PortDirection, Spi_Ss, LOW );
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_PortDirection, Spi_Ss, HIGH, "Spi_Ss");
}
void SpiMasterInit_makes_Miso_an_input(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val( Spi_PortDirection, Spi_Miso, HIGH );
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_PortDirection, Spi_Miso, LOW, "Spi_Miso");
}
void SpiMasterInit_enables_pullup_on_Miso(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val( Spi_PortOutput, Spi_Miso, LOW );
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_PortOutput, Spi_Miso, HIGH, "Spi_Miso");
}
void SpiMasterInit_makes_DataReady_an_input(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val( Spi_PortDirection, Spi_DataReady, HIGH );
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_PortDirection, Spi_DataReady, LOW, "Spi_DataReady");
}
void SpiMasterInit_enables_pullup_on_DataReady(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(Spi_PortOutput, Spi_DataReady, LOW);
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_PortOutput, Spi_DataReady, HIGH, "Spi_DataReady");
}
void SpiMasterInit_makes_Mosi_an_output(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(Spi_PortDirection, Spi_Mosi, LOW);
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_PortDirection, Spi_Mosi, HIGH, "Spi_Mosi");
}
void SpiMasterInit_makes_Sck_an_output(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(Spi_PortDirection, Spi_Sck, LOW);
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_PortDirection, Spi_Sck, HIGH, "Spi_Sck");
}
void SpiMasterInit_makes_this_MCU_the_SPI_Master(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(Spi_SPCR, Spi_MasterSlaveSelect, LOW);
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_SPCR, Spi_MasterSlaveSelect, HIGH, "Spi_MasterSlaveSelect");
}
void SpiMasterInit_sets_SPI_Clock_to_10MHz_ext_osc_divided_by_8(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(Spi_SPCR, Spi_ClockBit0, LOW);
    setup_bit_val(Spi_SPCR, Spi_ClockBit1, HIGH);
    setup_bit_val(Spi_SPSR, Spi_DoubleClock, LOW);
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    TEST_ASSERT_TRUE(BitIsSet(Spi_SPCR, Spi_ClockBit0));
    TEST_ASSERT_TRUE(BitIsClear(Spi_SPCR, Spi_ClockBit1));
    TEST_ASSERT_TRUE(BitIsSet(Spi_SPSR, Spi_DoubleClock));
}
void SpiMasterInit_enables_the_SPI_hardware_module(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(Spi_SPCR, Spi_Enable, LOW);
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    test_bit_val_msg(Spi_SPCR, Spi_Enable, HIGH, "Spi_Enable");
}
void SpiMasterInit_clears_SPI_interrupt_flag(void)
{
    /* =====[ Operate ]===== */
    SpiMasterInit();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    _AssertCall(call_n, "ClearSpiInterruptFlag");
}

