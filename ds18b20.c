


#include <iom16.h>
#include <inavr.h>

#include "ds18b20.h"
#include "uart.h"

#define PIN_TO_OUT     DDRC=0x10;// PC5 
#define PIN_TO_IN      DDRC=0x00;// PC5 
#define SET_PIN_DRV    PORTC |= (1<<4);
#define RESET_PIN_DRV  PORTC &=~ (1<<4);

#define TIC 500

void delay_us (int timeout)  {
  int i;
  
  for (i=0;i<timeout;i++){ 
  ;
  }
}

void testPin()  {
  
  PIN_TO_OUT
    while(1){
  SET_PIN_DRV
  delay_us (1);
  RESET_PIN_DRV
  delay_us (1);
    }
  PIN_TO_IN
   
  
}

char InitDS18B20(){
  char a;
  PIN_TO_OUT
  RESET_PIN_DRV
  __delay_cycles(1920);//500
  PIN_TO_IN
  __delay_cycles(280);;//65
  a = PINC&0x10;
  
  __delay_cycles(1640);;//65
  if (a == 0x10) return 1;
  else return 0;
  
  
}

void TxDS18B20(unsigned char cmd){
 
unsigned char temp = 0;
unsigned char i = 0;
temp = cmd;


for (i=0;i<8;i++) {
                  if (temp&0x01) {
                                 PIN_TO_OUT
                                 RESET_PIN_DRV;              // 1
                                 __delay_cycles(24);;//5
                                 PIN_TO_IN;
                                 __delay_cycles(256);;//70
                                 } 
                    else               {                //0
                                        PIN_TO_OUT
                                        RESET_PIN_DRV;
                                        __delay_cycles(240);//70
                                        PIN_TO_IN;
                                        __delay_cycles(40);//5
                                        }
                                 temp >>= 1;
                 }
 PIN_TO_OUT
 SET_PIN_DRV
 __delay_cycles(10);//5
}

unsigned char RxDS18B20() {
 
unsigned char d = 0;
unsigned char i = 0;
for (i=0;i<8;i++){
                 PIN_TO_OUT
                 RESET_PIN_DRV;                //прижимаем линию
                 __delay_cycles(24);//2
                 PIN_TO_IN
                 __delay_cycles(36);//7
                 d>>=1;                     //освобождаем место под новый бит
                 if ((PINC&0x10) == 0x10) d |= 0x80;   //если 1 то записываем 1
                 __delay_cycles(220);            //70  ждем до положенного времени
                 }
PIN_TO_OUT
 SET_PIN_DRV
 __delay_cycles(10);//5
return d;
}

signed char GetTempDS18B20() {
  unsigned char temp1, temp2, sign, temp_drob,x;
  signed char temperature;
  char tx_buff1[64];
  int i;
#ifndef _DEBUG
  InitDS18B20();
  TxDS18B20(0xCC);
  
  
  TxDS18B20(0x44);
  for(i=0;i<1000;i++) __delay_cycles(4000);
  
  
  InitDS18B20();
  TxDS18B20(0xCC);
  

  TxDS18B20(0xBE);
#endif
  temp1 = RxDS18B20();    //читаем младший байт
  temp2 = RxDS18B20();    //читаем старший байт
  
#ifdef _DEBUG
  temp1 = 0x5E;    //читаем младший байт
  temp2 = 0xFF;
  
#endif
  
  //USART_Transmit( temp1);
  // USART_Transmit( temp2);
 // x = RxDS18B20();
 // x = RxDS18B20();
//  x = RxDS18B20();
 // x = RxDS18B20();
//  x = RxDS18B20();
 // x = RxDS18B20();
 // x = RxDS18B20();
  
  
 
 // temp_drob = temp1 & 0xFF;           //Записываем дробную часть в отдельную переменную
  //temp_drob = ((temp_drob*6)+2)/10;         //Переводим в нужное дробное число
  temp1 = temp1>>4;
  temp1 = temp1&0x0F;
  sign = temp2 & 0x80;                      //определяем знак температуры
  temp2 = temp2<<4;
  temp2 &= 0x70;
  temp2 |= temp1;                           //помещаем все в одну переменную
  temperature = temp2;
 
   //USART_Transmit( temp2);
if (sign) {                               //если минус
            temperature = 127-temp2;      //глобальная переменная
            temperature = -1*temperature;
       //     temp_drob = 10 - temp_drob;   //глобальная переменная
           }   else temperature = temp2;

__delay_cycles(10000);

 
return temperature ;
}
  
  
  
