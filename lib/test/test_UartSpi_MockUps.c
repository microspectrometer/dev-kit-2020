#include "UartSpi.h"
#include "mock_UartSpi.h"
#include <Mock.h>
#include "test_UartSpi_MockUps.h"

static bool (*UartSpiTxBufferIsEmpty_Saved)(void);
static void Stub_UartSpiTxBufferIsEmpty(void) {
    UartSpiTxBufferIsEmpty_Saved = UartSpiTxBufferIsEmpty;
    UartSpiTxBufferIsEmpty = UartSpiTxBufferIsEmpty_Stubbed;
}
static void Unstub_UartSpiTxBufferIsEmpty(void) {
    UartSpiTxBufferIsEmpty = UartSpiTxBufferIsEmpty_Saved;
}
static bool (*UartSpiTransferIsDone_Saved)(void);
static void Stub_UartSpiTransferIsDone(void) {
    UartSpiTransferIsDone_Saved = UartSpiTransferIsDone;
    UartSpiTransferIsDone = UartSpiTransferIsDone_Stubbed;
}
static void Unstub_UartSpiTransferIsDone(void) {
    UartSpiTransferIsDone = UartSpiTransferIsDone_Saved;
}
static void (*UartSpiTransfer16bits_Saved)(void);
static void Stub_UartSpiTransfer16bits(void) {
    UartSpiTransfer16bits_Saved = UartSpiTransfer16bits;
    UartSpiTransfer16bits = UartSpiTransfer16bits_Stubbed;
}
static void Unstub_UartSpiTransfer16bits(void) {
    UartSpiTransfer16bits = UartSpiTransfer16bits_Saved;
}
static void (*UartSpiStartAdcConversion_Saved)(void);
static void Stub_UartSpiStartAdcConversion(void) {
    UartSpiStartAdcConversion_Saved = UartSpiStartAdcConversion;
    UartSpiStartAdcConversion = UartSpiStartAdcConversion_Stubbed;
}
static void Unstub_UartSpiStartAdcConversion(void) {
    UartSpiStartAdcConversion = UartSpiStartAdcConversion_Saved;
}
static void (*UartSpiStartAdcReadout_Saved)(void);
static void Stub_UartSpiStartAdcReadout(void) {
    UartSpiStartAdcReadout_Saved = UartSpiStartAdcReadout;
    UartSpiStartAdcReadout = UartSpiStartAdcReadout_Stubbed;
}
static void Unstub_UartSpiStartAdcReadout(void) {
    UartSpiStartAdcReadout = UartSpiStartAdcReadout_Saved;
}
void SetUpMock_UartSpiRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_UartSpiTxBufferIsEmpty();  // DOF
    Stub_UartSpiTransferIsDone();  // DOF
    Stub_UartSpiTransfer16bits();  // DOF
    Stub_UartSpiStartAdcConversion();  // DOF
    Stub_UartSpiStartAdcReadout();  // DOF
}
void TearDownMock_UartSpiRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_UartSpiTxBufferIsEmpty();  // DOF
    Unstub_UartSpiTransferIsDone();  // DOF
    Unstub_UartSpiTransfer16bits();  // DOF
    Unstub_UartSpiStartAdcConversion();  // DOF
    Unstub_UartSpiStartAdcReadout();  // DOF
}

