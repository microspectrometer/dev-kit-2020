#include "Spi.h"
#include "mock_Spi.h"
#include <Mock.h>
#include "test_Spi_MockUps.h"

static void (*SpiMasterOpenSpi_Saved)(void);
static void Stub_SpiMasterOpenSpi(void) {
    SpiMasterOpenSpi_Saved = SpiMasterOpenSpi;
    SpiMasterOpenSpi = SpiMasterOpenSpi_Stubbed;
}
static void Unstub_SpiMasterOpenSpi(void) {
    SpiMasterOpenSpi = SpiMasterOpenSpi_Saved;
}
static void (*SpiMasterCloseSpi_Saved)(void);
static void Stub_SpiMasterCloseSpi(void) {
    SpiMasterCloseSpi_Saved = SpiMasterCloseSpi;
    SpiMasterCloseSpi = SpiMasterCloseSpi_Stubbed;
}
static void Unstub_SpiMasterCloseSpi(void) {
    SpiMasterCloseSpi = SpiMasterCloseSpi_Saved;
}
static bool (*SpiTransferIsDone_Saved)(void);
static void Stub_SpiTransferIsDone(void) {
    SpiTransferIsDone_Saved = SpiTransferIsDone;
    SpiTransferIsDone = SpiTransferIsDone_Stubbed;
}
static void Unstub_SpiTransferIsDone(void) {
    SpiTransferIsDone = SpiTransferIsDone_Saved;
}
void SetUpMock_SpiMasterWrite(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_SpiMasterOpenSpi();  // DOF
    Stub_SpiMasterCloseSpi();  // DOF
    Stub_SpiTransferIsDone();  // DOF
}
void TearDownMock_SpiMasterWrite(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_SpiMasterOpenSpi();  // DOF
    Unstub_SpiMasterCloseSpi();  // DOF
    Unstub_SpiTransferIsDone();  // DOF
}
static uint8_t (*ReadSpiDataRegister_Saved)(void);
static void Stub_ReadSpiDataRegister(void) {
    ReadSpiDataRegister_Saved = ReadSpiDataRegister;
    ReadSpiDataRegister = ReadSpiDataRegister_Stubbed;
}
static void Unstub_ReadSpiDataRegister(void) {
    ReadSpiDataRegister = ReadSpiDataRegister_Saved;
}
void SetUpMock_SpiMasterRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_SpiTransferIsDone();  // DOF
    Stub_ReadSpiDataRegister();  // DOF
}
void TearDownMock_SpiMasterRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_SpiTransferIsDone();  // DOF
    Unstub_ReadSpiDataRegister();  // DOF
}
void SetUpMock_SpiSlaveRead(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_SpiTransferIsDone();  // DOF
}
void TearDownMock_SpiSlaveRead(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_SpiTransferIsDone();  // DOF
}

static bool (*SpiResponseIsReady_Saved)(void);
static void Stub_SpiResponseIsReady(void) {
    SpiResponseIsReady_Saved = SpiResponseIsReady;
    SpiResponseIsReady = SpiResponseIsReady_Stubbed;
}
static void Unstub_SpiResponseIsReady(void) {
    SpiResponseIsReady = SpiResponseIsReady_Saved;
}
void SetUpMock_SpiMasterWaitForResponse(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_SpiResponseIsReady();  // DOF
}
void TearDownMock_SpiMasterWaitForResponse(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_SpiResponseIsReady();  // DOF
}

static uint8_t (*ReadSpiStatusRegister_Saved)(void);
static void Stub_ReadSpiStatusRegister(void) {
    ReadSpiStatusRegister_Saved = ReadSpiStatusRegister;
    ReadSpiStatusRegister = ReadSpiStatusRegister_Stubbed;
}
static void Unstub_ReadSpiStatusRegister(void) {
    ReadSpiStatusRegister = ReadSpiStatusRegister_Saved;
}
void SetUpMock_ClearPendingSpiInterrupt(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_ReadSpiStatusRegister();  // DOF
    Stub_ReadSpiDataRegister();  // DOF
}
void TearDownMock_ClearPendingSpiInterrupt(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_ReadSpiStatusRegister();  // DOF
    Unstub_ReadSpiDataRegister();  // DOF
}

static void (*ClearPendingSpiInterrupt_Saved)(void);
static void Stub_ClearPendingSpiInterrupt(void) {
    ClearPendingSpiInterrupt_Saved = ClearPendingSpiInterrupt;
    ClearPendingSpiInterrupt = ClearPendingSpiInterrupt_Stubbed;
}
static void Unstub_ClearPendingSpiInterrupt(void) {
    ClearPendingSpiInterrupt = ClearPendingSpiInterrupt_Saved;
}
void SetUpMock_SpiMasterInit(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_ClearPendingSpiInterrupt();  // DOF
}
void TearDownMock_SpiMasterInit(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_ClearPendingSpiInterrupt();  // DOF
}
void SetUpMock_SpiSlaveInit(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_ClearPendingSpiInterrupt();  // DOF
}
void TearDownMock_SpiSlaveInit(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_ClearPendingSpiInterrupt();  // DOF
}

static void (*DisableSpi_Saved)(void);
static void Stub_DisableSpi(void) {
    DisableSpi_Saved = DisableSpi;
    DisableSpi = DisableSpi_Stubbed;
}
static void Unstub_DisableSpi(void) {
    DisableSpi = DisableSpi_Saved;
}
static void (*EnableSpi_Saved)(void);
static void Stub_EnableSpi(void) {
    EnableSpi_Saved = EnableSpi;
    EnableSpi = EnableSpi_Stubbed;
}
static void Unstub_EnableSpi(void) {
    EnableSpi = EnableSpi_Saved;
}
void SetUpMock_SpiSlaveSignalDataIsReady(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_DisableSpi();  // DOF
    Stub_EnableSpi();  // DOF
}
void TearDownMock_SpiSlaveSignalDataIsReady(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_DisableSpi();  // DOF
    Unstub_EnableSpi();  // DOF
}

