

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
