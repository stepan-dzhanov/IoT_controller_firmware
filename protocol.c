#include "protocol.h"
#include <iom16.h>
#include "uart.h"
#include <inavr.h>
#include <string.h>
#include <stdio.h>
#include "name.h"
#include "adc.h"
#include "cooking.h"
#include "timer.h"

extern char int0_flag,int1_flag;
char rx_buff[32];
char tx_buff[32];
int tempee[10];
int timee [10];
static char pointer =0;
static char flag_ready=0;
extern unsigned char power_tic;
extern char device_addr;
#define STOP_SYMBOL 0x0A
#define MAX_COMMAND 15
const char * strings[16] = {
"?",
"SetB",
"ResetB",
"SetA",
"ResetA",
"GetADC",
"SetDH",
"ResetDH",
"GetINT0",
"GetINT1",
"GetCS",
"SetCP",
"GetCT",
"Set Port source",
"Set MAC",
"Upload network settings",
};


unsigned char  AddrConvert ( char *pData)  {
  unsigned char result;
  result = (pData[0]&0x0f)*10 + (pData[1]&0x0F);
  return result;
}

unsigned int  StrToInt ( char *pData)  {
  unsigned int result;
  result = (pData[0]&0x0f)*100 +(pData[1]&0x0f)*10+ (pData[2]&0x0F);
  return result;
}

void clear_buff(char *buff) {
  char ii;
  for (ii=0; ii<=sizeof(buff); ii++)  {
    buff[ii]=0;
  }
  if(buff == rx_buff) {
    pointer =0;
    flag_ready=0;
  }
  
  
}

#pragma vector = USART_RXC_vect
  __interrupt void USART_RXC(void)   { 
    
    char temp;     
    temp = UDR;
    rx_buff[pointer++]=temp;
   // PORTB &=~ (1<<0);
    if(temp == STOP_SYMBOL) {
      if( !(AddrConvert (rx_buff)==device_addr) ) {
        pointer=0;
        clear_buff(rx_buff);
        // PORTB |= (1<<0);
        return;
      }
     
      else {
        flag_ready=1;
       
        return;
      }
    }
    
  }











