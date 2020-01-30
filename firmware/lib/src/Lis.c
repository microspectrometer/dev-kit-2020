#include "Lis.h"
// ---Private---
void Pin_LisPixSelect_SetOutput(void);
void LisPixSelectIdleLow(void);
void Pin_LisClk_SetOutput(void);
void Pin_LisRst_SetOutput(void);
void LisRstIdleLowAfterPowerUp(void);
void Pin_LisSync_SetInput(void);
void ResetPwmTimerAtTop(void);
void PwmTimerTopIsOCR0A(void);
void PwmTimerClockedByCpu_NoPrescaling(void);
void LisClkFreq50kHz(void);
void LisClkOn(void);
void _ConfigAs28bits(uint8_t *config);
void _WaitForLisClkLow(void);
void _WaitForLisClkHigh(void);
void _EnterLisProgrammingMode(void);
void _WriteLisConfigBit(uint8_t const * config, uint8_t bit_index);
void _Write28bitLisConfig(uint8_t const *config);
void _ExitLisProgrammingMode(void);

// ---API---
void LisInit(void);
bool LisConfigIsValid(
    uint8_t binning,
    uint8_t gain,
    uint8_t active_rows);
void LisWriteConfig(void);
