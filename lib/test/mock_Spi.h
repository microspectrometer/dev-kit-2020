#ifndef _MOCK_SPI_H
#define _MOCK_SPI_H

#include <stdint.h>
#include <stdbool.h>

void Expect_SpiMasterOpenSpi(void);
void SpiMasterOpenSpi_Stubbed(void);

void Expect_SpiMasterCloseSpi(void);
void SpiMasterCloseSpi_Stubbed(void);

void Expect_SpiTransferIsDone(void);
bool SpiTransferIsDone_Stubbed(void);
extern bool *SpiTransferIsDone_StubbedReturnValue;

void Expect_SpiResponseIsReady(void);
bool SpiResponseIsReady_Stubbed(void);
extern bool *SpiResponseIsReady_StubbedReturnValue;

void Expect_ReadSpiDataRegister(void);
uint8_t ReadSpiDataRegister_Stubbed(void);
extern uint8_t ReadSpiDataRegister_StubbedReturnValue;
void Expect_ReadSpiStatusRegister(void);
uint8_t ReadSpiStatusRegister_Stubbed(void);
void Expect_ClearPendingSpiInterrupt(void);
void ClearPendingSpiInterrupt_Stubbed(void);

#endif // _MOCK_SPI_H
