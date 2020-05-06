/** \file Spi_faked.c
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
  *     AssertCall(mock, call_n, "_SignalDataReady")
  *     );
  * ```
  * */
#include "Mock.h" // record call history in "mock"
#include "ReadWriteBits.h"
#include "Spi_faked.h"
#include "Spi.h"
/* =====[ Mock _SpiTransferIsDone() ]===== */
static RecordedCall * Record__SpiTransferIsDone(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_SpiTransferIsDone";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
bool _SpiTransferIsDone_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  */
    RecordActualCall(mock, Record__SpiTransferIsDone());
    /** Fake sets SPI Interrupt Flag bit in SPI Status Register.
     * */
    SetBit(Spi_SPSR, Spi_InterruptFlag);
    /** Fake always returns true:\n 
     *  - Prevents test suite from hanging forever.
     * */
    return true;
}

/* =====[ Mock ClearSpiInterruptFlag() ]===== */
static RecordedCall * Record_ClearSpiInterruptFlag(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "ClearSpiInterruptFlag";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void ClearSpiInterruptFlag_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  */
    RecordActualCall(mock, Record_ClearSpiInterruptFlag());
}

