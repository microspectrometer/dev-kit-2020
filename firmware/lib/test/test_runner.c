// Vim: use ;mktgc to build and run unit tests with compiler=gcc
// ---Unit Test Framework---
#include "unity.h"
#include "Mock.h"
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
#include "test_BiColorLed.h"
#include "test_ReadWriteBits.h"
#include "test_SpiSlave.h"
#include "test_Queue.h"

// ---Fake all hardware---
#include "HardwareFake.h"

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;
//
void BiColorLed_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(BiColorLedOn_sets_bit_in_ddr);
        RUN_TEST(BiColorLedGreen_clears_bit_in_port);
    }
}
void ReadWriteBits_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SetBit_sets_bit_in_register);
        RUN_TEST(ClearBit_clears_bit_in_register);
    }
}
void Run_SpiSlaveInit_tests(bool run_test)
{
    if (run_test)
    {
        setUp = SetUp_Mock; tearDown = TearDown_Mock;
        RUN_TEST(SpiSlaveInit_makes_DataReady_an_output_pin);
        RUN_TEST(SpiSlaveInit_idles_DataReady_high);
        RUN_TEST(SpiSlaveInit_makes_Miso_an_output_pin);
        RUN_TEST(SpiSlaveInit_enables_SPI);
        RUN_TEST(SpiSlaveInit_enables_SPI_interrupt);
    }
}
void SpiSlave_tests(bool run_test)
{
    if (run_test)
    {
        Run_SpiSlaveInit_tests(Yep);
    }
}
void Run_QueueInit_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueueInit_returns_a_pointer_to_a_Queue_struct);
        RUN_TEST(QueueInit_memory_for_Queue_struct_is_allocated_in_Queue_object_file);
        RUN_TEST(QueueInit_assigns_input_buffer_as_Queue_buffer);
        RUN_TEST(QueueInit_size_input_is_the_maximum_Queue_length);
        RUN_TEST(QueueInit_initializes_Queue_with_length_0);
    }
}
void Run_QueueLength_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueueLength_increments_after_a_push);
        RUN_TEST(QueueLength_does_not_increase_beyond_max_length);
        RUN_TEST(QueueLength_decrements_after_a_pop);
        RUN_TEST(QueueLength_does_not_decrease_below_zero);
    }
}
void Run_QueuePush_tests(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(QueuePush_writes_to_byte_to_Queue_buffer);
    }
}
void Queue_tests(bool run_test)
{
    if (run_test)
    {
        Run_QueueInit_tests(Nope);
        Run_QueueLength_tests(Nope);
        Run_QueuePush_tests(Yep);
    }
}

int main()
{
    UNITY_BEGIN();
    BiColorLed_tests(Nope);
    ReadWriteBits_tests(Nope);
    SpiSlave_tests(Nope);
    Queue_tests(Yep);
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    return UNITY_END();
}
