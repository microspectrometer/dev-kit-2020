#include "unity.h"          // unity macros: TEST_BLAH
#include "test_Example.h"
#include "Example.h"
void test_Can_build_test_suite(void)
{
    TEST_PASS();
}
void test_Can_call_function_defined_in_app_lib(void)
{
    example_function();
    TEST_PASS();
}
