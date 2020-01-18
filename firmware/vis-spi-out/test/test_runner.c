// Vim: use ;mktgc to build and run unit tests with compiler=gcc
// ---Unit Test Framework---
#include <unity.h>
#include <Mock.h>
// Python-to-Firmware communication status codes
#include "StatusCodes.h"
// LIS-770i configuration
#include "LisConfigs.h"

/* ---Global Queue--- */
#include "Queue.h"          // VisCmd accesses the Spi Rx Buffer
// SpiFifo must be global in main application for linking against
// extern declarations in other translation units
volatile Queue_s * SpiFifo;

// ---Test Framework requires runner to define setup/teardown/mock pointers---
void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock; // record calls/args to mocked-out libs
// ---Define an empty setup/teardown for pointing to---
void NothingToSetUp(void){}
void NothingToTearDown(void){}
/* ---Define a setup/teardown for writing call history--- */
void SetUp_Mock(void) { mock = Mock_new(); }
void TearDown_Mock(void) { Mock_destroy(mock); mock = NULL; }

// ---Lists of tests---
#include "test_Example.h"
#include "test_VisCmd.h"

// ---Fake all hardware---
#include "HardwareFake.h"

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;
//
void Example_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(test_Can_build_test_suite);
        RUN_TEST(test_Can_call_function_defined_in_app_lib);
        RUN_TEST(test_Can_call_inline_function_defined_in_app_lib);
    }
}

/* =====[ VisCmd_tests ]===== */
void Run_ReplyCommandInvalid_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(ReplyCommandInvalid_transmits_one_byte_over_SPI);
        RUN_TEST(ReplyCommandInvalid_sends_byte_INVALID_CMD);
    }
}
void Run_SetSensorConfig_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(SetSensorConfig_receives_three_bytes_of_config_from_Bridge);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_binning_is_invalid);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_gain_is_invalid);
        RUN_TEST(SetSensorConfig_replies_ERROR_if_active_rows_is_invalid);
        RUN_TEST(SetSensorConfig_replies_OK_if_all_config_values_are_valid);
        RUN_TEST(SetSensorConfig_writes_config_to_LIS_770i_programmable_setup_register);
    }
}
void VisCmd_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        Run_ReplyCommandInvalid_tests(Nope);
        Run_SetSensorConfig_tests(Yep);
    }
}

int main(void)
{
    UNITY_BEGIN();
    Example_tests(Nope);
    VisCmd_tests(Yep);
    return UNITY_END();
}
