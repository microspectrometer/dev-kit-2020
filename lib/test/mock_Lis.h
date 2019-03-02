#ifndef _MOCK_LIS_H
#define _MOCK_LIS_H

#include<stdint.h>

void Expect_LisRunClkAt50kHz(void); void LisRunClkAt50kHz_Stubbed(void);
void Expect_LisClkOn(void); void LisClkOn_Stubbed(void);
void Expect_LoadNextCfgBit(uint32_t arg1); void LoadNextCfgBit_Stubbed(uint32_t arg1);

#endif // _MOCK_LIS_H
