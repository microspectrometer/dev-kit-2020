#include <unity.h>
#include "test_example.h"
void (*setUp)(void); void (*tearDown)(void);
void EmptySetUp(void){}
void EmptyTearDown(void){}


bool Yep=true, Nope=false; // Yep/Nope := tests on/off
void test_example(bool run_test)
{
    if (run_test)
    {
        setUp = EmptySetUp; tearDown = EmptyTearDown;
        RUN_TEST(test_Can_build_test_suite);
        RUN_TEST(test_Can_call_function_defined_in_app_lib);
        RUN_TEST(test_Can_call_inline_function_defined_in_app_lib);
    }
}
int main(void)
{
    UNITY_BEGIN();
    test_example(Yep);
    return UNITY_END();
}
