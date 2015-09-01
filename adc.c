
#include "adc.h"
#include <iom16.h>
#include <inavr.h>



void InitADC() {
   //ADMUX=0x00;
   ADCSRA |=(1<<ADEN);
}

unsigned int StartADC(char ch) {
  unsigned int result=0;
  unsigned int adcl, adch;
  ADMUX=ch;
  __delay_cycles(3000);
  ADCSRA |=(1<<ADSC);
  while ( ADCSRA & (1<<ADSC) );
  adcl=ADCL;
  adch=ADCH;
  result=adcl|(adch<<8);
  return result;
}

