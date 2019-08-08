#ifndef _MOCK_LIS_H
#define _MOCK_LIS_H

#include<stdint.h>

void Expect_LisRunClkAt50kHz(void); void LisRunClkAt50kHz_Stubbed(void);
void Expect_LisClkOn(void); void LisClkOn_Stubbed(void);

#endif // _MOCK_LIS_H
