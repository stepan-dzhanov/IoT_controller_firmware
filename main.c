#include <iom16.h>
#include <inavr.h>
#include "name.h"
#include "uart.h"
#include "adc.h"
#include "protocol.h"
#include "spi.h"
#include "timer.h"
#include "cooking.h"

int position;
unsigned char time_pulse =0;
unsigned int  time_phase = 0;
unsigned char time_tic =0;
unsigned int  volatile time_set =0;
unsigned char power_tic=0;
unsigned char host_power;
char flag_null=0;
char flag_control=0;
char flag_ac=0;
char int0_flag =0;
char int1_flag =0;

 char device_addr=0;
 char power=0;

 //int mt[3] = {100, 80, 100};
 //int mtime [3] = {120,60,30};
 

  
#pragma vector = ANA_COMP_vect 
__interrupt void ANA_COMP(void) {
   flag_null=1;
   time_tic=0;
   TCNT0=144;
}

#pragma vector = INT0_vect 
__interrupt void INT0_vectINT(void) {
  if(int0_flag)  return;
  else int0_flag = 1;
}

#pragma vector = INT1_vect 
__interrupt void INT1_vectINT(void) {
  if(int1_flag)  return;
  else int1_flag = 1;
}

void main(void) {
  DDRA=0xF8;
  DDRB=0x01;
  DDRC=0x00;
  DDRD=0x22;
  PORTD = 0x0F;
  PORTC=255;
  
  
  
  
  TCCR0 = 0X02;  //Divider   1/8
  TIMSK = 0X01; // Enable Timer
  GICR |= (1<<INT1)|(1<<INT0);
  MCUCR |= (1<<ISC11)|(1<<ISC10)|(1<<ISC00)|(1<<ISC01);
 
 
  
  
  SFIOR &=~(1<<ACME); //Disable analog multiplexor
  ACSR=0;
  ACSR |=(1<<ACIE); //Enable interrupt comparator
  ACSR |=(1<<ACIS1); //Falling output edge
  USART_Init(25);
  InitADC();
  SPIInit();
  __delay_cycles(1000000);
  device_addr=0;
  __delay_cycles(100000);
  __enable_interrupt();

 
  

  
  while(1)  {
    Processing_reqest ();
    CoockScripts();
  
 } 
}
