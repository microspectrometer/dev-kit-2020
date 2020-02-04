/** \file Usb_faked.c
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
#include "Usb_faked.h"
#include "Mock.h" // record call history in "mock"
#include "Usb.h"

// ---Private functions faked for testing---

/* =====[ Mock _FtClockDatabus(FtDrive) ]===== */
static RecordedCall * Record__FtClockDatabus(uint8_t arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_FtClockDatabus";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void _FtClockDatabus_fake(uint8_t direction)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  - arg1
   *  */
    RecordActualCall(mock, Record__FtClockDatabus(direction));
}

/* =====[ Mock _FtReadDatabus ]===== */
static RecordedCall * Record__FtReadDatabus(uint8_t * arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_FtReadDatabus";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t **)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void _FtReadDatabus_fake(uint8_t * pbyte)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   * */
    RecordActualCall( mock, Record__FtReadDatabus(pbyte) );
}

/* =====[ Mock _FtWriteDatabus ]===== */
static RecordedCall * Record__FtWriteDatabus(uint8_t arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_FtWriteDatabus";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void _FtWriteDatabus_fake(uint8_t byte)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   * */
    RecordActualCall( mock, Record__FtWriteDatabus(byte) );
}

/* =====[ Mock _FtDatabusPinDirection ]===== */
static RecordedCall * Record__FtDatabusPinDirection(uint8_t arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "_FtDatabusPinDirection";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void _FtDatabusPinDirection_fake(uint8_t pin_direction)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   * */
    RecordActualCall( mock, Record__FtDatabusPinDirection(pin_direction) );
}

// ---Ft API used internally by Usb---

/* =====[ Mock FtSelectFT221X() ]===== */
static RecordedCall * Record_FtSelectFT221X(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "FtSelectFT221X";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void FtSelectFT221X_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  */
    RecordActualCall(mock, Record_FtSelectFT221X());
}

/* =====[ Mock FtBusTurnaround() ]===== */
static RecordedCall * Record_FtBusTurnaround(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "FtBusTurnaround";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void FtBusTurnaround_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  */
    RecordActualCall(mock, Record_FtBusTurnaround());
}

/* =====[ Mock FtIsOk() ]===== */
static RecordedCall * Record_FtIsOk(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "FtIsOk";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
bool FtIsOk_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  */
    RecordActualCall(mock, Record_FtIsOk());
    return false;
}

/* =====[ Mock FtRead(pbyte) ]===== */
static RecordedCall * Record_FtRead(uint8_t * arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "FtRead";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_p_uint8_t);
    *((uint8_t **)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void FtRead_fake(uint8_t * pbyte)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name\n 
   *  - arg1
   * */
    RecordActualCall( mock, Record_FtRead(pbyte) );
}

/* =====[ Mock FtWrite(byte) ]===== */
static RecordedCall * Record_FtWrite(uint8_t arg1)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "FtWrite";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8_t);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    // Store the arg records in the call record.
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void FtWrite_fake(uint8_t byte)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  - arg1
   *  */
    RecordActualCall(mock, Record_FtWrite(byte));
}

/* =====[ Mock FtUnselectFT221X() ]===== */
static RecordedCall * Record_FtUnselectFT221X(void)
{ // Define **what is recorded** when fake is called.
    char const *call_name = "FtUnselectFT221X";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void FtUnselectFT221X_fake(void)
{ //! Fake records calls made by **function under test**.
  /** Record:\n 
   *  - call name
   *  */
    RecordActualCall(mock, Record_FtUnselectFT221X());
}

