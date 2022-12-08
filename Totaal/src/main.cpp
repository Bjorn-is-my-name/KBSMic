#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
//#include <Adafruit_ILI9341.h>
#include "LCD.cpp"
#include <Nunchuk.h>
#include "Player1.c"
#include "Player2.c"
#include "Background.c"
#include <HardwareSerial.h>

/*Info Arduino pins
PORTD : PIN 0-7
PORTB : PIN 8-13
PORTC : PIN A0-A5
*/

//Prototype functions
bool checkCollision(uint8_t xRect1, uint8_t yRect1, uint8_t wRect1, uint8_t hRect1, uint8_t xRect2, uint8_t yRect2, uint8_t hRect2, uint8_t wRect2);

void init_timer0();

void setFreq(uint8_t);

void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *);

void drawBackground();

void buffer(uint16_t, uint8_t, uint16_t *, uint16_t *);

uint16_t getColor(uint8_t);

void update();

void draw();

void drawWall(uint8_t, uint8_t, uint8_t, uint8_t);

//Defines
#define NUNCHUK_ADDRESS 0x52

#define IR_38KHZ 52
#define IR_56KHZ 35

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 20

#define SENDINGDATA_LEN 9
#define SENDINGBIT_START_VALUE - 2

#define STARTBIT_VALUE -1
#define STARTBIT_MIN 3
#define STARTBIT_MAX 6

#define ZERO_MAX 2
#define ONE_MIN 1
#define ONE_MAX 4

#define INITIAL_Y_VEL 10
#define FRAME_TIME 32 //(1000/30FPS)-1 == 32

#define BG_SPRITE_AMOUNT 192
#define BG_SPRITE_SIZE 20

#define WALL_WIDTH 40
#define WALL_HEIGHT 40
#define WALL_X 100
#define WALL_Y 140 //240 - WALL_HEIGHT

#define GRAVITY 1

struct {
public:
    uint16_t x = 0;
    uint8_t y = SCREEN_HEIGHT - PLAYER_HEIGHT;
    uint16_t xOld = 0;
    uint8_t yOld = SCREEN_HEIGHT - PLAYER_HEIGHT;
    int8_t yVelocity = 0;
    bool jumping = false;
} player1;

struct {
public:
    uint16_t x = 0;
    uint8_t y = SCREEN_HEIGHT - PLAYER_HEIGHT;
    uint16_t xOld = 0;
    uint8_t yOld = SCREEN_HEIGHT - PLAYER_HEIGHT;
//    uint8_t animation
} player2;

// Check to see if the current bit is done sending
bool dataIsSend = false;
// Data to send over IR
uint16_t sendingData = 0;
// The data bit to send;
int8_t sendingBit = SENDINGBIT_START_VALUE;
uint16_t onTime = 0;
volatile unsigned long currentMs = 0;
volatile unsigned char intCurrentMs = 0;

uint8_t msTime;
uint16_t startTime;
uint8_t zeroTime;
uint8_t oneTime;
uint8_t offTime;

unsigned long startMs = 0;
bool startBitReceived = false;
uint16_t receivedData = 0;
uint8_t bitCounter = 0;
bool isDataBit = false;

enum gameState{MENU, GAME, LEVELSELECT};
gameState currentGameState = GAME;

ISR(PCINT2_vect) {
    isDataBit = ((PIND >> PIND2) & 1) != 0; // Check for the pin state (high or low)

    if (isDataBit) {
        uint8_t difference = currentMs - startMs; // Calculate the length to determin the value

        if (difference > STARTBIT_MIN && difference < STARTBIT_MAX) // Check if its a start bit
        {
            startBitReceived = true;
            bitCounter = 0;
        }

        if (startBitReceived) // If the start bit has been send, check what the data is
        {
            if (difference < ZERO_MAX) // Check if its a zero
                receivedData &= ~(1 << bitCounter++);
            else if (difference > ONE_MIN && difference < ONE_MAX) // Check if its a one
                receivedData |= (1 << bitCounter++);

            if (bitCounter == SENDINGDATA_LEN) // If all bits are send, save the value in the variable
            {
                startBitReceived = false;
                player2.x = receivedData;
            }
        }
    }

    startMs = currentMs; // Save the time from startup to now
}

