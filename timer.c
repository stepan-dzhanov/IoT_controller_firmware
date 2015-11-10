

#include <iom16.h>
#include <inavr.h>
#include "timer.h"
#include "name.h"

extern int position;
extern unsigned char time_pulse;
extern unsigned int  time_phase;
extern unsigned char time_tic;
extern unsigned int  volatile time_set;
extern unsigned char power_tic;
extern unsigned char host_power;
extern char flag_null;
extern char flag_control;
extern char flag_ac;





static  int loc_tic =0;
static int loc_sec =0;
static char b_flg =0;
static int b_set;

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

#pragma vector = TIMER0_OVF_vect  
__interrupt void TIMER0_OVF(void) {
  TCNT0=144;
  
  time_phase++;
  time_tic++;
  time_pulse++;
  time_set++;
  loc_tic++;
  if (loc_tic>=408)  {
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
//**************************Control Phase*******************************
  if (time_phase>=CONTROL_PHASE_TIME)  {
    time_phase=0;
    if ((flag_null==0)&&(time_tic>+PERIOD_TIME))  {
      flag_ac=0;
    }
  }
  if (flag_ac==0) ResetLed;
  if (flag_ac==1) SetLed;
//*************************End control phase****************************  
  if (flag_null==0)  return;
  
  if ((flag_control==1)&&(time_pulse==PULSE_TIME))  {
    ResetControl
    flag_control=0;
  }
  
  if ((flag_null==1)&&(time_tic>=(2*PERIOD_TIME+2)))  {
    flag_null=0;
  }
  
  
  if ( (flag_null==1)&&( (time_tic==power_tic)||(time_tic==power_tic+PERIOD_TIME) ) )  {
      time_pulse=0;
      if (power_tic<MAX_POWER_TIC) SetControl
      flag_control=1;
      flag_ac=1;
  }   
} 