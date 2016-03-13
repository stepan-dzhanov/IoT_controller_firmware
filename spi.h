

#ifndef __SPI__
#define __SPI__

void SPIInit();
void SPI_MasterTransmit(char cData);
unsigned char SPI_MasterRecive();
unsigned char TM_SPI_Send(char cData);
void TM_SPI_SendMulti(unsigned char* dataOut, unsigned char* dataIn, int count);
void TM_SPI_WriteMulti( unsigned char* dataOut, int count);
void TM_SPI_ReadMulti(unsigned char* dataIn, unsigned char dummy, int count);


#endif