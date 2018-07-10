#include "test_Usb.h"
#include "unity.h"
//=====[ List of tests to write ]=====
    // [x] UsbRead should:
    //  - call FtSendCommand(FtCmd_Read)
    //  - check if ok to proceed after FtBusTuraround()
    //  - loop FtRead() until buffer is empty
    //  - then call FtDeactivateInterface() to end Ft1248 session
    //      - FtRead() leaves SCK low.
    //          - This is good.
    //          - SCK should be low before deactivating the interface.
void SetUp_NothingForUsb(void){}
void TearDown_NothingForUsb(void){}

void UsbRead_should_read_until_buffer_is_empty(void)
{
    TEST_FAIL_MESSAGE("Implement test.");

    //=====[ Set expectations ]=====
    //TEST_ASSERT_TRUE_MESSAGE(
    //    RanAsHoped(mock),           // If this is false,
    //    WhyDidItFail(mock)          // print this message.
    //    );
}


