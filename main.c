#include <iom16.h>
#include <inavr.h>
#include "name.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "spi.h"
#include "adc.h"
#include "timer.h"
#include "m8_nrf24l01.h"


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
 
unsigned char MyAddress[] = {
    0xE7,
    0xE7,
    0xE7,
    0xE7,
    0xE7
};
/* Receiver address */
unsigned char TxAddress[] = {
    0x7E,
    0x7E,
    0x7E,
    0x7E,
    0x7E
};
 
 
unsigned char dataOut[32], dataIn[32];

 //int mt[3] = {100, 80, 100};
 //int mtime [3] = {120,60,30};
 

  
/*#pragma vector = ANA_COMP_vect 
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
}*/



#pragma vector = INT1_vect 
__interrupt void INT1_vectINT(void) {
  if(int1_flag)  return;
  else int1_flag = 1;
}




void main(void) {
  TM_NRF24L01_Transmit_Status_t transmissionStatus;
  char conf_flag = 0;
  unsigned int bat_level, power;
  volatile unsigned char t;
  
  char tx_buff [64];
  
 char timeout_water =0;

  
  DDRA=0x03;
  PORTA = 0x40;
  DDRB=0xB3;
  DDRC=0x00;
  DDRD=0x62;
  PORTD = 0x08;
  PORTC=255;
 
 // GICR |= (1<<INT1);	
  InitTimer() ;
  InitADC();
   //Beep(500);
#ifndef _DEBUG 
  
  RTCInit();
  
 
  __delay_cycles(1000000);
  
 
 
  __enable_interrupt();
  
#endif
  
    
  
  
   
    
  
  
    TM_NRF24L01_Init(19, 32);
  
    
    /* Set 2MBps data rate and -18dBm output power */
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_0dBm);
    
    /* Set my address, 5 bytes */
    TM_NRF24L01_SetMyAddress(MyAddress);
    /* Set TX address, 5 bytes */
    TM_NRF24L01_SetTxAddress(TxAddress);
    //TM_NRF24L01_PowerDown();
    //__sleep();
    //PORTB |= (1<<0);
    
   
  
  while(1)  {
     
            
       
            
            
           
            __sleep();
            if(CheckSleepTimeout()) {
              InitADC();
              __delay_cycles(100);
             
              bat_level = StartADC(2);
              if (bat_level<500);
             
              /* Send it back, automatically goes to TX mode */
            //  PORTB |= (1<<0);
              if (bat_level<500) sprintf((char *)dataOut, "iambat");
              else sprintf((char *)dataOut, "iam");
              TM_NRF24L01_Transmit(dataOut);
              
              
              do {
                  transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
              } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
             // PORTB ~&= (1<<0);
              /* Send done */
            
              
              /* Go back to RX Mode */
              TM_NRF24L01_PowerUpRx();  
              
              SetTimer(1);
              while(GetTimer()>0);
                if(TM_NRF24L01_DataReady()) {
                  TM_NRF24L01_GetData(dataIn);
                  sprintf((char *)dataOut, "wdl");
                  if(!memcmp(&dataIn,&dataOut,3)){
                   if (dataIn[3]==0) timeout_water = 10;
                   else timeout_water = 10*(dataIn[3]&0x0F0)+ 1*(dataIn[4]&0x0F0);
                   
                   PORTB |= (1<<0);
                   PORTD |= (1<<6);
                   SetTimer(timeout_water);
                   while(GetTimer()>0);
                   PORTB &=~ (1<<0);
                   PORTD &=~ (1<<6);
                   
                   
                  }
                  sprintf((char *)dataOut, "tst");
                  if(!memcmp(&dataIn,&dataOut,3)){
                   PORTB |= (1<<0);
                  sprintf((char *)dataOut, "iam");
                  TM_NRF24L01_Transmit(dataOut);
                  do {
                    transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
                  } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
                  
                   PORTB &=~ (1<<0);
                  
                   
                   
                  }
  
                }
                 
            
              OffADC();
              //PORTB &=~ (1<<0);
              
              
              TM_NRF24L01_PowerDown();
              ResetSleepTimeout();
     }
  } 
}
