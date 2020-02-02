#include "unity.h" // unit test framework
#include "Mock.h" // function faking framework
#include "test_Usb.h"
#include "Usb.h"
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
static void _AssertArg(uint16_t call_n, uint8_t arg_n, void *pval)
{
    //! Assert value of arg number **arg_n** == (*pval)
    /** Pass **address of arg** to parameter **pval**.\n
      * **Pass arg same way, whether it is a value or a pointer.**
      *
      * Example: `CallWithTwoArgs(uint8_t *pbyte, uint8_t bit_num)`
      *
      * ```
      * uint8_t byte = 0x0a;
      * uint8_t *pbyte = &byte;
      * uint8_t bit_num = 7;
      * _AssertCall(1, "CallWithTwoArgs");
      * _AssertArg(1,1, &pbyte);
      * _AssertArg(1,2, &bit_num);
      * ```
      *
      * The first _AssertArg checks a pointer to a byte.
      * The second _AssertArg checks a byte.
      *
      * Both use the `&` prefix to pass the address.
      *
      * In the case of `&pbyte`, AssertArg() checks the pointer
      * (pbyte), but the pre-test-result prints the value
      * pointed to (byte).
      *
      * Example:
      *
      * ```
      * // Call was `CallWithTwoArgs(pbyte, bit_num);`
      * _AssertArg(1,2, &bit_num); // check arg2 equals 7
      * _AssertArg(1,1, &pbyte); // check arg1 equals address of byte
      * ```
      *
      * The AssertArg pre-test-result message prints the value
      * recorded arg1 points to (0x0a).\n 
      *
      * Note that the test fails if arg1 does not equal pbyte,
      * even if pbyte and arg1 happen to point to equal values.
      * */
    GString *msg = g_string_new(NULL);
    g_string_printf(msg, "Expect different value for call %d arg %d.", call_n, arg_n);
    // I cannot print the expected value without asking the
    // caller to include the type as a string. Better to keep the
    // arg list short. Call and arg number are good enough.
    TEST_ASSERT_TRUE_MESSAGE(
        AssertArg(mock, call_n, arg_n, pval),
        msg->str
        );
    g_string_free(msg, true);
}
/* =====[ setup_bit_val ]===== */
enum bit_val {LOW, HIGH}; typedef enum bit_val bit_val;
static void setup_bit_val( usb_reg reg, usb_pin bit, bit_val v )
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
    usb_reg reg,
    usb_pin bit,
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

/* =====[ UsbRxbufferIsEmpty ]===== */
void UsbRxbufferIsEmpty_returns_true_if_pin_FT1248_MISO_is_HIGH(void)
{
    /* =====[ Setup ]===== */
    SetBit(FtCtrl_pin, FtMiso);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(UsbRxbufferIsEmpty());
}
void UsbRxbufferIsEmpty_returns_false_if_pin_FT1248_MISO_is_LOW(void)
{
    /* =====[ Setup ]===== */
    ClearBit(FtCtrl_pin, FtMiso);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(UsbRxbufferIsEmpty());
}

/* =====[ UsbRxbufferIsFull ]===== */
void UsbRxbufferIsFull_returns_false_if_pin_MIOSIO0_is_HIGH(void)
{
    /* =====[ Setup ]===== */
    SetBit(FtData_pin, FtMiosio0);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(UsbRxbufferIsFull());
}
void UsbRxbufferIsFull_returns_true_if_pin_MIOSIO0_is_LOW(void)
{
    /* =====[ Setup ]===== */
    ClearBit(FtData_pin, FtMiosio0);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(UsbRxbufferIsFull());
}

/* =====[ _FtDriveDatabus ]===== */
void FtDriveDatabus_drives_FtClock_HIGH_to_drive_data_onto_the_bus(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(FtCtrl_port, FtClock, LOW);
    /* =====[ Operate ]===== */
    _FtDriveDatabus();
    /* =====[ Test ]===== */
    test_bit_val_msg(FtCtrl_port, FtClock, HIGH, "FtClock");
}

/* =====[ _FtWriteDatabus ]===== */
void FtWriteDatabus_makes_FtMiosio0_to_FtMiosio7_output_pins(void)
{
    /* =====[ Setup ]===== */
    *FtData_ddr = 0x00;
    /* =====[ Operate ]===== */
    uint8_t data = 0xab;
    _FtWriteDatabus(data);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(0xFF, *FtData_ddr);
}
void FtWriteDatabus_outputs_data_on_FtMiosio0_to_FtMiosio7(void)
{
    /* =====[ Setup ]===== */
    uint8_t data = 0xab;
    *FtData_port = 0x00;
    /* =====[ Operate ]===== */
    _FtWriteDatabus(data);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(data, *FtData_port);
}

/* =====[ _FtSampleDatabus ]===== */
void FtSampleDatabus_drives_FtClock_LOW_to_sample_data_from_the_bus(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(FtCtrl_port, FtClock, HIGH);
    /* =====[ Operate ]===== */
    _FtSampleDatabus();
    /* =====[ Test ]===== */
    test_bit_val_msg(FtCtrl_port, FtClock, LOW, "FtClock");
}

/* =====[ UsbRxbufferPop ]===== */
void UsbRxbufferPop_selects_the_FT221X(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(FtCtrl_port, FtChipSelect, HIGH);
    /* =====[ Operate ]===== */
    UsbRxbufferPop();
    /* =====[ Test ]===== */
    test_bit_val_msg(FtCtrl_port, FtChipSelect, LOW, "FtChipSelect");
}
void UsbRxbufferPop_drives_databus_with_read_command(void)
{
    /* =====[ Operate ]===== */
    UsbRxbufferPop();
    /* =====[ Test ]===== */
    uint16_t call_n = 1;
    _AssertCall(call_n++, "_FtDriveDatabus");
    _AssertCall(call_n, "_FtWriteDatabus");
    uint8_t read_cmd = FtReadCmd;
    _AssertArg(call_n, 1, &read_cmd);
}
void UsbRxbufferPop_signals_FT221X_to_sample_the_databus(void)
{
    /* =====[ Operate ]===== */
    UsbRxbufferPop();
    /* =====[ Test ]===== */
    _AssertCall(3, "_FtSampleDatabus");
}
