/*	Author: yxing024
 *  Partner(s) Name: Hao Wu
 *	Lab Section:6
 *	Assignment: Lab #  Exercise1 #
 *	Exercise Description: [lit!]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;



void TimerOn()
{
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}

void TimerOff()
{
  TCCR1B = 0x00;
}

void TimerISR()
{
  TimerFlag = 1;
   unsigned char  tmp = 0x00;
      unsigned char i = 0;
      tmp = (1<<i);
      PORTC = tmp;
      if( i < 3 ){i++;}
      else { i = 0;}
}

ISR(TIMER1_COMPA_vect)
{
  _avr_timer_cntcurr--;
  if (_avr_timer_cntcurr == 0)
    {
      TimerISR();
      _avr_timer_cntcurr = _avr_timer_M;
    }
}

void TimerSet(unsigned long M)
{
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}

/*enum states { on} state;


Tick_blink()
{

  unsigned char i = 0x00;

  unsigned char tmp = 0x00;

  switch(state)
    {
    case start:
      state = init;
      break;
    case init:
      state = on;
     
      break;
    case on:
      state = on;
      i++;
      break;
    default:
      state = start;
      break;
    }
  switch(state)
    {
    case init:
      i = 0;
      break;
    case on:
      if (i == 3){i = 0;}
      tmp = (1 << i);
      break;

    default:
      break;
    }
  PORTC = tmp;
  return state;
}

*/
int main(void)
{
    
  DDRC = 0xFF;
  PORTC = 0x00;
 
  TimerSet(1000);
  TimerOn();
  
 
 
  while(1)
    {
      //Tick_blink();
   
    
      while (!TimerFlag);
      TimerFlag = 0;
    }
   
    return 1;
}
