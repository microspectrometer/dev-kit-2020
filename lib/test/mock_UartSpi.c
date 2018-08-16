#include "mock_UartSpi.h"
#include <Mock.h>
#include <RecordedCall.h>
#include <RecordedArg.h>

static RecordedCall * Mock_UartSpiTxBufferIsEmpty(void)
{
    char const *call_name = "UartSpiTxBufferIsEmpty";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_UartSpiTxBufferIsEmpty(void) {
    RecordExpectedCall(mock, Mock_UartSpiTxBufferIsEmpty());
}
bool UartSpiTxBufferIsEmpty_example[] = {true};  // example return values
bool *UartSpiTxBufferIsEmpty_StubbedReturnValue = UartSpiTxBufferIsEmpty_example;
bool UartSpiTxBufferIsEmpty_Stubbed(void) {
    RecordActualCall(mock, Mock_UartSpiTxBufferIsEmpty());
    return *(UartSpiTxBufferIsEmpty_StubbedReturnValue++);
}

static RecordedCall * Mock_UartSpiTransferIsDone(void)
{
    char const *call_name = "UartSpiTransferIsDone";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_UartSpiTransferIsDone(void) {
    RecordExpectedCall(mock, Mock_UartSpiTransferIsDone());
}
bool UartSpiTransferIsDone_example[] = {true};  // example return values
bool *UartSpiTransferIsDone_StubbedReturnValue = UartSpiTransferIsDone_example;
bool UartSpiTransferIsDone_Stubbed(void) {
    RecordActualCall(mock, Mock_UartSpiTransferIsDone());
    return *(UartSpiTransferIsDone_StubbedReturnValue++);
}