ISR(TIMER0_COMPA_vect) // Toggle IR light
{
    static uint16_t counter = 0;
    static uint8_t msCounter = 0;

    if (++msCounter > msTime) {
        currentMs++;
        intCurrentMs++;
        msCounter = 0;
    }

    if (++counter > onTime) {
        if (dataIsSend) // If the bit is done sending, wait before sending the next bit
        {
            onTime = offTime;        // Time after which to continue to the next bit
            TCCR0A |= (1 << COM0A1); // Disable TC0
        } else // If the waiting time is passed, send the next bit
        {
            if (sendingBit++ < SENDINGDATA_LEN) {
                if (sendingBit == STARTBIT_VALUE) // Send start bit
                    onTime = startTime;
                else                                                                 // Send data
                    onTime = ((sendingData >> sendingBit) & 1) ? oneTime
                                                               : zeroTime; // Set the time corresponding to the bit

                TCCR0A &= ~(1 << COM0A1); // Enable TC0
            } else {
                sendingBit = SENDINGBIT_START_VALUE; // Once all bits are send, reset for next run
                sendingData = player2.x;
            }
        }

        dataIsSend = !dataIsSend; // Flip between sending a bit and waiting
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
    Wire.begin();
    setupSPI();
    sei();
    START_UP();
    Serial.begin(9600);

    // Check nunchuk connection
    while (!Nunchuk.begin(NUNCHUK_ADDRESS)) {
        fillRect(0, 0, 320, 240, ILI9341_RED);
    }

    drawBackground();
    fillRect(WALL_X, WALL_Y, WALL_WIDTH, WALL_HEIGHT, ILI9341_CYAN);
    volatile int frameCounter = 0; //#TODO reset deze ergens en hem verplaatsen

    while (1) {
        if (intCurrentMs > FRAME_TIME) 
        { 
            //30 FPS
            intCurrentMs = 0;
            frameCounter++;
            if(currentGameState == GAME)
            {
                //Game code
                update();
                draw();
                
            }
        }
    }
    return (0);
}
void init_timer0() {
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

void setFreq(uint8_t freq) {
    OCR0A = freq;

    if (freq == IR_38KHZ) {
        msTime = 37;
        startTime = 189;
        zeroTime = 37;
        oneTime = 113;
        offTime = 37;
    } else {
        msTime = 55;
        startTime = 279;
        zeroTime = 55;
        oneTime = 167;
        offTime = 55;
    }
}


void update() {
    player1.xOld = player1.x;
    player1.yOld = player1.y;
    player1.y += player1.yVelocity;

    //Checks if the player is not already at the bottom of the screen.
    if(player1.y + PLAYER_HEIGHT + player1.yVelocity <= SCREEN_HEIGHT)
    {
        player1.yVelocity += GRAVITY;
    }
    else
    {
        player1.yVelocity = 0;
        player1.jumping = false;
    }
    // Get the nunchuk input data
    if (!Nunchuk.getState(NUNCHUK_ADDRESS))
    {
        return;
    }
    

    // Check for movement to right (only move when not against the wall)
    if (Nunchuk.state.joy_x_axis > 140 && player1.x + PLAYER_WIDTH < SCREEN_WIDTH)
        player1.x += 2;

    // Check for movement to left (only move when not against the wall)
    else if (Nunchuk.state.joy_x_axis < 100 && player1.x > 0)
        player1.x -= 2;


    //Top collision detection.
    if(player1.y + PLAYER_HEIGHT <= WALL_Y && player1.y + PLAYER_HEIGHT + player1.yVelocity >= WALL_Y && player1.x + PLAYER_WIDTH >= WALL_X && player1.x <= WALL_X + WALL_WIDTH)
    {
        player1.yVelocity = 0;
        player1.jumping = false;

    }
    
    //Bottom collision detection.
    if(player1.y <= WALL_Y + WALL_HEIGHT && player1.x + PLAYER_WIDTH >= WALL_X && player1.x <= WALL_X + WALL_WIDTH)
    {
        player1.yVelocity = 1;
        player1.y = WALL_Y + WALL_HEIGHT;
    }

    if(WALL_Y + WALL_HEIGHT > player1.y) //Checking if player is not underneath the wall.
    {
        //Right collision detection.
        if(player1.x + PLAYER_WIDTH >= WALL_X && player1.y + PLAYER_HEIGHT > WALL_Y && player1.x <= WALL_X + WALL_WIDTH / 2)
        {
            player1.x = WALL_X - PLAYER_WIDTH;
        }

        //Left collision detection.
        if(player1.x <= WALL_X + WALL_WIDTH && player1.y + PLAYER_HEIGHT > WALL_Y && player1.x >= WALL_X + WALL_WIDTH / 2)
        {
            player1.x = WALL_X + WALL_WIDTH;
        }
    }     

    

    //Jumping and falling mechanics
    if(Nunchuk.state.c_button == 1 && !player1.jumping)
    {
        player1.jumping = true;
        player1.yVelocity -= INITIAL_Y_VEL;
    }
}

void draw() {
    drawSprite(player1.x, player1.y, 16, 20, Player1);
    drawSprite(player2.x, player2.y, 16, 20, Player2);
}

void drawSprite(uint16_t x, uint8_t y, const uint8_t w, const uint8_t h, const uint8_t *Sprite) {
    for (uint16_t PixGroup = 0; PixGroup < (w / 2) * h; PixGroup++) {
        if (PixGroup % (w / 2) == 0 && PixGroup != 0) {
            x -= w;
            y++;
        }
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++) {
            if (Pixel == 0) {
                if (getColor((Sprite[PixGroup] & 0xF0) >> 4) != 255) {
                    drawPixel(x, y, getColor((Sprite[PixGroup] & 0xF0) >> 4));
                }
            } else {
                if (getColor((Sprite[PixGroup] & 0x0F)) != 255) {
                    drawPixel(x, y, getColor(Sprite[PixGroup] & 0x0F));
                }
            }
            x++;
        }
    }
}

void drawBackground() {
    uint16_t x = 0;
    uint8_t y = 0;
    for (uint8_t i = 0; i < BG_SPRITE_AMOUNT; i++) {
        drawSprite(x, y, BG_SPRITE_SIZE, BG_SPRITE_SIZE, Background);
        if (x >= SCREEN_WIDTH - BG_SPRITE_SIZE) {
            x = 0;
            y += BG_SPRITE_SIZE;
        } else
            x += BG_SPRITE_SIZE;
    }
}

void buffer(/*OldPos, NewPos, UsedSprite*/) {
    //fix dan jan ❤️
}

uint16_t getColor(uint8_t Color) {
    switch (Color) {
        case 0:
            return ILI9341_RED;
        case 1:
            return ILI9341_BLACK;
        case 2:
            return ILI9341_GREEN;
        case 3:
            return ILI9341_OLIVE;
        case 4:
            return ILI9341_ORANGE;
        case 5:
            return ILI9341_YELLOW;
        case 6:
            return ILI9341_LIGHTGREY;
        case 7:
            return ILI9341_DARKGREY;
        case 8:
            return ILI9341_BLUE;
        case 9:
            return ILI9341_CYAN;
        case 10:
            return ILI9341_WHITE;
        case 11:
            return ILI9341_BACKGROUND_DARK;
        case 12:
            return ILI9341_BACKGROUND_LIGHT;
        case 13:
            // return ILI9341_---;
            break;
        case 14:
            // return ILI9341_---;
            break;
        case 15:
            return 255;
    };
    return 0;
}

// bool checkCollision(uint8_t xRect1, uint8_t yRect1, uint8_t wRect1, uint8_t hRect1, uint8_t xRect2, uint8_t yRect2, uint8_t hRect2, uint8_t wRect2)
// {
//     return (xRect1 + wRect1 >= xRect2 && yRect1 > yRect2) /* Checks if right side (1) collides with left side (2)*/ 
//     || (xRect1 <= xRect2 + wRect2 && yRect1 > yRect2) /* Checks if left side (1) collides with right side (2) */
//     || (yRect1 + hRect1 >= yRect2 && xRect1 >= xRect2 || xRect1 + wRect1 <= xRect2 + wRect2)/* Checks if bottom side (1) collides with top side (2)  and if the rectangle is still on top of the other rectangle*/ 
//     || (yRect1 <= yRect2 + hRect2 && xRect1 >= xRect2 || xRect1 + wRect1 <= xRect2 + wRect2); /* Checks if top side (1) collides with bottom side (2) and if the rectangle is still on top of the other rectangle */
// }
