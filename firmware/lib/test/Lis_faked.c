/** \file Lis_faked.c
  * **Faked** calls record themselves in `mock`.\n 
  * Tests access `mock` to see what the **function under test**
  * does:\n 
  * - **calls made** by the function under test\n 
  * - **args passed** by those calls.\n 
  * `TestSuite.exe` seg faults if any test accesses `mock`
  * without defining `mock`.\n 
  * `Mock.h` declares `mock`:\n 
  * ```
  * extern Mock_s mock;
  * ```
  * `mock` is a global that is defined in `test_runner`:
  * ```
  * Mock_s *mock;
  * ```
  * The memory for `mock` is allocated and deallocated in the
  * setup/teardown for each test that calls the `faked` call.\n 
  * Create/destroy `mock` for an entire test suite by pointing
  * `setUp` and `tearDown` in the `test_runner`:\n 
  * ```
  * setUp = SetUp_Mock; tearDown = TearDown_Mock;
  * ```
  * For any `test` with `mock` defined:\n 
  * View all calls made (not an assertion, but helpful for debug)
  * ```
  * PrintAllCalls(mock);
  * ```
  * Assert a specific call is made
  * ```
  * uint16_t call_n = 1;
  * TEST_ASSERT_TRUE(
  *     AssertCall(mock, call_n, "_SomeCall")
  *     );
  * ```
  * */
#include "Lis_faked.h"
#include "Mock.h" // record call history in "mock"
#include "Lis.h"

/* =====[ Mock _ConfigAs28bits() ]===== */
static RecordedCall * Record__ConfigAs28bits(uint8_t * arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_ConfigAs28bits";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t **)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void _ConfigAs28bits_fake(uint8_t * config)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   * */
    RecordActualCall(mock, Record__ConfigAs28bits(config));
}

