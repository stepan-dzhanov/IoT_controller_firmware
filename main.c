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


#define CH 19
#define PAYLOAD 32

typedef enum {
  SENSOR_LOW_POWER=1,
  DEVICE_LOW_POWER,
  DEVICE_FULL_POWER
}ID_remote_t;

#define RF_ADDR 1

void main(void) {
  TM_NRF24L01_Transmit_Status_t transmissionStatus;
  char conf_flag = 0;
  char tr_flg=0;
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
  
    
  
  
   
    
  
  
    TM_NRF24L01_Init(CH, PAYLOAD);
  
    
    /* Set 2MBps data rate and -18dBm output power */
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_250k, TM_NRF24L01_OutputPower_0dBm);
    
    /* Set my address, 5 bytes */
    TM_NRF24L01_SetMyAddress(MyAddress);
    /* Set TX address, 5 bytes */
    TM_NRF24L01_SetTxAddress(TxAddress);
    //TM_NRF24L01_PowerDown();
    //__sleep();
    //PORTB |= (1<<0);
    
   
  
  while(1)  {
     
            
       
            
            
           
            __sleep();
          //  PORTB |= (1<<0);
            //while(1);
            if(CheckSleepTimeout()) {
              InitADC();
              __delay_cycles(100);
             
              bat_level = StartADC(2);
              if (bat_level<500);
             
              /* Send it back, automatically goes to TX mode */
            //  PORTB |= (1<<0);
              
              else sprintf((char *)dataOut, "iam");
              dataOut[3] = RF_ADDR;
              dataOut[4] = DEVICE_LOW_POWER;
              if (bat_level<500) sprintf(&dataOut[5], "bat");
              
              TM_NRF24L01_Transmit(dataOut);
              
              
              do {
                  transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
              } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
             // PORTB ~&= (1<<0);
              /* Send done */
            
             
              
              /* Go back to RX Mode */
              TM_NRF24L01_PowerUpRx();  
              
              SetTimer(1);
              while(GetTimer()>0);                // To Do check status inside of delay
                if(TM_NRF24L01_DataReady()) {
                  TM_NRF24L01_GetData(dataIn);
                  sprintf((char *)dataOut, "wdl");
                  
            
                  
                  if((!memcmp(&dataIn[1],&dataOut,3))&&((dataIn[0]&0x0F)==RF_ADDR)){
                   if (dataIn[4]==0) timeout_water = 10;
                   else timeout_water = 10*(dataIn[4]&0x0F)+ 1*(dataIn[5]&0x0F);
                   
                  sprintf((char *)dataOut, "OK");
                  dataOut[2]= RF_ADDR;
                  dataOut[3]=0;
                  TM_NRF24L01_Transmit(dataOut);
                  tr_flg=1;
                   
                   PORTB |= (1<<0);
                   PORTD |= (1<<6);
                   SetTimer(timeout_water);
                   while(GetTimer()>0);
                   PORTB &=~ (1<<0);
                   PORTD &=~ (1<<6);
                   
                   
                  }
                  sprintf((char *)dataOut, "tst");
                  if((!memcmp(&dataIn[1],&dataOut,3))&&((dataIn[0]&0x0F)==RF_ADDR)){//&&((dataIn[0]&0x0F)==RF_ADDR)
                        
                  PORTB |= (1<<0);
                  sprintf((char *)dataOut, "OK");
                  dataOut[2]= RF_ADDR;
                  dataOut[3]=0;
                  TM_NRF24L01_Transmit(dataOut);
                  tr_flg=1;
                  PORTB &=~ (1<<0);
                  
                   
                   
                  }
                  if(tr_flg==1){
                    do {
                    transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
                  } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
                  tr_flg=0;
                  }
                  
                }
                 
            
              OffADC();
              //PORTB &=~ (1<<0);
              
              
              TM_NRF24L01_PowerDown();
              ResetSleepTimeout();
     }
  } 
}
