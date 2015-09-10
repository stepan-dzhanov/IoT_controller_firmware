
#include <iom16.h>
#include <inavr.h>
#include <stdio.h>
#include <string.h> 
#include "name.h"
#include "uart.h"
#include "timer.h"
#include "wi_fi.h"





void SendToServer(int data1, int data2) {
  
 
  
char tx_buff [64];
char tx_buff1 [64];
char tx_buff2 [64];
char counter;
char i;
  
  if (!data1) return;
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
  
  sprintf(tx_buff, "GET /update?api_key=1CV2GX9SLOJGA16D&field4=");//28\r\n\r\n");
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
  
  
  if (!data2) return;
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
  
  sprintf(tx_buff2,"%d",data2,"/n");
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
  
  sprintf(tx_buff, "GET /update?api_key=1CV2GX9SLOJGA16D&field5=");//28\r\n\r\n");
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