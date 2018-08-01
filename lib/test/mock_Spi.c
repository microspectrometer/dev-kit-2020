#include "mock_Spi.h"
#include <Mock.h>
#include <RecordedCall.h>
#include <RecordedArg.h>

static RecordedCall * Mock_SpiMasterOpenSpi(void)
{
    char const *call_name = "SpiMasterOpenSpi";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_SpiMasterOpenSpi(void) {
    RecordExpectedCall(mock, Mock_SpiMasterOpenSpi());
}
void SpiMasterOpenSpi_Stubbed(void) {
    RecordActualCall(mock, Mock_SpiMasterOpenSpi());
}

static RecordedCall * Mock_SpiMasterCloseSpi(void)
{
    char const *call_name = "SpiMasterCloseSpi";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_SpiMasterCloseSpi(void) {
    RecordExpectedCall(mock, Mock_SpiMasterCloseSpi());
}
void SpiMasterCloseSpi_Stubbed(void) {
    RecordActualCall(mock, Mock_SpiMasterCloseSpi());
}

static RecordedCall * Mock_SpiTransferIsDone(void)
{
    char const *call_name = "SpiTransferIsDone";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_SpiTransferIsDone(void) {
    RecordExpectedCall(mock, Mock_SpiTransferIsDone());
}
bool SpiTransferIsDone_SPIF_example[] = {true};  // example return values
bool *SpiTransferIsDone_StubbedReturnValue = SpiTransferIsDone_SPIF_example;
bool SpiTransferIsDone_Stubbed(void) {
    RecordActualCall(mock, Mock_SpiTransferIsDone());
    return *(SpiTransferIsDone_StubbedReturnValue++);
}
