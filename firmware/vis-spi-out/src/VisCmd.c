#include "VisCmd.h"
/* ------------------ */
/* | ---Commands--- | */
/* ------------------ */
void ReplyCommandInvalid(void);
void NullCommand(void);
void GetSensorLED(void);
void SetSensorConfig(void);

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
uint8_t ReadLedState(uint8_t); // -> led_state
