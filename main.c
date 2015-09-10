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
  
 
  char t;
  int humidity;
  DDRA=0x78;
  DDRB=0x03;
  DDRC=0x00;
  DDRD=0x22;
  PORTD = 0x0F;
  PORTC=255;
  RTCInit();
  USART_Init(25);
  InitADC();
  SPIInit();
  __delay_cycles(1000000);
  device_addr=0;
  __delay_cycles(100000);
  __enable_interrupt();
  
  
  

  
  while(1)  {
    



 __sleep(); 
 if(CheckSleepTimeout()){
   ON_WIFI 
   OffTimer ();
   t = GetTempDS18B20();
   OnTimer ();
   humidity = StartADC(0)/4;
   if(humidity>100) humidity = 100;
   SendToServer(t,humidity);
   ResetSleepTimeout();
   OFF_WIFI 
 }
 
  
    
    
 
 } 
}
