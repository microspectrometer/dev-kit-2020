#include "S13131.h"

uint16_t exposure_ticks;
uint8_t MSB(uint16_t msb_lsb);
uint8_t LSB(uint16_t msb_lsb);
void S13131PinSetup(void);
void S13131StartClocking(void);
void WaitForS13131ClkLow(void);
void WaitForS13131ClkHigh(void);
void S13131Expose(void);
