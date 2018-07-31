#ifndef _MOCK_SPIMASTER_H
#define _MOCK_SPIMASTER_H

#include <stdbool.h>

void Expect_SpiMasterOpenSpi(void);
void SpiMasterOpenSpi_Stubbed(void);

void Expect_SpiMasterCloseSpi(void);
void SpiMasterCloseSpi_Stubbed(void);

void Expect_SpiTransferIsDone(void);
bool SpiTransferIsDone_Stubbed(void);
extern bool *SpiTransferIsDone_StubbedReturnValue;

#endif // _MOCK_SPIMASTER_H
