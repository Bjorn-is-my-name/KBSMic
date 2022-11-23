#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Nunchuk.h>

void init_timer0();
void init_timer2();
void sendData();

#define NUNCHUK_ADDRESS 0x52
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

bool dataIsSend = false;

// Toggle IR light
ISR(TIMER2_COMPB_vect)
{
    dataIsSend = !dataIsSend;

    // If data is send, execute the waiting time and pull TC0 low
    if (dataIsSend)
    {
        OCR2A = OFF_TIME;
        TCCR0A |= (1 << COM0A1);
    }
    // If waiting time finished executing, turn TC2 off
    else
        TCCR2B &= ~((1 << CS22) | (1 << CS20));
}

int main(void) {
    DDRD |= (1 << DDD0);
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
            }
            // Left
            else if (Nunchuk.state.joy_x_axis < 100) {
                if (pos[1] > 1)
                    pos[1]--;
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

        // Send the data over IR
        sendData();
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
    OCR0A = 52;
}

void init_timer2()
{
    // CTC mode
    TCCR2A |= (1 << WGM21);

    // Enable interupts on compare match
    TIMSK2 |= (1 << OCIE2B);
}

void sendData()
{
    // If already sending data, wait for it to end
    while((TCCR2B >> CS20) & 1);


    PORTD |= (1 << PORTD0);
    uint8_t data = 0b10101010;

    for (int i = 0; i < 8; i++)
    {
        // Enable TC0 by setting the prescaler (set to /128)
        TCCR2B |= (1 << CS22) | (1 << CS20);
        // Set the bit to send
        OCR2A = ((data >> i) & 1) ? ONE_TIME : ZERO_TIME;
        // Set TC0 to toggle on compare match (stop pulling low)
        TCCR0A &= ~(1 << COM0A1);
    
        // Wait for the sending to finish
        while((TCCR2B >> CS20) & 1);
    }



    PORTD &= ~(1 << PORTD0);
}
