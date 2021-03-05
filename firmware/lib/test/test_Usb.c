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

// ---Run_Usb_Private_Ft_tests---

/* =====[ _FtClockDatabus ]===== */
void FtClockDatabus_drives_FtClock_HIGH_if_direction_is_FtDrive(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(FtCtrl_port, FtClock, LOW);
    /* =====[ Operate ]===== */
    _FtClockDatabus(FtDrive);
    /* =====[ Test ]===== */
    test_bit_val_msg(FtCtrl_port, FtClock, HIGH, "FtClock");
}
void FtClockDatabus_drives_FtClock_LOW_if_direction_is_FtSample(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(FtCtrl_port, FtClock, HIGH);
    /* =====[ Operate ]===== */
    _FtClockDatabus(FtSample);
    /* =====[ Test ]===== */
    test_bit_val_msg(FtCtrl_port, FtClock, LOW, "FtClock");
}

/* =====[ _FtReadDatabus ]===== */
void FtReadDatabus_copies_databus_pin_values_to_address_pbyte(void)
{
    /* =====[ Setup ]===== */
    *FtData_pin = 0xab;
    /* =====[ Operate ]===== */
    uint8_t byte = 0; uint8_t * pbyte = &byte;
    _FtReadDatabus(pbyte);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(*FtData_pin, byte);
}

/* =====[ _FtWriteDatabus ]===== */
void FtWriteDatabus_outputs_byte_on_databus_pins(void)
{
    /* =====[ Setup ]===== */
    *FtData_port = 0x00;
    /* =====[ Operate ]===== */
    uint8_t byte = 0xab;
    _FtWriteDatabus(byte);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(byte, *FtData_port);
}

/* =====[ _FtDatabusPinDirection ]===== */
void FtDatabusPinDirection_makes_databus_pins_outputs_if_direction_is_FtOut(void)
{
    /* =====[ Setup ]===== */
    *FtData_ddr = 0x00; // start with pins as inputs
    /* =====[ Operate ]===== */
    _FtDatabusPinDirection(FtOut);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(0xFF, *FtData_ddr);
}
void FtDatabusPinDirection_makes_databus_pins_inputs_if_direction_is_FtIn(void)
{
    /* =====[ Setup ]===== */
    *FtData_ddr = 0xFF; // start with pins as outputs
    /* =====[ Operate ]===== */
    _FtDatabusPinDirection(FtIn);
    /* =====[ Test ]===== */
    TEST_ASSERT_EQUAL_HEX8(0x00, *FtData_ddr);
}

// ---Run_Usb_Ft_tests---

/* =====[ FtSelectFT221X ]===== */
void FtSelectFT221X_drives_FtChipSelect_LOW(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(FtCtrl_port, FtChipSelect, HIGH);
    /* =====[ Operate ]===== */
    FtSelectFT221X();
    /* =====[ Test ]===== */
    test_bit_val_msg(FtCtrl_port, FtChipSelect, LOW, "FtChipSelect");
}

/* =====[ FtUnselectFT221X ]===== */
void FtUnselectFT221X_drives_FtChipSelect_HIGH(void)
{
    /* =====[ Setup ]===== */
    setup_bit_val(FtCtrl_port, FtChipSelect, LOW);
    /* =====[ Operate ]===== */
    FtUnselectFT221X();
    /* =====[ Test ]===== */
    test_bit_val_msg(FtCtrl_port, FtChipSelect, HIGH, "FtChipSelect");
}

/* =====[ FtBusTurnaround ]===== */
void FtBusTurnaround_clocks_one_cycle_to_signal_data_drive_then_data_sample(void)
{
    puts("Drive clock to signal Data-Drive then Data-Sample:\n");
    puts("- FtDrive: rising edge");
    puts("- FtSample: falling edge");
    /* =====[ Operate ]===== */
    FtBusTurnaround();
    /* PrintAllCalls(mock); */
    /* =====[ Test ]===== */
    uint16_t call_n; uint8_t direction;
    //
    call_n = 1;
    SilentAssertCall(mock, call_n, "_FtClockDatabus");
    //
    direction = FtDrive;
    _AssertArg(call_n, 1, &direction);
    //
    call_n++;
    SilentAssertCall(mock, call_n, "_FtClockDatabus");
    //
    direction = FtSample;
    _AssertArg(call_n, 1, &direction);
}

