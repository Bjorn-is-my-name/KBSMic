#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Nunchuk.h>
#include <spriteTest.c>

void init_timer0();
void setFreq(uint8_t);
void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, uint16_t*);

#define TFT_CS 10
#define TFT_DC 9
#define NUNCHUK_ADDRESS 0x52
#define IR_38KHZ 52
#define IR_56KHZ 35
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16
#define SENDINGDATA_LEN 9

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Player position (x and y flipped because they are flipped on the joystick)
uint16_t pos[] = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT};
uint16_t pos2[] = {SCREEN_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT};

// Check to see if the current bit is done sending
bool dataIsSend = false;
// Data to send over IR
uint16_t sendingData = 0;
// The data bit to send;
int8_t sendingBit = -2;
uint16_t onTime = 0;
unsigned long currentMs = 0;

uint8_t msTime;
uint16_t startTime;
uint8_t zeroTime;
uint8_t oneTime;
uint8_t offTime;
uint16_t stopTime;

unsigned long startMs = 0;
bool startBitReceived = false;
uint16_t receivedData = 0;
uint8_t bitCounter = 0;
bool isDataBit = false;

ISR(PCINT2_vect)
{
    uint8_t difference = currentMs - startMs;

    if (difference > 4 && difference < 6)
        startBitReceived = true;
    else if (difference > 5)
    {
        startBitReceived = false;
        bitCounter = 0;
    }

    if (startBitReceived && isDataBit)
    {
        if (difference < 2)
            receivedData &= ~(1 << bitCounter++);
        else if (difference > 1 && difference < 4)
            receivedData |= (1 << bitCounter++);
    }

    if (bitCounter == SENDINGDATA_LEN)
        pos2[0] = receivedData;

    isDataBit = !isDataBit;
    startMs = currentMs;
}

// Toggle IR light
ISR(TIMER0_COMPA_vect)
{
    static uint16_t counter = 0;
    static uint8_t msCounter = 0;

    if(++msCounter >= msTime){
        currentMs++;
        msCounter = 0;
    }

    if (++counter > onTime)
    {
        // If the bit is done sending, wait before sending the next bit
        if (dataIsSend)
        {
            // Time after which to continue to the next bit
            onTime = offTime;
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
                    onTime = startTime;
                // Send data
                else
                    // Set the time corresponding to the bit
                    onTime = ((sendingData >> sendingBit) & 1) ? oneTime : zeroTime;

                // Enabe TC0
                TCCR0A &= ~(1 << COM0A1);
            }
            else if (sendingBit == SENDINGDATA_LEN)
            {
                onTime = stopTime;
            }
            else
            {
                // Once all bits are send, reset for next run
                sendingBit = -2;
                sendingData = pos[0];
            }
        }

        // Flip between sending a bit and waiting
        dataIsSend = !dataIsSend;
        counter = 0;
    }
}

int main(void) {
    // Setup IR sending
    DDRD |= (1 << DDD6);
    init_timer0();

    // Setup IR recieving
    PORTD |= (1 << PORTD2); //pull up
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);

    setFreq(IR_38KHZ);

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

        // Draw black line over old players
        tft.fillRect(0, SCREEN_HEIGHT - PLAYER_HEIGHT, SCREEN_WIDTH, PLAYER_HEIGHT, ILI9341_BLACK);
        // Draw yourself
        drawSprite(pos[0], pos[1], PLAYER_WIDTH, PLAYER_HEIGHT, spriteTest1);
        // Draw the other player
        drawSprite(pos2[0], pos2[1], PLAYER_WIDTH, PLAYER_HEIGHT, spriteTest2);

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

    // Enable interupts on compare match
    TIMSK0 |= (1 << OCIE0A);
}

void setFreq(uint8_t freq)
{
    OCR0A = freq;

    if (freq == IR_38KHZ)
    {
        msTime = 38;
        startTime = 190;
        zeroTime = 38;
        oneTime = 114;
        offTime = 76;
        stopTime = 950;
    }
    else
    {
        msTime = 56;
        startTime = 280;
        zeroTime = 56;
        oneTime = 168;
        offTime = 112;
        stopTime = 1400;
    }
}

void drawSprite(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t* Array){
    tft.startWrite();
        tft.setAddrWindow(x, y, w, h);
        tft.writePixels(Array, w*h);
    tft.endWrite();
}
