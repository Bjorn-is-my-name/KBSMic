#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"


#define THIRTY_EIGHT_KHZ 26.3
#define FIFTY_SIX_KHZ 17.5


/* NOTE
I want to make a function that sends data via ir.
This function will also check what the timing of the receiver is.
The function will change the occra value depending on the data that needs to be send.


Timer1 will toggle the led at the desired frequency.
Timer2 will determine the duty cycle of timer1.

*/

/* Pseudo code for function

void sendData (uint8_t data)
{
  //Start bit ir on for 5000us
  //600us ir off (space)
  
  //To determine what 1 and 0 is, we can bitshift the data and check if the bit is 1 or 0.
  //If the bit is 1, we send 1200us ir on, 600us ir off.
  //If the bit is 0, we send 600us ir on, 600us ir off.

  for (int i = 0; i < 8; i++)
  {
    if (data & (1 << i))
    {
      //Send 1200us ir on, 600us ir off.
      //Turn timer1 on for 1200us
      //Turn timer1 off for 600us

    }
    else
    {
      //Send 600us ir on, 600us ir off.
      //Turn timer1 on for 600us
      //Turn timer1 off for 600us

    }
  }
}
*/

//Timer1 compa interrupt
ISR(TIMER1_COMPA_vect) 
{
  
}

//Functions
void init_timer1() 
{
  //Set up timer with prescaler = 1024 and CTC mode
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
  //Set compare value
  OCR1A = 421;
  //Enable compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}

void init_timer2() 
{
  TCCR2A |= (1 << WGM21) | (1 << WGM20); //Turns on Fast PWM mode.
  TCCR2B |= (1 << CS21); //Turns on timer with a prescaler of 8.
}

void initPins()
{
  DDRD |= (1 << PD6); // Set PD6 as output
}

int main(int argc, char const *argv[])
{
  init_timer1();
  init_timer2();
  initPins();
  
  return 0;
}
