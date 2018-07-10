#ifndef _MOCK_FT1248_H
#define _MOCK_FT1248_H

#include <stdint.h>    // uint8_t
#include <stdbool.h>   // bool

//=====[ Low-level mocks ]=====
void Expect_FtActivateInterface(void);
void FtActivateInterface_Stubbed(void);

void Expect_FtPushData(void);
void FtPushData_Stubbed(void);

void Expect_FtOutputByte(uint8_t arg1);
void FtOutputByte_Stubbed(uint8_t arg1);

void Expect_FtPullData(void);
void FtPullData_Stubbed(void);

void Expect_FtLetMasterDriveBus(void);
void FtLetMasterDriveBus_Stubbed(void);

void Expect_FtLetSlaveDriveBus(void);
void FtLetSlaveDriveBus_Stubbed(void);

void Expect_FtIsBusOk(void);
bool FtIsBusOk_Stubbed(void);
extern bool FtIsBusOk_StubbedReturnValue;

void Expect_FtReadData(void);
uint8_t FtReadData_Stubbed(void);
extern uint8_t FtReadData_StubbedReturnValue;

//=====[ High-level mocks ]=====
void Expect_FtSendCommand(uint8_t arg1);
void FtSendCommand_Stubbed(uint8_t arg1);

extern bool FtBusTurnaround_StubbedReturnValue;
void Expect_FtBusTurnaround(void);
bool FtBusTurnaround_Stubbed(void);
void Expect_FtDeactivateInterface(void);
void FtDeactivateInterface_Stubbed(void);

#endif // _MOCK_FT1248_H
