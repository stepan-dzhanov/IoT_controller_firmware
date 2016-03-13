#include <iom16.h>
#include <inavr.h>
#include "name.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "spi.h"

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
 
unsigned char TxAddress[] = {
    0xE7,
    0xE7,
    0xE7,
    0xE7,
    0xE7
};
/* My address */
unsigned char MyAddress[] = {
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

#define TIME_UPDATE 225
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
  TM_NRF24L01_Transmit_Status_t transmissionStatus;
  char data = 15;
  unsigned int temp, power;
  signed char t;
  
  char tx_buff [64];
  
  float temp2;

  
  DDRA=0x73;
  DDRB=0xB3;
  DDRC=0x00;
  DDRD=0x22;
  PORTD = 0x0F;
  PORTC=255;
  MCUCR |=(1<<SM1)|(1<<SE); // power down enable 
  
#ifndef _DEBUG 
  
  //RTCInit();
  
 
  __delay_cycles(1000000);
  
 
 
  __enable_interrupt();
  
#endif
  
    
  
  
  
    TM_NRF24L01_Init(15, 32);
  
    
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
     if (TM_NRF24L01_DataReady()) {
            
       
            
            /* Get data from NRF24L01+ */
            TM_NRF24L01_GetData(dataIn);
            sprintf((char *)dataOut, "arm");
            if(!memcmp(&dataIn,&dataOut,3)){
              
              PORTB &=~ (1<<0);
              PORTA &=~ (1<<0);
            }
             sprintf((char *)dataOut, "drm");
            if(!memcmp(&dataIn,&dataOut,3)){
              
              PORTB |= (1<<0);
              PORTA |= (1<<0);
            }
            __delay_cycles(1000000);
            
            /* Send it back, automatically goes to TX mode */
            sprintf((char *)dataOut, "OK");
            TM_NRF24L01_Transmit(dataOut);
            
            
            do {
                transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
            } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
            do {
                transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
            } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
            do {
                transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
            } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);
            /* Send done */
          
            
            /* Go back to RX Mode */
            TM_NRF24L01_PowerUpRx();        
        }
    




 } 
}
