#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Nunchuk.h>
#include <spriteTest.c>

void init_timer0();
void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, uint16_t*);

#define NUNCHUK_ADDRESS 0x52
#define IR_38KHZ 52
#define IR_52KHZ 37
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 8
#define OFF_TIME 19
#define START_TIME 190
#define ZERO_TIME 19
#define ONE_TIME 57
#define SENDINGDATA_LEN 8

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Check to see if the current bit is done sending
bool dataIsSend = false;
// Data to send over IR
uint8_t sendingData = 0;
// The data bit to send;
int8_t sendingBit = -2;
uint8_t onTime = 0;

// Toggle IR light
ISR(TIMER0_COMPA_vect)
{
    static uint8_t counter = 0;

    if (++counter > onTime)
    {
        // If the bit is done sending, wait before sending the next bit
        if (dataIsSend)
        {
            // Time after which to continue to the next bit
            onTime = OFF_TIME;
            // Disable TC0
            TCCR0A |= (1 << COM0A1);
        }
        // If the waiting time is passed, send the next bit
        else
        {
            if (++sendingBit < SENDINGDATA_LEN)
            {
                // Send start bit
                if (sendingBit == -1)
                    onTime = START_TIME;
                // Send data
                else
                    // Set the time corresponding to the bit
                    onTime = ((sendingData >> sendingBit) & 1) ? ONE_TIME : ZERO_TIME;

                // Enabe TC0
                TCCR0A &= ~(1 << COM0A1);
            }
            else
            {
                // Once all bits are send, reset for next run
                sendingBit = -2;
            }
        }

        // Flip between sending a bit and waiting
        dataIsSend = !dataIsSend;
        counter = 0;
    }
}

int main(void) {
    // Player position (x and y flipped because they are flipped on the joystick)
    uint16_t pos[] = {SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2};

    // Setup IR
    DDRD |= (1 << DDD6);
    init_timer0();

    // Setup screen
    sei();
    Wire.begin();
    tft.begin();
    tft.setRotation(1);

    // Check nunckuk connection
    while(!Nunchuk.begin(NUNCHUK_ADDRESS))
        tft.fillScreen(ILI9341_RED);

    tft.fillScreen(ILI9341_BLACK);

    while (1) {
        // Clear old position

        // Position change lambda function
        [&pos]()
        {
            // Check nunchuk connection
            if(!Nunchuk.getState(NUNCHUK_ADDRESS))
                return 0;

            // Check for joystick usage

            // Right
            if (Nunchuk.state.joy_x_axis > 140) {
                if (pos[0] + PLAYER_WIDTH < SCREEN_WIDTH)
                    pos[0]++;
            }
            // Left
            else if (Nunchuk.state.joy_x_axis < 100) {
                if (pos[0] > 1)
                    pos[0]--;
            }

            // Upwards acceleration
            if (Nunchuk.state.accel_z_axis <= 10 && pos[1] > 5) {
                pos[1]-= 5;
            }
            else if(pos[1] < SCREEN_HEIGHT - PLAYER_HEIGHT){
                pos[1]++;
            }
        }();

        // Draw new position
        // tft.fillRect(pos[0], pos[1], PLAYER_WIDTH, PLAYER_HEIGHT, ILI9341_WHITE);
        drawSprite(pos[0], pos[1], PLAYER_WIDTH, PLAYER_HEIGHT, spriteTest);

        // Send the data over IR
    }

    return (0);
}

void init_timer0() 
{
    /*
    Fast-PWM mode (TOP = OCRA) -> WGM0[2:0] = 0b111
    Set on compare match and clear on bottom (effectively pulling OC0A low) -> COM0A[1:0] = 0b11
    Prescale /8 -> CS0[2:0] = 0b010
    */
    TCCR0A |= (1 << COM0A0) | (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B |= (1 << WGM02) | (1 << CS01);

    // Compare value
    OCR0A = IR_38KHZ;

    // Enable interupts on compare match
    TIMSK0 |= (1 << OCIE0A);
}

void drawSprite(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* Array){
    tft.startWrite();
        tft.setAddrWindow(x, y, w, h);
        tft.writePixels(Array, w*h);
    tft.endWrite();
}