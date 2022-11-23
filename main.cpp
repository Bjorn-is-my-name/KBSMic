#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Nunchuk.h>

void init_timer0();
void init_timer2();

#define NUNCHUK_ADDRESS 0x52
#define IR_38KHZ 52
#define IR_52KHZ 37
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PLAYER_WIDTH 20
#define PLAYER_HEIGHT 20
#define OFF_TIME 70
#define ZERO_TIME 70
#define ONE_TIME 210

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

uint8_t dataSend;
uint8_t ocr2aVal;

// Toggle IR light
ISR(TIMER2_COMPB_vect)
{
    // Set compare value
    OCR2A = dataSend ? OFF_TIME : ocr2aVal;
    // Toggle between "Toggle OC0A on Compare Match" and "Set OC0A on Compare Match, clear OC0A at BOTTOM", effectively pulling OC2A low on waiting time
    TCCR0A ^= (1 << COM0A1);
    // Flip the action (data sending or waiting)
    dataSend = !dataSend;
}

int main(void) {
    // Player position (x and y flipped because they are flipped on the joystick)
    uint16_t pos[] = {SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2};

    // Setup IR
    DDRD |= (1 << DDD6);
    init_timer0();
    init_timer2();

    // Setup screen
    sei();
    Wire.begin();
    tft.begin();
    tft.fillScreen(ILI9341_BLACK);

    // Check nunckuk connection
    if (!Nunchuk.begin(NUNCHUK_ADDRESS))
        return -1;

    while (1) {
        // Clear old position
        tft.fillRect(pos[0], pos[1], PLAYER_WIDTH, PLAYER_HEIGHT, ILI9341_BLACK);

        // Position change lambda function
        [&pos]()
        {
            // Check nunchuk connection
            if(!Nunchuk.getState(NUNCHUK_ADDRESS))
                return 0;

            // Check for joystick usage

            // Right
            if (Nunchuk.state.joy_x_axis > 140) {
                if (pos[1] + PLAYER_WIDTH < SCREEN_WIDTH)
                    pos[1]++;

                ocr2aVal = ONE_TIME;
            }
            // Left
            else if (Nunchuk.state.joy_x_axis < 100) {
                if (pos[1] > 1)
                    pos[1]--;

                ocr2aVal = ZERO_TIME;
            }
            // No horizontal movement
            else {
                if (pos[1] + PLAYER_WIDTH < SCREEN_WIDTH)
                    pos[1]++;

                ocr2aVal = ONE_TIME;
            }

            // Upwards acceleration
            if (Nunchuk.state.accel_z_axis <= 10 && pos[0] <= SCREEN_HEIGHT - PLAYER_HEIGHT) {
                pos[0]+= 5;
            }
            else if(pos[0] >= 1){
                pos[0]--;
            }
        }();

        // Draw new position
        tft.fillRect(pos[0], pos[1], PLAYER_WIDTH, PLAYER_HEIGHT, ILI9341_WHITE);
    }

    return (0);
}

void init_timer0() 
{
    // Fast-PWM mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << WGM02);

    // Prescale /8
    TCCR0B |= (1 << CS00);

    // Toggle OC0A (PD6) on compare match
    TCCR0A |= (1 << COM0A0);

    // Compare value
    OCR0A = IR_38KHZ;
}

void init_timer2()
{
    // CTC mode
    TCCR2A |= (1 << WGM21);

    // Prescale /128
    TCCR2B |= (1 << CS22) | (1 << CS20);

    dataSend = true;
    ocr2aVal = ONE_TIME;

    // Compare value
    OCR2A = ocr2aVal;

    // Enable interupts on compare match
    TIMSK2 |= (1 << OCIE2B);
}
