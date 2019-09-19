/* Vim: use ;mktgc to run unit tests with compiler=gcc */
#include <unity.h>          // used by all test client code
#include "test_Bridge.h"    // test lib `Bridge`
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Makefile_finds_lib_Bridge);
    return UNITY_END();
}
