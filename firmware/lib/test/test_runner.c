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

// ---Fake all hardware---
#include "HardwareFake.h"

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;
//
void BiColorLed(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(BiColorLedOn_sets_bit_in_ddr);
        RUN_TEST(BiColorLedGreen_clears_bit_in_port);
    }
}
void ReadWriteBits(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(SetBit_sets_bit_in_register);
        RUN_TEST(ClearBit_clears_bit_in_register);
    }
}
void SpiSlave(bool run_test)
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

int main()
{
    UNITY_BEGIN();
    BiColorLed(Nope);
    ReadWriteBits(Nope);
    SpiSlave(Yep);
    setUp = NothingToSetUp; tearDown = NothingToTearDown;
    return UNITY_END();
}
