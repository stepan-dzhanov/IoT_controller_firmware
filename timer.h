
#ifndef __TIMER__
#define __TIMER__

#define SetControl   PORTA |= (1<<3); // Set control pin for driver triac
#define ResetControl PORTA &=~ (1<<3);
#define SetLed PORTC |= (1<<4);
#define ResetLed PORTC &=~ (1<<4);

#define SET_BEP        PORTD |= (1<<6);
#define RESET_BEP      PORTD &=~ (1<<6);

#define START_BEEP     2500

#define RUN_BEEP       5000
#define STOP_BEEP      15000


void InitTimer();
int GetTimer();

void SetTimer(int tm);
void Beep(int b_time);
#endif