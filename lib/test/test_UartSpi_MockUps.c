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
void SetUpMock_UartSpiRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_UartSpiTxBufferIsEmpty();  // DOF
    Stub_UartSpiTransferIsDone();  // DOF
}
void TearDownMock_UartSpiRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_UartSpiTxBufferIsEmpty();  // DOF
    Unstub_UartSpiTransferIsDone();  // DOF
}

