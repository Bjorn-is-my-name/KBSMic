#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Nunchuk.h>
#include "HardwareSerial.h"
void init_IR();
void init_timer0();
void init_timer2();

#define NUNCHUK_ADDRESS 0x52
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

ISR(TIMER0_COMPA_vect)
{
    //IR-LED gaat hier aan en uit. geen code nodig.

}

ISR(TIMER2_COMPA_vect)
{
    //Toggle iets
    TCCR0A ^= (1 << COM0A1);
}


int main(void) {
    uint16_t pos[] = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};

    init_IR();
    init_timer0();
    init_timer2();

    sei();
    Wire.begin();

    tft.begin();
    tft.fillScreen(ILI9341_BLACK);

    if (!Nunchuk.begin(NUNCHUK_ADDRESS))
        return -1;

    while (1) {
        [&pos]()
        {
            if(!Nunchuk.getState(NUNCHUK_ADDRESS)){
                return 0;
            }

            if (Nunchuk.state.joy_x_axis > 140 && pos[1] <= SCREEN_WIDTH-20) {
                pos[1]++;
            }
            if (Nunchuk.state.joy_x_axis < 100 && pos[1] >= 1) {
                pos[1]--;
            }

            if (Nunchuk.state.accel_z_axis <= 10 && pos[0] <= SCREEN_HEIGHT-20) {
                pos[0]+=5;
            }else if(pos[0] >= 1){
                pos[0]--;
            }
//             if (Nunchuk.state.joy_y_axis < 100 && pos[1] >= 0) {
//                 pos[1]--;
//             }
        }();
        int16_t W = 20;
        int16_t H = 20;
        // Compare      Y       X       +Y   +X
        tft.fillRect(pos[0]-1, pos[1]-1, H+2, W+2, ILI9341_BLACK);
        tft.fillRect(pos[0], pos[1], W, H, ILI9341_WHITE);
    }

    return (0);
}

void init_IR()
{
    DDRD |= (1 << PD6);                         //Pin D6 as output (IR-LED)
}

void init_timer0()
{
    TCCR0A |= (1 << WGM00) | (1 << WGM01);      //fast-PWM + CTC
    TCCR0B |= (1 << WGM02);                     //^^^^^^^^^^^^^^
    TCCR0B |= (1 << CS01);                      //prescaler 8
    OCR0A = 52;                                 //compare value = 52 = 38kHZ  35 = 56kHZ
    TIMSK0 |= (1 << OCIE0A);                    //enable interrupts
    TCCR0A |= (1 << COM0A0);                    //enables toggle on OC0A
}

void init_timer2()
{
    TCCR2A |= (1 << WGM21);                     //CTC
    TCCR2B |= (1 << CS22);                      //64 prescale
    //1 is aan voor x us
    //0 is aan voor x us
    OCR2A = 200;                                //Comp waarde = 100
    TIMSK2 |= (1 << OCIE2A);                    //Enable interupts
}