/* =====[ FtIsOk ]===== */
void FtIsOk_returns_true_if_FtMiso_is_LOW(void)
{
    setup_bit_val(FtCtrl_pin, FtMiso, LOW);
    TEST_ASSERT_TRUE(FtIsOk());
}
void FtIsOk_returns_false_if_FtMiso_is_HIGH(void)
{
    setup_bit_val(FtCtrl_pin, FtMiso, HIGH);
    TEST_ASSERT_FALSE(FtIsOk());
}

/* =====[ FtRead ]===== */
void FtRead_clocks_one_byte_out_of_the_FT221X(void)
{
    /* =====[ Operate ]===== */
    uint8_t byte = 0; uint8_t *pbyte = &byte;
    FtRead(pbyte);
    /* PrintAllCalls(mock); */
    /* =====[ Test ]===== */
    uint16_t call_n; uint8_t direction;
    //
    call_n = 1;
    SilentAssertCall(mock, call_n, "_FtClockDatabus");
    //
    direction = FtDrive;
    _AssertArg(call_n, 1, &direction);
    //
    call_n = 2;
    SilentAssertCall(mock, call_n, "_FtClockDatabus");
    //
    direction = FtSample;
    _AssertArg(call_n, 1, &direction);
    //
}
void FtRead_stores_the_byte_at_address_pbyte(void)
{
    /* =====[ Operate ]===== */
    uint8_t byte = 0xcd; uint8_t *pbyte = &byte;
    FtRead(pbyte);
    /* =====[ Test ]===== */
    uint16_t call_n = 3;
    SilentAssertCall(mock, call_n, "_FtReadDatabus");
    //
    _AssertArg(call_n, 1, &pbyte);
    // See test of _FtReadDatabus to confirm byte==*FtData_pin.
    // Cannot test this here because _FtReadDatabus is faked.
}

/* =====[ FtWrite ]===== */
void FtWrite_signals_to_drive_data_onto_the_databus(void)
{
    /* =====[ Operate ]===== */
    FtWrite(0xab);
    /* =====[ Test ]===== */
    uint16_t call_n; uint8_t direction;
    //
    call_n = 1;
    SilentAssertCall(mock, call_n, "_FtClockDatabus");
    //
    direction = FtDrive;
    _AssertArg(call_n, 1, &direction);
}
void FtWrite_sets_microcontroller_databus_pins_as_outputs(void)
{
    /* =====[ Operate ]===== */
    FtWrite(0xab);
    /* =====[ Test ]===== */
    uint16_t call_n = 2;
    SilentAssertCall(mock, call_n, "_FtDatabusPinDirection");
    uint8_t pin_direction = FtOut;
    _AssertArg(call_n, 1, &pin_direction);
}
void FtWrite_outputs_byte_on_databus_pins(void)
{
    /* =====[ Operate ]===== */
    uint8_t byte = 0xab;
    FtWrite(byte);
    /* =====[ Test ]===== */
    uint16_t call_n = 3;
    SilentAssertCall(mock, call_n, "_FtWriteDatabus");
    _AssertArg(call_n, 1, &byte);
}
void FtWrite_signals_FT221X_to_sample_the_databus(void)
{
    /* =====[ Operate ]===== */
    FtWrite(0xab);
    /* =====[ Test ]===== */
    uint16_t call_n; uint8_t direction;
    //
    call_n = 4;
    SilentAssertCall(mock, call_n, "_FtClockDatabus");
    //
    direction = FtSample;
    _AssertArg(call_n, 1, &direction);
}
void FtWrite_sets_microcontroller_databus_pins_as_inputs(void)
{
    /* =====[ Operate ]===== */
    FtWrite(0xab);
    /* =====[ Test ]===== */
    uint16_t call_n = 5;
    SilentAssertCall(mock, call_n, "_FtDatabusPinDirection");
    uint8_t pin_direction = FtIn;
    _AssertArg(call_n, 1, &pin_direction);
}

