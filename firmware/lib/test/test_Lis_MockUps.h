#ifndef _TEST_LIS_MOCKUPS_H
#define _TEST_LIS_MOCKUPS_H

void SetUpMock_LisRunClkAt50kHz(void); void TearDownMock_LisRunClkAt50kHz(void);
void SetUpMock_LisInit(void); void TearDownMock_LisInit(void);
void SetUpMock_LisClkOn(void); void TearDownMock_LisClkOn(void);
void SetUpMock_LisClkOff(void);
void TearDownMock_LisClkOff(void);
void SetUpMock_LisWriteCfg(void);
void TearDownMock_LisWriteCfg(void);

#endif // _TEST_LIS_MOCKUPS_H
