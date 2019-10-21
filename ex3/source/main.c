/*	Author: yxing024
 *  Partner(s) Name: Hao Wu
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
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

enum states {start, init, add, minus, reset} state;
  
  unsigned char i = 0x00;
  static unsigned char cntC = 0x00;
    
    

void Tick()
{
    unsigned char A0 = ((~PINA) & 0x01);
    unsigned char A1 = ((~PINA) & 0x02);
  switch(state)
    {
    case start:
      state = init; 
      break;
    case init:
      if (A0&&!A1) {state = add;}
      if (!A0&&A1) {state = minus;}
      if (A0&&A1)  {state = reset;}
      if (!A0&&!A1) {state = init;}
      break;
    case add:
      if (A0&&!A1) {state = add;}
      if (!A0&&A1) {state = minus;}
      if (A0&&A1)  {state = reset;}
     
     
      break;
    case minus:
      if (A0&&!A1) {state = add;}
      if (!A0&&A1) {state = minus;}
      if (A0&&A1)  {state = reset;}
     
     
      break;
   case reset:
      if (A0&&!A1) {state = add;}
      if (!A0&&A1) {state = minus;}
      if (A0&&A1)  {state = reset;}
      if (!A0&&!A1) {state = init;}
     
      break;
    default:
      break;
    }
  switch(state)
    {
    case start:
      break;
    case init:
      cntC = 0x07;
      break;
    case add:
      i++;
      if ((cntC < 9)&&(i = 9))
		     {
		       cntC++;
		       i = 0;
		     }
      break;
    case minus:
      i++;
      if ((cntC > 0)&&(i = 9))
		     {
		       cntC--;
		       i = 0;
		     }
      break;
    case reset:
      cntC = 0x00;
      break;
    default:
      break;
    }
  PORTC = cntC;
  return state;
}


void main()
{
  DDRA = 0x00;
  PORTA = 0xff;
  DDRC = 0xff;
  PORTC = 0x00;
 
  cntC = 0x00;
  state = start;

  TimerSet(100);
  TimerOn();
  

 
 
  while(1)
    {
      Tick();
      while (!TimerFlag);
      TimerFlag = 0;
    }
   
  //return 1;
}
