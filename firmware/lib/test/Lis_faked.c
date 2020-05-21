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

/* =====[ Mock LisReadout ]===== */
static RecordedCall * Record_LisReadout(uint16_t arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "LisReadout";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint16_t);
    *((uint16_t *)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void LisReadout_fake(uint16_t num_pixels)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   * */
    RecordActualCall(mock, Record_LisReadout(num_pixels));
}

/* =====[ Mock LisExpose ]===== */
static RecordedCall * Record_LisExpose(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "LisExpose";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void LisExpose_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   * */
    RecordActualCall(mock, Record_LisExpose());
}

/* =====[ Mock LisWriteConfig ]===== */
static RecordedCall * Record_LisWriteConfig(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "LisWriteConfig";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void LisWriteConfig_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   * */
    RecordActualCall(mock, Record_LisWriteConfig());
}

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

/* =====[ Mock _WaitForLisClkLow() ]===== */
static RecordedCall * Record__WaitForLisClkLow(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_WaitForLisClkLow";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void _WaitForLisClkLow_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   * */
    RecordActualCall(mock, Record__WaitForLisClkLow());
}

/* =====[ Mock _WaitForLisClkHigh() ]===== */
static RecordedCall * Record__WaitForLisClkHigh(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_WaitForLisClkHigh";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void _WaitForLisClkHigh_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   * */
    RecordActualCall(mock, Record__WaitForLisClkHigh());
}

/* =====[ Mock _EnterLisProgrammingMode() ]===== */
static RecordedCall * Record__EnterLisProgrammingMode(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_EnterLisProgrammingMode";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void _EnterLisProgrammingMode_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   * */
    RecordActualCall(mock, Record__EnterLisProgrammingMode());
}

/* =====[ Mock _WriteLisConfigBit ]===== */
static RecordedCall * Record__WriteLisConfigBit(uint8_t * arg1, uint8_t arg2)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_WriteLisConfigBit";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    RecordedArg *record_of_arg2 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t **)record_of_arg1->pArg) = arg1;
    *((uint8_t *)record_of_arg2->pArg) = arg2;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    RecordArg(record_of_this_call, record_of_arg2);
    return record_of_this_call;
}
void _WriteLisConfigBit_fake(uint8_t const * config, uint8_t bit_index)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   *  - arg2
   * */
    RecordActualCall(mock,  Record__WriteLisConfigBit // call name
                            (
                                (uint8_t *)config,     // arg1
                                bit_index   // arg2
                            )
                    );
}

/* =====[ Mock _Write28bitLisConfig() ]===== */
static RecordedCall * Record__Write28bitLisConfig(uint8_t * arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_Write28bitLisConfig";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t **)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void _Write28bitLisConfig_fake(uint8_t * config)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   * */
    RecordActualCall(mock, Record__Write28bitLisConfig(config));
}

/* =====[ Mock _ExitLisProgrammingMode() ]===== */
static RecordedCall * Record__ExitLisProgrammingMode(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_ExitLisProgrammingMode";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void _ExitLisProgrammingMode_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   * */
    RecordActualCall(mock, Record__ExitLisProgrammingMode());
}
