#include "VisCmd.h"
/* ------------------ */
/* | ---Commands--- | */
/* ------------------ */
void ReplyCommandInvalid(void);
void NullCommand(void);
void GetSensorLED(void);
void SetSensorLED(void);
void GetSensorConfig(void);
void SetSensorConfig(void);
void GetExposure(void);
void SetExposure(void);

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
bool LedNumIsValid(bicolorled_num);
uint8_t ReadLedState(bicolorled_num);
