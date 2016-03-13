
#include "adc.h"
#include <iom16.h>
#include <inavr.h>



void InitADC() {
   //ADMUX=0x00;
   ADCSRA |=(1<<ADEN);
   ADMUX|=(1<<REFS1)|(1<<REFS0)|(1<<MUX1)|(1<<MUX0)|(1<<MUX2);
}

unsigned int StartADC(char ch) {
  unsigned int result=0;
  unsigned int adcl, adch;
  __delay_cycles(3000);
  ADCSRA |=(1<<ADSC);
  while ( ADCSRA & (1<<ADSC) );
  adcl=ADCL;
  adch=ADCH;
  result=adcl|(adch<<8);
  return result;
}


void OffADC() {
  ADCSRA=0;
  ADMUX = 0;
  
}