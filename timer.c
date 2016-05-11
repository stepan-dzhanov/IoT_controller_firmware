

#include <iom16.h>
#include <inavr.h>
#include "timer.h"
#include "name.h"






static  int loc_tic =0;
static int loc_sec =0;
static char b_flg =0;
static int b_set;
static int timeout =0;
static char ready = 0;


void Beep(int b_time){
  
  b_set = b_time;
  b_flg =1;
  SET_BEP 
}


int GetTimer()  {
  return loc_sec;
}

void SetTimer(int tm)  {
  loc_sec = tm;
}



void InitTimer()  {
  TCCR0 = 0X02;  //Divider   1/8
  TIMSK = 0X01; // Enable Timer

}

void OffTimer (){
   TIMSK &=~(1<<TOIE0); // Enable Timer
}
void OnTimer ()  {
   TIMSK |= (1<<TOIE0); // Enable Timer
}

#pragma vector = TIMER0_OVF_vect  
__interrupt void TIMER0_OVF(void) {
  TCNT0=144;
  

  loc_tic++;
  if (loc_tic>=4408)  {
    loc_tic = 0;
    if(!(loc_sec==0))loc_sec--;
  }
  if (b_flg)  {
    b_set--;
    if(b_set == 0){
      RESET_BEP
      b_flg =0;
    }
  }
    
  
} 


#pragma vector = TIMER2_OVF_vect 
__interrupt void TIMER2_OVF_vectINT(void)
{
   timeout++;
   if (timeout>=TIME_SLEEP) {
     timeout =0;
     ready = 1;
   }
}

char CheckSleepTimeout()  {
  return ready;
}
void ResetSleepTimeout()  {
  ready = 0;
  timeout = 0;
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
    TIMSK  |= (1<<TOIE0);
}