#include"uart.h" 
#include <iom16.h>
#include <inavr.h>
#include"protocol.h" 
#define EnableTransmit  PORTD |= (1<<5);
#define DisableTransmit PORTD &=~ (1<<5);

void USART_Init( unsigned int baud )  {
  /* Set baud rate */
  UBRRH = (unsigned char)(baud>>8);
  UBRRL = (unsigned char)baud;
  /* Enable receiver and transmitter */
  UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
  UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_Transmit( char data) {
  
  while (!( UCSRA & (1<<UDRE)));
  UDR = data;
   __delay_cycles(300);
  
}


void TransmitString( char *string, char len)  {

   unsigned char k;
   
   
   EnableTransmit
  __delay_cycles(3000);
 
  
   for (k=0; k<len; k++)  {
    USART_Transmit(string[k]);
  }
  __delay_cycles(30000);
  DisableTransmit
}
