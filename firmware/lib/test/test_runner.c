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

// ---Libs under test---
#include "test_BiColorLed.h"
// ---Fake Hardware Definitions---
#include "HardwareFake.h"

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;
//
void BiColorLed(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(test_BiColorLedOn_sets_bit_in_ddr);
    }
}

int main()
{
    UNITY_BEGIN();
    BiColorLed(Yep);
    return UNITY_END();
}
