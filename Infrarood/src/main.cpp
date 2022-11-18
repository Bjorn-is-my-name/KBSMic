#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"

void initPins()
{
  // put your setup code here, to run once:
  DDRD |= (1 << PD6); // Set PD6 as output
}

int main(int argc, char const *argv[])
{
  initPins();
  while(1)
  {
    PORTD ^= (1 << PD6);
    _delay_us(26.3);
  }
  return 0;
}
