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


#define RESET_R_LED PORTA &=~ (1<<6); 
#define RESET_G_LED PORTA &=~ (1<<7); 
#define SET_G_LED PORTA |= (1<<7);  
#define SET_R_LED PORTA |= (1<<6);   
 
 

void main(void) {
  
  char data = 15;
  unsigned int temp, power;
  char beep_fl=0;;
  
  int u1,u2,u3;
  char t;
  
  float temp2;

  
  DDRA=0xF0;
  DDRB=0x03;
  DDRC=0x00;
  DDRD=0x62;
  PORTD = 0x0F;
  PORTC=255;
  
  
  
 
 InitADC();
 InitTimer();
  __delay_cycles(100000);
  __enable_interrupt();
 //Beep(2500);
 // SET_G_LED
  
  

  
  while(1)  {
    SetTimer(1);
    SET_R_LED
    while (GetTimer());
    RESET_R_LED
    SET_G_LED
    SetTimer(1);
    while (GetTimer());
    RESET_G_LED
    
    beep_fl=0;
    t = PIND&0x04;
    if (!t) {
       u1 = StartADC(3);
     u2 = StartADC(1);
     u3 = StartADC(2);
      if((u1<u2)&&(u2<u3)&&(u1>100)&&(u2>200)&&(u3>400)){
      SET_G_LED
      RESET_R_LED
        if(!beep_fl){
        Beep(300);
        beep_fl=1;
        }
      while(!t){
        t = PIND&0x04;
      }
      
      
      }
      else   {
        SET_R_LED
        RESET_G_LED
      if(!beep_fl){
        Beep(1500);
        beep_fl=1;
        }
      while(!t){
        t = PIND&0x04;
       
      }
      
      
      
      }
        
    }
    else {
      RESET_R_LED
      RESET_G_LED
    }



 
 
  
 } 
}
