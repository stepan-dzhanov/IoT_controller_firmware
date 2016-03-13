

#include <iom16.h>
#include <inavr.h>

#include "spi.h"


void SPIInit()  {
  //DDRB = (1<<5)|(1<<7);
  //DDRB &=~ (1<<6);
  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR);//|(1<<SPR1);
}


void SPI_MasterTransmit(char cData)   {
/* Start transmission */
SPDR = cData;
/* Wait for transmission complete */

while(!(SPSR & (1<<SPIF)));

}


unsigned char SPI_MasterRecive()   {
/* Start transmission */
SPDR = 0x00;
/* Wait for transmission complete */
while(!(SPSR & (1<<SPIF)));
return SPDR;
}


unsigned char TM_SPI_Send(char cData)   {
/* Start transmission */
SPDR = cData;
/* Wait for transmission complete */

while(!(SPSR & (1<<SPIF)));
return SPDR;
}

void TM_SPI_SendMulti(unsigned char* dataOut, unsigned char* dataIn, int count) {
	int i;
	
	
	
	for (i = 0; i < count; i++) {
		/* Fill output buffer with data */
		SPDR = dataOut[i];
		
		/* Wait for SPI to end everything */
		while(!(SPSR & (1<<SPIF)))
		
		/* Read data register */
		dataIn[i] = SPDR;
	}
}


void TM_SPI_WriteMulti( unsigned char* dataOut, int count) {
	int i;	
	char x;
	
	
	for (i = 0; i < count; i++) {
		/* Fill output buffer with data */
		SPDR = dataOut[i];
		
		/* Wait for SPI to end everything */
		while(!(SPSR & (1<<SPIF)))
		
		/* Read data register */
		x=SPDR;
	}
}

void TM_SPI_ReadMulti(unsigned char* dataIn, unsigned char dummy, int count) {
	int i;
	
	
	
	for (i = 0; i < count; i++) {
		/* Fill output buffer with data */
		SPDR  = dummy;
		
		/* Wait for SPI to end everything */
		while(!(SPSR & (1<<SPIF)));
		
		/* Save data to buffer */
		dataIn[i] = SPDR;
	}
}