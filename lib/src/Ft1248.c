#include "Ft1248.h"
#include "ReadWriteBits.h"

//=====[ Ft1248: FtSendCommand ]=====
//-----------------------------------
// 8-bit Bus Width R/W
//#define FT1248_CMD_WRITE8   0x86
//#define FT1248_CMD_READ8    0xC6
//static uint8_t const FT1248Cmd_8BitWide_Write = 0x86;

//=====[ High-level API ]=====
#include "DebugLed.h"
void FtInit(void)
{
    FtSetMisoAsInput();
    FtEnablePullupOnMiso();
    FtSetMiosioAsInputs();
    FtEnablePullupsOnMiosio();
}
void FtSetMisoAsInput(void)
{
    ClearBit(Ft1248_ddr, Ft1248_Miso);
}
void FtEnablePullupOnMiso(void)
{
    SetBit(Ft1248_port, Ft1248_Miso);
}
void FtSetMiosioAsInputs(void)
{
    *FtMiosio_ddr = 0x00;
}
void FtEnablePullupsOnMiosio(void)
{
    *FtMiosio_port = 0xFF;
}
void FtSckLow(void)
{
    ClearBit(Ft1248_port, Ft1248_Sck);  // drive low
    SetBit(Ft1248_ddr, Ft1248_Sck);     // cfg as output
}
void FtSsHigh(void)
{
    SetBit(Ft1248_port, Ft1248_Ss);     // drive high
    SetBit(Ft1248_ddr, Ft1248_Ss);      // cfg as output
}

uint8_t const FtCmd_Read  = 0xC6;
uint8_t const FtCmd_Write = 0x86;
static void FtSendCommand_Implementation(uint8_t FtCmd)
{
    FtActivateInterface();
    FtPushData();
    FtLetMasterDriveBus();
    FtOutputByte(FtCmd);
    FtPullData();
}
void (*FtSendCommand)(uint8_t) = FtSendCommand_Implementation;

static bool FtBusTurnaround_Implementation(void)
{
    FtLetSlaveDriveBus();
    FtPushData();                   // tells everyone to output to bus
    FtPullData();                   // tells everyone to input from bus
    return FtIsBusOk();
}
bool (*FtBusTurnaround)(void) = FtBusTurnaround_Implementation;

static bool FtRead_Implementation(uint8_t *read_buffer)
{
    FtPushData();                   // tells everyone to output to bus
    FtPullData();                   // tells everyone to input from bus
    if (!FtIsBusOk()) return false; // buffer is empty
    *read_buffer = FtReadData();   // buffer is not empty, write to mem
    return true;
}
bool (*FtRead)(uint8_t *read_buffer) = FtRead_Implementation;

static bool FtWrite_Implementation(uint8_t *write_buffer)
{
    FtPushData();                   // tells everyone to output to bus
    FtWriteData(*write_buffer);     // drive Miosio with data
    FtPullData();                   // tells everyone to input from bus
    if (!FtIsBusOk()) return false; // failed to write to transmit buffer
    return true;
}

bool (*FtWrite)(uint8_t *write_buffer) = FtWrite_Implementation;
//=====[ Low-level API ]=====
static void FtActivateInterface_Implementation(void)
{
    ClearBit(Ft1248_port, Ft1248_Ss);
}

void (*FtActivateInterface)(void) = FtActivateInterface_Implementation;

static void FtDeactivateInterface_Implementation(void)
{
    SetBit(Ft1248_port, Ft1248_Ss);
}
void (*FtDeactivateInterface)(void) = FtDeactivateInterface_Implementation;

static void FtPushData_Implementation(void)
{
    SetBit(Ft1248_port, Ft1248_Sck);
}

void (*FtPushData)(void) = FtPushData_Implementation;

static void FtOutputByte_Implementation(uint8_t FtCmd)
{
    *FtMiosio_port = FtCmd;
}

void (*FtOutputByte)(uint8_t) = FtOutputByte_Implementation;

static void FtPullData_Implementation(void)
{
    ClearBit(Ft1248_port, Ft1248_Sck);
}

void (*FtPullData)(void) = FtPullData_Implementation;

static void FtLetMasterDriveBus_Implementation(void)
{
    *FtMiosio_ddr = 0xFF;
}

void (*FtLetMasterDriveBus)(void) = FtLetMasterDriveBus_Implementation;

static void FtLetSlaveDriveBus_Implementation(void)
{
    *FtMiosio_ddr = 0x00;
}

void (*FtLetSlaveDriveBus)(void) = FtLetSlaveDriveBus_Implementation;

static bool FtIsBusOk_Implementation(void)
{
    return BitIsClear(Ft1248_pin, Ft1248_Miso);
}
bool (*FtIsBusOk)(void) = FtIsBusOk_Implementation;
bool (*FtHasDataToRead)(void) = FtIsBusOk_Implementation;

static bool FtHasRoomToWrite_Implementation(void)
{
    return BitIsClear(FtMiosio_pin, FtMiosio0);
}
bool (*FtHasRoomToWrite)(void) = FtHasRoomToWrite_Implementation;

static uint8_t FtReadData_Implementation(void)
{
    return *FtMiosio_pin;
}
uint8_t (*FtReadData)(void) = FtReadData_Implementation;

static void FtWriteData_Implementation(uint8_t byte_to_write)
{
    *FtMiosio_port = byte_to_write;
}
void (*FtWriteData)(uint8_t) = FtWriteData_Implementation;
