#include <iom16.h>
#include <inavr.h>
#include "name.h"
#include "uart.h"
#include "adc.h"
#include "protocol.h"
#include "spi.h"
#include "timer.h"
#include "cooking.h"
#include "ds18b20.h"


#include <string.h>
#include <stdio.h>
static int timeout =0;
static char ready =0;
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

#define TIME_UPDATE 75

#pragma vector = TIMER2_OVF_vect 
__interrupt void TIMER2_OVF_vectINT(void)
{
   timeout++;
   if (timeout>=TIME_UPDATE) {
     timeout =0;
     ready = 1;
   }
   
 //   asm volatile("nop"::);
    //_delay_us(10);
}

SendToServer(int data1, int data2) {
  
 
  
char tx_buff [64];
char tx_buff1 [64];
char tx_buff2 [64];
char counter;
char i;
  
  
  sprintf(tx_buff, "AT+RST\r\n");
  TransmitString(tx_buff,strlen(tx_buff));
  SetTimer(4);
  while(GetTimer()>0);
  sprintf(tx_buff, "AT+CIPMUX=1\r\n");
  TransmitString(tx_buff,strlen(tx_buff));
  SetTimer(1);
  while(GetTimer()>0);
  sprintf(tx_buff, "AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n");
  TransmitString(tx_buff,strlen(tx_buff));
  SetTimer(4);
  while(GetTimer()>0);
  
  sprintf(tx_buff, "AT+CIPSEND=4,");
  
  sprintf(tx_buff2,"%d",data1,"/n");
  counter = strlen(tx_buff2);
  sprintf(tx_buff1,"%d",counter+48,"/n");
                 ///TransmitString(tx_buff1,strlen(tx_buff1));
  
  for(i=0;i<2; i++){
    tx_buff[13+i] = tx_buff1[i];
  }

  tx_buff[13+i] = '\r';
  i++;
  tx_buff[13+i] = '\n';
  i++;
  tx_buff[13+i] = 0;
  
  
  TransmitString(tx_buff,strlen(tx_buff));
  SetTimer(2);
  while(GetTimer()>0);
  
  sprintf(tx_buff, "GET /update?api_key=1CV2GX9SLOJGA16D&field3=");//28\r\n\r\n");
  for(i=0;i<counter; i++){
    tx_buff[44+i] = tx_buff2[i];
  }
          

  tx_buff[44+i] = '\r';
  i++;
  tx_buff[44+i] = '\n';
  
  i++;
  tx_buff[44+i] = '\r';
  i++;
  tx_buff[44+i] = '\n';
  i++;
  tx_buff[44+i] = 0;
  
  TransmitString(tx_buff,strlen(tx_buff));
  
  SetTimer(5);
  while(GetTimer()>0);
  
 
  
}

void RTCInit(void)
{
     TCCR0 = 0X02;
     MCUCR |= (1<<SM0)|(1<<SM1)|(1<<SE);
    //Disable timer2 interrupts
    //TIMSK  = 0;
    //Enable asynchronous mode
    ASSR  = (1<<AS2);
    //set initial counter value
    TCNT2=0;
    //set prescaller 1024
    TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20);
    while (ASSR & ((1<<TCN2UB)|(1<<TCR2UB)));
    //enable TOV2 interrupt
    TIMSK  |= (1<<TOIE2)|(1<<TOIE0);
}

void main(void) {
  
  char data = 15;
  unsigned int temp, power;
  char t;
  
  char tx_buff [64];
  
  float temp2;

  
  DDRA=0xF8;
  DDRB=0x01;
  DDRC=0x00;
  DDRD=0x22;
  PORTD = 0x0F;
  PORTC=255;
  
  
  
  RTCInit();
 // TCCR0 = 0X02;  //Divider   1/8
 // TIMSK = (1<<TOIE0); // Enable Timer
 // GICR |= (1<<INT1)|(1<<INT0);
 
 
 
  
  
  //SFIOR &=~(1<<ACME); //Disable analog multiplexor
 //ACSR=0;
 // ACSR |=(1<<ACIE); //Enable interrupt comparator
 // ACSR |=(1<<ACIS1); //Falling output edge
  USART_Init(25);
  InitADC();
  SPIInit();
  __delay_cycles(1000000);
  device_addr=0;
  __delay_cycles(100000);
  __enable_interrupt();
  
  
  

  
  while(1)  {
    
  

 __sleep(); 
 if(ready){
   PORTB |= (1<<0);  
   TIMSK &=~(1<<TOIE0); // Enable Timer
   t = GetTempDS18B20();
   TIMSK |= (1<<TOIE0); // Enable Timer
   SendToServer(t,power);
   ready=0;
   timeout=0;
   PORTB &=~ (1<<0);  
 }
 
   //PORTB &=~ (1<<0);
  
    // power = StartADC(1);
    // if(power<440)power =100;
   //  else power = (power - 440)*10;
  
     
   //  TIMSK = 0X00; // Enable Timer
   //  t = GetTempDS18B20();
   //  TIMSK = 0X01; // Enable Timer
   //  SendToServer(t,power);
    // SetTimer(600);
   //  while(GetTimer()>0);
   
    
    
    
    
   
 
  
 } 
}
