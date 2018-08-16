#ifndef _MOCK_UARTSPI_H
#define _MOCK_UARTSPI_H

#include <stdbool.h>

void Expect_UartSpiTxBufferIsEmpty(void);
bool UartSpiTxBufferIsEmpty_Stubbed(void);
extern bool *UartSpiTxBufferIsEmpty_StubbedReturnValue;
void Expect_UartSpiTransferIsDone(void);
bool UartSpiTransferIsDone_Stubbed(void);
extern bool *UartSpiTransferIsDone_StubbedReturnValue;

#endif // _MOCK_UARTSPI_H
