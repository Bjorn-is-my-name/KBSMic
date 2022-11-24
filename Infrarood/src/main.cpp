#include "avr/io.h"
#include "avr/interrupt.h"
#include "util/delay.h"
#include <Nunchuk.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include "Adafruit_ILI9341.h"
#include <Adafruit_STMPE610.h>
#include "Arduino.h"

/*Info
PORTD : PIN 0-7
PORTB : PIN 8-13
PORTC : PIN A0-A5
*/

// Defines
#define THIRTY_EIGHT_KHZ 51
#define FIFTY_SIX_KHZ 34
#define NUNCHUK_ADDRESS 0x52
#define WAIT        0
#define BAUDRATE    9600
#define CHUNKSIZE    32
#define BUFFERLEN    256
// #define THIRTY_EIGHT_KHZ 26.3
// #define FIFTY_SIX_KHZ 17.5

//LCD defines
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000
#define LCD_RD PC0 //Read signal at rising edge.
#define LCD_WR PC1 //Write signal at rising edge.
#define LCD_RS PC2 //D/CX signal. 0 = command, 1 = data.
#define LCD_CS PC3

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//Logic for the recieving of the data
// for(int i = 0; i < 8; i++) {
//         if(OntvangenData)          //output van IR ontvanger
//             Data |= (1 << i);
//     }

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

// what to show
#define STATE
//#define MEM
//#define CAL

// prototypes
bool show_memory(void);

bool move_rect(void);

bool show_calibration(void);

//Interrupts

//Timer1 compa interrupt
ISR(TIMER0_COMPA_vect) 
{ 
}

//Functions
void LCD_write(uint8_t data) 
{
  PORTC &= ~(1 << LCD_WR); //WR low

  PORTD = (PORTD & 0x3) | ((data) & 0xFC);
  PORTB = (PORTB & 0xFC) | ((data) & 0x3);

  PORTC |= (1 << LCD_WR); //WR high
}

void LCD_command_write(uint8_t command)
{
  PORTC &= ~(1 << LCD_RS); //RS low
  LCD_write(command);
}

void LCD_data_write(uint8_t data)
{
  PORTC |= (1 << LCD_RS); //RS high
  LCD_write(data);
}

void sendData(int data, int khz)
{
  if(khz == 38)
  {
    OCR0A = THIRTY_EIGHT_KHZ;
  }
  if(khz == 56)
  {
    OCR0A = FIFTY_SIX_KHZ;
  }
  if(data == 0)
  {
    //Enable timer0 compa interrupt
    TIMSK0 |= (1 << OCIE0A);
    _delay_us(600);
    //Disable timer0 compa interrupt
    TIMSK0 &= ~(1 << OCIE0A);
    _delay_us(600);
  }
  else
  {
    //Enable timer0 compa interrupt
    TIMSK0 |= (1 << OCIE0A);
    _delay_us(1200);
    //Disable timer0 compa interrupt
    TIMSK0 &= ~(1 << OCIE0A);
    _delay_us(600);
  }
}

void init_lcd()
{
  //lcd cs high
  PORTC |= (1 << LCD_CS);
  //lcd write high
  PORTC |= (1 << LCD_WR);
  //lcd read high
  PORTC |= (1 << LCD_RD);
  //lcd cs low
  PORTC &= ~(1 << LCD_CS);
}

void init_timer0() 
{
  //Set timer0 to toggle led at 38kHz
  //Set prescaler to 8
  TCCR0A |= (1 << COM0A0) | (1 << WGM01);
  TCCR0B |= (1 << CS01);
  OCR0A = THIRTY_EIGHT_KHZ;
}

void init_timer2() 
{
  TCCR2A |= (1 << WGM21) | (1 << WGM20); //Turns on Fast PWM mode.
  TCCR2B |= (1 << CS21); //Turns on timer with a prescaler of 8.
  TIMSK2 |= (1 << OCIE2A); //Turns on interrupt for timer2.
}

void initPins()
{
  DDRD |= (1 << PD6); // Set PD6 as output
}


int main(int argc, char const *argv[])
{
  // init_timer0();
  // init_timer2();
  initPins();
  init_lcd();

  LCD_command_write(0x51);
  LCD_data_write(0x0);
  
  // enable global interrupts
  sei();
  // use Serial for printing nunchuk data
  Serial.begin(BAUDRATE);

  // join I2C bus as master
  Wire.begin();

  tft.begin();

  if (!ts.begin()) {
        Serial.println("Couldn't start touchscreen controller");
        while (1);
    }
    Serial.println("Touchscreen started");

    tft.fillScreen(ILI9341_BLUE);

  // handshake with nunchuk
  if (!Nunchuk.begin(NUNCHUK_ADDRESS)) {
      Serial.println("******** No nunchuk found");
      Serial.flush();
      return (1);
  }

  /*
    * get identificaton (nunchuk should be 0xA4200000)
    */
  Serial.print("-------- Nunchuk with Id: ");
  Serial.println(Nunchuk.id);

  while (1) 
  {
    
    // while(Nunchuk.state.joy_x_axis > 140) //Right
    // {
    //   // sendData(1, 38);
    //   Serial.println("Right");
    // }   
    // while(Nunchuk.state.joy_x_axis < 100)//Left
    // {
    //   Serial.println("Left");
    //   // sendData(0, 38);
    // }
    // sendData(1, 38);
    // Serial.print("Working");
  }
  return 0;
}
