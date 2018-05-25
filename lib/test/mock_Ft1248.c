#include "mock_Ft1248.h"
#include <Mock.h>           // RecordExpectedCall, RecordActualCall
#include <RecordedCall.h>   // Class: RecordedCall
#include <RecordedArg.h>    // Class: RecordedArg

static RecordedCall * Mock_FtOutputCmdOnMiosio(uint8_t arg1)
{
    char const *call_name = "FtOutputCmdOnMiosio";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    RecordedArg *record_of_arg1 = RecordedArg_new(SetupRecord_uint8);
    *((uint8_t *)record_of_arg1->pArg) = arg1;
    RecordArg(record_of_this_call, record_of_arg1);
    return record_of_this_call;
}
void Expect_FtOutputCmdOnMiosio(uint8_t arg1) {
    RecordExpectedCall(mock, Mock_FtOutputCmdOnMiosio(arg1));
}
void FtOutputCmdOnMiosio_Stubbed(uint8_t arg1) {
    RecordActualCall(mock, Mock_FtOutputCmdOnMiosio(arg1));
}

