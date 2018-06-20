#include "Ft1248.h"
#include "ReadWriteBits.h"

//=====[ Ft1248: FtSendCommand ]=====
//-----------------------------------
// 8-bit Bus Width R/W
//#define FT1248_CMD_WRITE8   0x86
//#define FT1248_CMD_READ8    0xC6
//static uint8_t const FT1248Cmd_8BitWide_Write = 0x86;

uint8_t const FtCmd_Read  = 0xC6;
void FtSendCommand(uint8_t FtCmd)
{
    FtActivateInterface();
    FtPushData();
    FtLetMasterDriveBus();
    FtOutputByte(FtCmd);
    FtPullData();
}

void FtBusTurnaround(void)
{
    FtLetSlaveDriveBus();
    FtPushData();
    FtPullData();
    FtIsBusOk();
}

static void FtActivateInterface_Implementation(void)
{
    ClearBit(Ft1248_port, Ft1248_Ss);
}

void (*FtActivateInterface)(void) = FtActivateInterface_Implementation;

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

static void FtIsBusOk_Implementation(void)
{}

void (*FtIsBusOk)(void) = FtIsBusOk_Implementation;
