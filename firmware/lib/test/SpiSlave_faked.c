/** \file SpiSlave_faked.c
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
  * For any `test` with `mock` defined, view all calls made:
  * ```
  * PrintAllCalls(mock);
  * ```
  * */
#include "SpiSlave_faked.h"
#include "Mock.h" // record call history in "mock"

/* =====[ Mock SpiSlaveTxByte() ]===== */
static RecordedCall * Record_SpiSlaveTxByte(uint8_t arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "SpiSlaveTxByte";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void SpiSlaveTxByte_fake(uint8_t input_byte)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   * */
    RecordActualCall(mock, Record_SpiSlaveTxByte(input_byte));
}

/* =====[ Mock EnableSpiInterrupt() ]===== */
static RecordedCall * Record_EnableSpiInterrupt(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "EnableSpiInterrupt";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void EnableSpiInterrupt_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  */
    RecordActualCall(mock, Record_EnableSpiInterrupt());
}

