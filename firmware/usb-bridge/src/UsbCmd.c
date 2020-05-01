#include "UsbCmd.h"
/* ------------------ */
/* | ---Commands--- | */
/* ------------------ */
void NullCommand(void);
void GetBridgeLED(void);

/* ---------------------------------------- */
/* | ---Command helpers (not commands)--- | */
/* ---------------------------------------- */
uint8_t ReadLedState(void);