// ---Run_Usb_API_tests---

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

/* =====[ UsbTxbufferIsFull ]===== */
void UsbTxbufferIsFull_returns_true_if_pin_MIOSIO0_is_HIGH(void)
{
    /* =====[ Setup ]===== */
    SetBit(FtData_pin, FtMiosio0);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_TRUE(UsbTxbufferIsFull());
}
void UsbTxbufferIsFull_returns_false_if_pin_MIOSIO0_is_LOW(void)
{
    /* =====[ Setup ]===== */
    ClearBit(FtData_pin, FtMiosio0);
    /* =====[ Operate and Test ]===== */
    TEST_ASSERT_FALSE(UsbTxbufferIsFull());
}

/* =====[ UsbReadByte ]===== */
void UsbReadByte_selects_the_FT221X(void)
{
    /* =====[ Operate ]===== */
    uint8_t byte = 0; uint8_t * pbyte = &byte;
    UsbReadByte(pbyte);
    /* =====[ Test ]===== */
    uint16_t call_n = 0;
    _AssertCall(++call_n, "FtSelectFT221X");
}
void UsbReadByte_drives_databus_with_read_command(void)
{
    puts("\n*Read command is* `0xc6`");
    /* =====[ Operate ]===== */
    uint8_t byte = 0; uint8_t * pbyte = &byte;
    UsbReadByte(pbyte);
    /* =====[ Test ]===== */
    uint16_t call_n = 0;
    SilentAssertCall(mock, ++call_n, "FtSelectFT221X");
    SilentAssertCall(mock, ++call_n, "FtWrite");
    uint8_t read_cmd = FtReadCmd;
    _AssertArg(call_n, 1, &read_cmd);
}
void UsbReadByte_signals_FT221X_to_sample_the_databus(void)
{
    /* =====[ Operate ]===== */
    uint8_t byte = 0; uint8_t * pbyte = &byte;
    UsbReadByte(pbyte);
    /* =====[ Test ]===== */
    uint16_t call_n = 0;
    SilentAssertCall(mock, ++call_n, "FtSelectFT221X");
    SilentAssertCall(mock, ++call_n, "FtWrite");
    _AssertCall(++call_n, "FtBusTurnaround");
}
void UsbReadByte_reads_the_byte_if_data_transfer_status_is_OK(void)
{
    /* =====[ Operate ]===== */
    uint8_t byte = 0; uint8_t * pbyte = &byte;
    UsbReadByte(pbyte);
    /* =====[ Test ]===== */
    uint16_t call_n = 0;
    SilentAssertCall(mock, ++call_n, "FtSelectFT221X");
    SilentAssertCall(mock, ++call_n, "FtWrite");
    SilentAssertCall(mock, ++call_n, "FtBusTurnaround");
    _AssertCall(++call_n, "FtIsOk");
    TEST_FAIL_MESSAGE("Implement Test");
    // TODO(sustainablelab): Test read is called only if FtIsOk() returns OK
}
void UsbReadByte_unselects_the_FT221X(void)
{
    /* =====[ Operate ]===== */
    uint8_t byte = 0; uint8_t * pbyte = &byte;
    UsbReadByte(pbyte);
    /* =====[ Test ]===== */
    uint16_t call_n = 0;
    SilentAssertCall(mock, ++call_n, "FtSelectFT221X");
    SilentAssertCall(mock, ++call_n, "FtWrite");
    SilentAssertCall(mock, ++call_n, "FtBusTurnaround");
    SilentAssertCall(mock, ++call_n, "FtIsOk");
    _AssertCall(++call_n, "FtUnselectFT221X");
}
void UsbReadByte_returns_either_FtOK_if_pbyte_has_the_read_data_or_FtError_if_Usb_receive_buffer_was_empty(void)
{
    TEST_FAIL_MESSAGE("Implement Test");
}

/* =====[ UsbWriteByte ]===== */


