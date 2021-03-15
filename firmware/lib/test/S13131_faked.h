#ifndef _S13131_FAKED_H
#define _S13131_FAKED_H
void S13131Expose_fake(void);
void S13131Readout_fake(void); // Readout defined in VisCmd.c
void WaitForS13131ClkLow_fake(void);
#endif
