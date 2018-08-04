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

static RecordedCall * Mock_SpiResponseIsReady(void)
{
    char const *call_name = "SpiResponseIsReady";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_SpiResponseIsReady(void) {
    RecordExpectedCall(mock, Mock_SpiResponseIsReady());
}
bool SpiResponseIsReady_returns[] = {true};  // example return values
bool *SpiResponseIsReady_StubbedReturnValue = SpiResponseIsReady_returns;
bool SpiResponseIsReady_Stubbed(void) {
    RecordActualCall(mock, Mock_SpiResponseIsReady());
    return *(SpiResponseIsReady_StubbedReturnValue++);
}

static RecordedCall * Mock_ReadSpiDataRegister(void)
{
    char const *call_name = "ReadSpiDataRegister";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_ReadSpiDataRegister(void) {
    RecordExpectedCall(mock, Mock_ReadSpiDataRegister());
}
uint8_t ReadSpiDataRegister_StubbedReturnValue = 0xFF;
uint8_t ReadSpiDataRegister_Stubbed(void) {
    RecordActualCall(mock, Mock_ReadSpiDataRegister());
    return ReadSpiDataRegister_StubbedReturnValue;
}

static RecordedCall * Mock_ReadSpiStatusReg(void)
{
    char const *call_name = "ReadSpiStatusReg";
    RecordedCall *record_of_this_call = RecordedCall_new(call_name);
    return record_of_this_call;
}
void Expect_ReadSpiStatusReg(void) {
    RecordExpectedCall(mock, Mock_ReadSpiStatusReg());
}
uint8_t ReadSpiStatusReg_Stubbed(void) {
    RecordActualCall(mock, Mock_ReadSpiStatusReg());
    return 0x00; // I do not care what this returns
}
