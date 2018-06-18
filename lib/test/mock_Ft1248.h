#ifndef _MOCK_FT1248_H
#define _MOCK_FT1248_H

#include <stdint.h>   // uint8_t

void Expect_FtActivateInterface(void);
void FtActivateInterface_Stubbed(void);
void Expect_FtPushData(void);
void FtPushData_Stubbed(void);
void Expect_FtOutputByte(uint8_t arg1);
void FtOutputByte_Stubbed(uint8_t arg1);
void Expect_FtPullData(void);
void FtPullData_Stubbed(void);

#endif // _MOCK_FT1248_H
