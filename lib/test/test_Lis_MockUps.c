#include "Lis.h"
#include "mock_Lis.h"
#include "Pwm.h"
#include "mock_Pwm.h"
#include <Mock.h>
#include "test_Lis_MockUps.h"

static void (*PwmResetCounterAtTop_Saved)(void);
static void Stub_PwmResetCounterAtTop(void) {
    PwmResetCounterAtTop_Saved = PwmResetCounterAtTop;
    PwmResetCounterAtTop = PwmResetCounterAtTop_Stubbed;
}
static void Unstub_PwmResetCounterAtTop(void) {
    PwmResetCounterAtTop = PwmResetCounterAtTop_Saved;
}
static void (*PwmTopIsOcr0a_Saved)(void);
static void Stub_PwmTopIsOcr0a(void) {
    PwmTopIsOcr0a_Saved = PwmTopIsOcr0a;
    PwmTopIsOcr0a = PwmTopIsOcr0a_Stubbed;
}
static void Unstub_PwmTopIsOcr0a(void) {
    PwmTopIsOcr0a = PwmTopIsOcr0a_Saved;
}
static void (*PwmClkIsCpuClk_Saved)(void);
static void Stub_PwmClkIsCpuClk(void) {
    PwmClkIsCpuClk_Saved = PwmClkIsCpuClk;
    PwmClkIsCpuClk = PwmClkIsCpuClk_Stubbed;
}
static void Unstub_PwmClkIsCpuClk(void) {
    PwmClkIsCpuClk = PwmClkIsCpuClk_Saved;
}
void SetUpMock_LisRunClkAt50kHz(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_PwmResetCounterAtTop();  // DOF
    Stub_PwmTopIsOcr0a();  // DOF
    Stub_PwmClkIsCpuClk();  // DOF
}
void TearDownMock_LisRunClkAt50kHz(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_PwmResetCounterAtTop();  // DOF
    Unstub_PwmTopIsOcr0a();  // DOF
    Unstub_PwmClkIsCpuClk();  // DOF
}

static void (*LisRunClkAt50kHz_Saved)(void);
static void Stub_LisRunClkAt50kHz(void) {
    LisRunClkAt50kHz_Saved = LisRunClkAt50kHz;
    LisRunClkAt50kHz = LisRunClkAt50kHz_Stubbed;
}
static void Unstub_LisRunClkAt50kHz(void) {
    LisRunClkAt50kHz = LisRunClkAt50kHz_Saved;
}
static void (*LisClkOn_Saved)(void);
static void Stub_LisClkOn(void) {
    LisClkOn_Saved = LisClkOn;
    LisClkOn = LisClkOn_Stubbed;
}
static void Unstub_LisClkOn(void) {
    LisClkOn = LisClkOn_Saved;
}
void SetUpMock_LisInit(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_LisRunClkAt50kHz();  // DOF
    Stub_LisClkOn();  // DOF
}
void TearDownMock_LisInit(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_LisRunClkAt50kHz();  // DOF
    Unstub_LisClkOn();  // DOF
}

static void (*PwmEnableOutputSetUntilMatch_Saved)(void);
static void Stub_PwmEnableOutputSetUntilMatch(void) {
    PwmEnableOutputSetUntilMatch_Saved = PwmEnableOutputSetUntilMatch;
    PwmEnableOutputSetUntilMatch = PwmEnableOutputSetUntilMatch_Stubbed;
}
static void Unstub_PwmEnableOutputSetUntilMatch(void) {
    PwmEnableOutputSetUntilMatch = PwmEnableOutputSetUntilMatch_Saved;
}
void SetUpMock_LisClkOn(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_PwmEnableOutputSetUntilMatch();  // DOF
}
void TearDownMock_LisClkOn(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_PwmEnableOutputSetUntilMatch();  // DOF
}

static void (*PwmDisableOutput_Saved)(void);
static void Stub_PwmDisableOutput(void) {
    PwmDisableOutput_Saved = PwmDisableOutput;
    PwmDisableOutput = PwmDisableOutput_Stubbed;
}
static void Unstub_PwmDisableOutput(void) {
    PwmDisableOutput = PwmDisableOutput_Saved;
}
void SetUpMock_LisClkOff(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_PwmDisableOutput();  // DOF
}
void TearDownMock_LisClkOff(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_PwmDisableOutput();  // DOF
}

static void (*LoadNextCfgBit_Saved)(uint32_t);
static void Stub_LoadNextCfgBit(void) {
    LoadNextCfgBit_Saved = LoadNextCfgBit;
    LoadNextCfgBit = LoadNextCfgBit_Stubbed;
}
static void Unstub_LoadNextCfgBit(void) {
    LoadNextCfgBit = LoadNextCfgBit_Saved;
}
void SetUpMock_LisWriteCfg(void)  // FUT
{
    mock = Mock_new();
    //
    Stub_LoadNextCfgBit();  // DOF
}
void TearDownMock_LisWriteCfg(void)  // FUT
{
    Mock_destroy(mock); mock = NULL;
    //
    Unstub_LoadNextCfgBit();  // DOF
}

