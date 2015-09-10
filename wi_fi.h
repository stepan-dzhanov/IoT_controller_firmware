#ifndef WIFI
#define WIFI


#define ON_WIFI    PORTB |= (1<<0);  
#define OFF_WIFI   PORTB &=~ (1<<0);


void SendToServer(int data1, int data2);




#endif