#ifndef _TEST_SPIMASTER_H
#define _TEST_SPIMASTER_H

void SpiMasterOpenSpi_selects_the_SPI_slave(void);
void SpiMasterCloseSpi_unselects_the_SPI_slave(void);
void SpiMasterInit_pulls_Ss_high(void);

#endif // _TEST_SPIMASTER_H
