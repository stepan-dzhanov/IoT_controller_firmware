
#ifndef __TIMER__
#define __TIMER__

#define SetControl   ; // Set control pin for driver triac
#define ResetControl ;
#define SetLed PORTC ;
#define ResetLed ;

#define SET_BEP        PORTD |= (1<<6);
#define RESET_BEP      PORTD &=~ (1<<6);

#define START_BEEP     2500

#define RUN_BEEP       5000
#define STOP_BEEP      15000

#define TIME_SLEEP 8 // 75=10 min timeout


void InitTimer();
int GetTimer();

void SetTimer(int tm);
void Beep(int b_time);
#endif