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
#include "wi_fi.h"







char device_addr=0;

void main(void) {
  
  char tx_buff[64];
  char t;
  int humidity;
  DDRA=0x78;
  DDRB=0x03;
  DDRC=0x00;
  DDRD=0x22;
  PORTD = 0x0F;
  PORTC=255;
 // RTCInit();
  InitTimer();
  USART_Init(25);
  InitADC();
  SPIInit();
  __delay_cycles(1000000);
  device_addr=0;
  __delay_cycles(100000);
  __enable_interrupt();
  
  sprintf(tx_buff, "AT+CWMODE=1\r\n");
  TransmitString(tx_buff,strlen(tx_buff));
  SetTimer(20);
  while(GetTimer()>0);
  sprintf(tx_buff, "AT+CWJAP=\"TOTOLINK N150RT\",\"12345677\"\r\n");
  TransmitString(tx_buff,strlen(tx_buff));
  SetTimer(20);
  while(GetTimer()>0);
  
  while(1)  {
    
 
SetTimer(600);
while(GetTimer()>0);
PORTB |= (1<<0); 

  // OnTimer ();
  // SetTimer(2);
  // while(GetTimer()>0);
 
   OffTimer ();
   t = GetTempDS18B20();
   OnTimer ();
  // humidity = StartADC(0)/4;
 //  if(humidity>100) humidity = 100;
   SendToServer(t,0);
  PORTB &=~ (1<<0);
  
 
 
  
    
    
 
 } 
}
