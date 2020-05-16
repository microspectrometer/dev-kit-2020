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
void CaptureFrame(void);

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
bool LedNumIsValid(bicolorled_num);
uint8_t ReadLedState(bicolorled_num);
void LisReadout(uint16_t num_pixels);
