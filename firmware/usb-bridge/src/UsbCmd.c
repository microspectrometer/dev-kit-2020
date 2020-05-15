#include "UsbCmd.h"
/* ------------------ */
/* | ---Commands--- | */
/* ------------------ */
void NullCommand(void);
void GetBridgeLED(void);
void SetBridgeLED(void);
void GetSensorLED(void);
void SetSensorLED(void);
void GetSensorConfig(void);
void SetSensorConfig(void);
void GetExposure(void);
void SetExposure(void);

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
uint8_t ReadLedState(void);
