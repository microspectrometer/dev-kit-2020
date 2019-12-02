// Vim: use ;mktgc to build and run unit tests with compiler=gcc
// ---Unit Test Framework---
#include <unity.h>
#include <Mock.h>
// ---Test Framework requires runner to define setup/teardown/mock pointers---
void (*setUp)(void); void (*tearDown)(void);
Mock_s *mock; // record calls/args to mocked-out libs
// ---Define an empty setup/teardown for pointing to---
void NothingToSetUp(void){}
void NothingToTearDown(void){}

// ---Libs under test---
#include "test_Example.h"

/* ---Turn test suites on and off--- */
bool Yep=true, Nope=false;
//
void ExampleTestSuite(bool run_test)
{
    if (run_test)
    {
        setUp = NothingToSetUp; tearDown = NothingToTearDown;
        RUN_TEST(test_Can_build_test_suite);
    }
}

int main(void)
{
    UNITY_BEGIN();
    ExampleTestSuite(Yep);
    return UNITY_END();
}