void Processing_reqest (void) {
   char i,temp;
   unsigned int result,t;
  
   if (!flag_ready) return;
 
  
 
    for(i=0; i<=(MAX_COMMAND);i++)  {
    if(!memcmp(&rx_buff[2],strings[i],strlen(strings[i]))) break;
    }
    if (i>MAX_COMMAND)  {
      sprintf(tx_buff, "Invalid commandOO\n");
      TransmitString( tx_buff, strlen(tx_buff));
      clear_buff(rx_buff);
       pointer =0;
      return ;
    }
    
    
    if(i<=(MAX_COMMAND)) {
     
     
      
      switch (i)  {
        case 0:
         sprintf(tx_buff, "betta\n");
          TransmitString( tx_buff,strlen(tx_buff));
         // UDPWriteW5100(tx_buff,strlen(tx_buff));
          clear_buff(rx_buff);
          pointer =0;
          break;
      
        case 1:
          PORTA |= (1<<4);
         sprintf(tx_buff, "OK\n");
          TransmitString( tx_buff,strlen(tx_buff));
          clear_buff(rx_buff);
           pointer =0;
          break;//TO DO check addr volume
        case 2:
          PORTA &=~ (1<<4);
          sprintf(tx_buff, "OK\n");
          TransmitString( tx_buff,strlen(tx_buff));
          clear_buff(rx_buff);
           pointer =0;
          break;
        case 3:
          temp = AddrConvert (&rx_buff[6])/2;
          if (temp<5) temp =5;
          if (temp>40) temp = 40;
          power_tic = temp;
          sprintf(tx_buff, "OK\n");
          TransmitString( tx_buff,strlen(tx_buff));
          clear_buff(rx_buff);
           pointer =0;
          break;
        case 4:
          power_tic = 40;
          sprintf(tx_buff, "OK\n");
          TransmitString( tx_buff, strlen(tx_buff));
          clear_buff(rx_buff);
           pointer =0;
          break;
        case 5:
          temp = AddrConvert (&rx_buff[8]);
          result = StartADC(temp);
          sprintf(tx_buff, "ADC=");
          TransmitString( tx_buff, 4);
          sprintf(tx_buff,"%d",result,"/n");
          TransmitString( tx_buff, strlen(tx_buff));
          tx_buff[0] = 0x0D;
          tx_buff[1] = 0x0A;
          TransmitString( tx_buff, 2);
          clear_buff(rx_buff);
           pointer =0;
          break;  
        case 6:
          temp = AddrConvert (&rx_buff[7]);
          PORTA |= (1<<(temp+5));
          sprintf(tx_buff, "OK\n");
          TransmitString( tx_buff, strlen(tx_buff));
          clear_buff(rx_buff);
           pointer =0;
          break;
        case 7:
          temp = AddrConvert (&rx_buff[9]);
          PORTA &=~ (1<<(temp+5));
          sprintf(tx_buff, "OK\n");
          TransmitString( tx_buff, strlen(tx_buff));
          clear_buff(rx_buff);
           pointer =0;
          
          clear_buff(rx_buff);
           pointer =0;
          break;
        case 8:
          sprintf(tx_buff, "INT0=");
          tx_buff[5] = int0_flag;
          TransmitString( tx_buff, 6);
          tx_buff[0] = 0x0D;
          tx_buff[1] = 0x0A;
          TransmitString( tx_buff, 2);
          int0_flag = 0;
          clear_buff(rx_buff);
           pointer =0;
          break;
        
        case 9:
          sprintf(tx_buff, "INT1=");
          tx_buff[5] = int1_flag;
          TransmitString( tx_buff, 6);
          tx_buff[0] = 0x0D;
          tx_buff[1] = 0x0A;
          TransmitString( tx_buff, 2);
          int1_flag = 0;
          clear_buff(rx_buff);
           pointer =0;
          break; 
        case 10:
          sprintf(tx_buff, "CStat=");
          tx_buff[6] = GetCookStatus()|0x30;
          TransmitString( tx_buff, 7);
          tx_buff[0] = 0x0D;
          tx_buff[1] = 0x0A;
          TransmitString( tx_buff, 2);
          int1_flag = 0;
          clear_buff(rx_buff);
           pointer =0;
          break; 
        case 11://SetCP
          t = StrToInt (&rx_buff[7]);
          tempee[0] = t;
          t = StrToInt (&rx_buff[10]);
          timee[0] = t*60;
          t = StrToInt (&rx_buff[13]);
          tempee[1] = t;
          t = StrToInt (&rx_buff[16]);
          timee[1] = t*60;
          InitCoock(tempee, timee, 2);
        //  sprintf(tx_buff, "OK\n");
          TransmitString( tx_buff,strlen(tx_buff));
          clear_buff(rx_buff);
          pointer =0;
           break;
           
         case 12://GetCT
         
          result = StartADC(2);
          result=(int)((float)result/7.5) + 15;
          sprintf(tx_buff, "T=");
          TransmitString( tx_buff, 2);
        //  sprintf(tx_buff,"%d",result,"/n");
          TransmitString( tx_buff, strlen(tx_buff));
          tx_buff[0] = 0x0D;
          tx_buff[1] = 0x0A;
          TransmitString( tx_buff, 2);
          clear_buff(rx_buff);
           pointer =0;
          break;  
        
        default:
         // sprintf(tx_buff, "Invalid command\n");
          TransmitString( tx_buff, strlen(tx_buff));
          clear_buff(rx_buff);
           pointer =0;
          break;
      } 
  
    }
 
    clear_buff(rx_buff);
    clear_buff(tx_buff);
}
