#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include "LCD.cpp"
#include "Player1.c"
#include "Player2.c"


#include "EEPROM.c"
#include "Adafruit_STMPE610.h"
#include "Adafruit_STMPE610.cpp"

#include "Button.c"
#include "WaterBlue.c"
#include "DoorCorner.c"
#include "DoorEdge.c"
#include "SignBlue.c"
#include "SignRed.c"
#include "PlatformEdge.c"
#include "PlatformMiddle.c"
#include "LeverBase.c"
#include "LeverTop.c"
#include "DiaBlue.c"
#include "DiaRed.c"

#include "Background.c"
#include "Nunchuk.cpp"

void init_timer0();

void checkWallCollision();

void setFreq(uint8_t);

void checkGameState();

void drawMenu();

void drawSettings();

void drawPlayerSelectScreen();

void drawLevelSelectScreen();

void drawBackgroundTile(uint16_t, uint8_t, uint8_t, uint8_t);

void clearSprite(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *);

bool pointInRect(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);

void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t ver = 0);

void drawSpriteMirror(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t ver = 0);

void drawBackground();

void drawInteractables();


uint16_t getColor(uint8_t, uint8_t ver = 0);

void update();

void draw();

#define NUNCHUK_ADDRESS 0x52

#define IR_38KHZ 52
#define IR_56KHZ 35

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define PLAYER_WIDTH 8
#define PLAYER_ACTUAL_WIDTH 16
#define PLAYER_HEIGHT 20

#define SENDINGDATA_LEN 16 // bits - 1
#define SENDINGBIT_START_VALUE (- 2)

#define STARTBIT_VALUE (-1)
#define STARTBIT_MIN 3
#define STARTBIT_MAX 6
#define ZERO_MAX 2
#define ONE_MIN 1
#define ONE_MAX 4

#define INITIAL_Y_VEL 10
#define FRAME_TIME 32 //(1000/30FPS)-1 = 32

#define BG_SPRITE_AMOUNT 192
#define BG_SPRITE_WIDTH 10
#define BG_SPRITE_ACTUAL_WIDTH 20
#define BG_SPRITE_HEIGHT 10

#define GRAVITY 1

#define BUTTON_WIDTH 3
#define BUTTON_HEIGHT 4
#define WATER_WIDTH 10
#define WATER_HEIGHT 5
#define DOOR_WIDTH 5
#define DOOR_HEIGHT 10
#define SIGN_WIDTH 2
#define SIGN_HEIGHT 13
#define PLATFORM_WIDTH 5
#define PLATFORM_HEIGHT 8
#define DIA_WIDTH 3
#define DIA_HEIGHT 9
#define LEVER_BASE_WIDTH 4
#define LEVER_BASE_HEIGHT 6
#define LEVER_TOP_WIDTH 4
#define LEVER_TOP_HEIGHT 7

#define PLAY_BUTTON_MIN_X 1384
#define PLAY_BUTTON_MAX_X 2707

#define PLAY_BUTTON_MIN_Y 700
#define PLAY_BUTTON_MAX_Y 1270


struct
{
public:
    uint16_t x = 13;
    uint8_t y = 170;
    uint16_t xOld = x;
    uint8_t yOld = y;
    int8_t yVelocity = 0;
    bool jumping = false;
} player1;

struct
{
public:
    uint16_t x = 13;
    uint8_t y = 210;
    uint16_t xOld = x;
    uint8_t yOld = y;
//    uint8_t animation
} player2;

struct Rect
{
    uint16_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
};

struct Interactables
{
    uint16_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
};

Rect walls[] = {
        Rect{0, 0, 5, 240},
        Rect{10, 0, 155, 10},
        Rect{310, 10, 5, 230},
        Rect{10, 230, 155, 10},
        Rect{10, 40, 20, 40},
        Rect{50, 70, 115, 10},
        Rect{153, 50, 30, 20},
        Rect{40, 110, 75, 10},
        Rect{180, 120, 50, 10},
        Rect{280, 120, 15, 20},
        Rect{290, 140, 10, 10},
        Rect{10, 150, 70, 10},
        Rect{150, 150, 5, 30},
        Rect{150, 180, 52, 10},
        Rect{280, 210, 15, 20},
        Rect{10, 190, 40, 10}
};

Interactables Interactable[] = {
        Interactables{12, 12, DIA_WIDTH * 2, DIA_HEIGHT},
        Interactables{164, 216, DIA_WIDTH * 2, DIA_HEIGHT},
        Interactables{26, 12, DIA_WIDTH * 2, DIA_HEIGHT},
        Interactables{229, 216, DIA_WIDTH * 2, DIA_HEIGHT}
};

Adafruit_STMPE610 touch = Adafruit_STMPE610(8);


// Check to see if the current bit is done sending
bool dataIsSend = false;
// Data to send over IR
uint32_t sendingData = 0;
// The data bit to send;
int8_t sendingBit = SENDINGBIT_START_VALUE;
uint16_t onTime = 0;
volatile uint32_t currentMs = 0;
volatile uint8_t intCurrentMs = 0;

uint8_t msTime;
uint16_t startTime;
uint8_t zeroTime;
uint8_t oneTime;
uint8_t offTime;

uint32_t startMs = 0;
bool startBitReceived = false;
uint32_t receivedData = 0;
uint8_t bitCounter = 0;
bool isDataBit = false;

enum gameState
{
    MENU, GAME, LEVELSELECT, SETTINGS, PAUSE, GAMEOVER, PLAYER_SELECT_SCREEN
};
gameState currentGameState = MENU;
gameState oldGameState = GAME;

ISR(PCINT2_vect)
{
    isDataBit = ((PIND >> PIND2) & 1) != 0; // Check for the pin state (high or low)

    if (isDataBit)
    {
        uint8_t difference = currentMs - startMs; // Calculate the length to determin the value

        if (difference > STARTBIT_MIN && difference < STARTBIT_MAX) // Check if its a start bit
        {
            startBitReceived = true;
            bitCounter = 0;
            receivedData = 0;
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
                player2.xOld = player2.x;
                player2.x = receivedData & 0x1FF;
                receivedData >>= 9;
                player2.yOld = player2.y;
                player2.y = (receivedData & 0xFF);
            }
        }
    }
    startMs = currentMs; // Save the time from startup to now
}

ISR(TIMER0_COMPA_vect) // Toggle IR light
{
    static uint16_t counter = 0;
    static uint8_t msCounter = 0;

    if (++msCounter > msTime)
    {
        currentMs++;
        intCurrentMs++;
        msCounter = 0;
    }

    if (++counter > onTime)
    {
        if (dataIsSend) // If the bit is done sending, wait before sending the next bit
        {
            onTime = offTime;        // Time after which to continue to the next bit
            TCCR0A |= (1 << COM0A1); // Disable TC0
        } else // If the waiting time is passed, send the next bit
        {
            if (sendingBit++ < SENDINGDATA_LEN)
            {
                if (sendingBit == STARTBIT_VALUE) // Send start bit
                    onTime = startTime;
                else                                                                 // Send data
                    onTime = ((sendingData >> sendingBit) & 1) ? oneTime
                                                               : zeroTime; // Set the time corresponding to the bit
                TCCR0A &= ~(1 << COM0A1); // Enable TC0
            } else
            {
                sendingBit = SENDINGBIT_START_VALUE; // Once all bits are send, reset for next run
                sendingData = player1.y;
                sendingData <<= 9;
                sendingData |= player1.x;
            }
        }

        dataIsSend = !dataIsSend; // Flip between sending a bit and waiting
        counter = 0;
    }
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
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

    // Start the screen and send startup commands
    init_LCD();

    //Start touch
//    touchBegin();
    touch.begin();
    if (EEPROM_read(20) == 0)
    {
        currentGameState = PLAYER_SELECT_SCREEN;
        drawPlayerSelectScreen();
    } else if (EEPROM_read(20) == IR_38KHZ)
    {
        setFreq(IR_38KHZ);
        drawMenu();
    } else if (EEPROM_read(20) == IR_56KHZ)
    {
        setFreq(IR_56KHZ);
        drawMenu();
    }

    // Setup IR sending
    // Check nunckuk connection
    while (!startNunchuk(NUNCHUK_ADDRESS))
    {
        drawString("Nunchuk", 60, 80, 5, PLAYER_RED);
        drawString("not", 60, 120, 5, PLAYER_RED);
        drawString("found", 60, 160, 5, PLAYER_RED);

        drawString("Nunchuk", 60, 80, 5, PLAYER_YELLOW);
        drawString("not", 60, 120, 5, PLAYER_YELLOW);
        drawString("found", 60, 160, 5, PLAYER_YELLOW);
    }


    volatile int frameCounter = 0; //#TODO reset deze ergens en hem verplaatsen
    bool playerSelectButtonPressed = false;
    bool menuButtonPressed = false;
    bool exitButtonPressed = false;
    bool level1ButtonPressed = false;
    bool level2ButtonPressed = false;
    bool level3ButtonPressed = false;

    uint16_t x, y;
    uint8_t z;

    while (true)
    {
        if (intCurrentMs > FRAME_TIME)
        {
            //30 FPS
            intCurrentMs = 0;
            frameCounter++;
            if (currentGameState == GAME)
            {
                //Game code
                update();
                draw();
            } else if (currentGameState == MENU)
            {
                //Menu code
                if (touch.touched())
                {
                    while (!touch.bufferEmpty())
                    {
                        touch.readData(&y, &x, &z); //reversed order because of screen rotation
                        if ((x > 1404 && x < 2727 && y > 2378 && y < 3193) && !menuButtonPressed) //check if you pressed play button
                        {
                            menuButtonPressed = true;
                            currentGameState = LEVELSELECT;
                            drawLevelSelectScreen();
//                            drawBackground();
//                            drawInteractables();
                        } else if ((x > 891 && x < 3310 && y > 1098 && y < 1903) && !menuButtonPressed) //check if you pressed settings button
                        {
                            menuButtonPressed = true;
                            drawPlayerSelectScreen();
                            currentGameState = PLAYER_SELECT_SCREEN;
                        }
                    }
                }
            } else if (currentGameState == LEVELSELECT)
            {
                if (touch.touched())
                {
                    while (!touch.bufferEmpty())
                    {
                        touch.readData(&y, &x, &z); //reversed order because of screen rotation
                        if ((x > 552 && x < 1232 && y > 52 && y < 196) && !exitButtonPressed)
                        {
                            exitButtonPressed = true;
                            currentGameState = MENU;
                            drawMenu();
                        }
                        if ((x > 880 && x < 1656 && y < 598 && y > 556) && !level2ButtonPressed) // Level 2 button

                        {
                            //level 1 code
                            currentGameState = GAME;
                            level1ButtonPressed = true;
                            drawBackground();
                            drawInteractables();
                        }

                        if ((x > 880 && x < 1656 && y > 556 && y < 598) && !level2ButtonPressed) // Level 2 button
                        {
                            //level 2 code
                            currentGameState = GAME;
                            level2ButtonPressed = true;
                            drawBackground();
                            drawInteractables();

                        }
                        if ((x > 880 && x < 1656 && y < 438 && y > 396) && !level3ButtonPressed) // Level 3 button
                        {
                            //level 3 code
                            currentGameState = GAME;
                            level3ButtonPressed = true;
                            drawBackground();
                            drawInteractables();
                        }
                    }
                }
            } else if (currentGameState == PLAYER_SELECT_SCREEN)
            {
                menuButtonPressed = false;
                if (touch.touched())
                {
                    while (!touch.bufferEmpty())
                    {
                        touch.readData(&y, &x, &z); //reversed order because of screen rotation
                        if ((x > 150 && x < 1900 && y > 130 && y < 4000) && !playerSelectButtonPressed) //check if you pressed Player1 button #TODO coords fixen
                        {
                            playerSelectButtonPressed = true;
                            EEPROM_write(20, IR_38KHZ);
                            setFreq(IR_38KHZ);
                            currentGameState = MENU;
                            drawMenu();
                        } else if ((x > 1900 && x < 3800 && y > 130 && y < 4000) && !playerSelectButtonPressed) //check if you pressed Player2 button
                        {
                            playerSelectButtonPressed = true;
                            EEPROM_write(20, IR_56KHZ);
                            setFreq(IR_56KHZ);
                            currentGameState = MENU;
                            drawMenu();
                        }
                    }
                }
            }
        }
    }
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
    if (freq == IR_38KHZ)
    {
        msTime = 37;
        startTime = 189;
        zeroTime = 37;
        oneTime = 113;
        offTime = 37;
    } else if (freq == IR_56KHZ)
    {
        msTime = 55;
        startTime = 279;
        zeroTime = 55;
        oneTime = 167;
        offTime = 55;
    }

    OCR0A = freq;
}


void drawPlayerSelectScreen()
{
    fillRect(0, 0, 320, 240, 0x0);
    drawString("Choose your player", 70, 16, 2, PLAYER_RED);
    drawString("Player1(38KHZ)", 10, 140, 2, PLAYER_RED);
    drawString("Player2(56KHZ)", 176, 160, 2, PLAYER_RED);
}


void drawMenu()
{
    fillRect(0, 0, 320, 240, 0x0);
    drawBorder(110, 50, 116, 50, 5, PLAYER_ORANGE); //Play button
    drawBorder(65, 130, 212, 50, 5, PLAYER_ORANGE); //Settings button
    drawString("Play", 120, 60, 4, PLAYER_RED);
    drawString("Settings", 75, 140, 4, PLAYER_RED);
}

void drawLevelSelectScreen()
{
    //Delete old menu screen
    drawBorder(110, 50, 116, 50, 5, 0x0);
    drawBorder(65, 130, 212, 50, 5, 0x0);
    drawString("Play", 120, 60, 4, 0x0);
    drawString("Settings", 75, 140, 4, 0x0);
    //Draw new screen
    drawBorder(45, 191, 229, 42, 5, PLAYER_BLUE); //Level 1 button
    drawBorder(45, 131, 229, 42, 5, PLAYER_BLUE); //Level 2 button
    drawBorder(45, 70, 229, 42, 5, PLAYER_BLUE); //Level 3 button
    drawBorder(14, 0, 294, 49, 5, PLAYER_BLUE); //Exit button
    drawString("Level 1", 75, 197, 4, 0xFFFF);
    drawString("Level 2", 75, 137, 4, 0xFFFF);
    drawString("Level 3", 75, 77, 4, 0xFFFF);
    drawString("Back to menu", 50, 13, 3, 0xFFFF);

}


void update()
{
    player1.xOld = player1.x;
    player1.yOld = player1.y;
    player1.y += player1.yVelocity;

    //Checks if the player is not already at the bottom of the screen.
    if (player1.y + PLAYER_HEIGHT + player1.yVelocity <= SCREEN_HEIGHT)
    {
        player1.yVelocity += GRAVITY;
    } else
    {
        player1.yVelocity = 0;
        player1.jumping = false;
    }
    // Get the nunchuk input data
    if (!getState(NUNCHUK_ADDRESS))
    {
        return;
    }

    if (state.joy_x_axis > 140 && player1.x + PLAYER_ACTUAL_WIDTH < SCREEN_WIDTH)
        player1.x += 2;

        // Check for movement to left (only move when not against the wall)
    else if (state.joy_x_axis < 100 && player1.x > 0)
        player1.x -= 2;

    checkWallCollision();

    //Jumping and falling mechanics
    if (state.c_button == 1 && !player1.jumping)
    {
        player1.jumping = true;
        player1.yVelocity -= INITIAL_Y_VEL;
    }
}

void checkWallCollision()
{
    for (auto &wall: walls)
    {
        // Check if the player is colliding with the wall.
        if (player1.x + PLAYER_ACTUAL_WIDTH > wall.x && player1.x < wall.x + (wall.width * 2) && player1.y + PLAYER_HEIGHT > wall.y && player1.y < wall.y + wall.height)
        {
            // Check if the player is colliding with the wall from the top
            if (player1.yOld + PLAYER_HEIGHT <= wall.y)
            {
                player1.y = wall.y - PLAYER_HEIGHT;
                player1.yVelocity = 0;
                player1.jumping = false;
            } else if (player1.yOld >= wall.y + wall.height)
            { // Check if the player is colliding with the wall from the bottom
                player1.y = wall.y + wall.height;
                player1.yVelocity = 0;
            } else if (player1.xOld + PLAYER_ACTUAL_WIDTH <= wall.x)
            { // Check if the player is colliding with the wall from the left
                player1.x = wall.x - PLAYER_ACTUAL_WIDTH;
            } else if (player1.xOld >= wall.x + (wall.width * 2))
            { // Check if the player is colliding with the wall from the right
                player1.x = wall.x + (wall.width * 2);
            }
        }
    }
}

void draw()
{
    clearSprite(player1.x, player1.y, player1.xOld, player1.yOld, PLAYER_WIDTH, PLAYER_HEIGHT, Player1);
    drawSprite(player1.x, player1.y, PLAYER_WIDTH, PLAYER_HEIGHT, Player1);

    clearSprite(player2.x, player2.y, player2.xOld, player2.yOld, PLAYER_WIDTH, PLAYER_HEIGHT, Player2);
    drawSprite(player2.x, player2.y, PLAYER_WIDTH, PLAYER_HEIGHT, Player2);
}

void clearSprite(uint16_t x, uint8_t y, uint16_t xOld, uint8_t yOld, uint8_t w, uint8_t h, const uint8_t *Sprite)
{
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            xOld -= w * 2;
            yOld++;
        }
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            uint16_t color = getColor(((Sprite[PixGroup] & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)));

            if (color != 255 && !pointInRect(xOld, yOld, x, y, w, h))
            {
                uint8_t idx =
                        ((yOld / BG_SPRITE_HEIGHT % 2) ? (xOld + BG_SPRITE_WIDTH) : xOld) % BG_SPRITE_ACTUAL_WIDTH / 2 +
                        yOld % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
                drawPixel(xOld, yOld, getColor((Background[idx] & ((xOld % 2) ? 0x0F : 0xF0)) >> ((xOld % 2) ? 0 : 4)));
            }
            xOld++;
        }
    }
}

bool pointInRect(uint16_t pointX, uint8_t pointY, uint16_t x, uint8_t y, uint8_t w, uint8_t h)
{
    return pointX >= x && pointX <= x + w * 2 - 1 && pointY >= y && pointY <= y + h - 1;
}

void drawSprite(uint16_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Sprite, uint8_t ver)
{
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            x -= w * 2;
            y++;
        }
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            uint16_t color = getColor(((Sprite[PixGroup] & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)), ver);

            if (color == 255)
            {
                uint8_t idx = ((y / BG_SPRITE_HEIGHT % 2) ? (x + BG_SPRITE_WIDTH) : x) % BG_SPRITE_ACTUAL_WIDTH / 2 +
                              y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
                color = getColor((Background[idx] & ((x % 2) ? 0x0F : 0xF0)) >> ((x % 2) ? 0 : 4), ver);
            }

            drawPixel(x, y, color);

            x++;
        }
    }
}

void drawBackground()
{
    for (uint8_t y = 0; y < 24; y++)
    {
        if (y % 2 == 0)
            for (uint8_t x = 0; x < 16; x++)
                drawBackgroundTile(x * BG_SPRITE_ACTUAL_WIDTH, y * BG_SPRITE_HEIGHT, BG_SPRITE_WIDTH, BG_SPRITE_HEIGHT);
        else
            for (uint8_t x = 0; x < 17; x++)
                drawBackgroundTile(x * BG_SPRITE_ACTUAL_WIDTH - BG_SPRITE_WIDTH, y * BG_SPRITE_HEIGHT, BG_SPRITE_WIDTH,
                                   BG_SPRITE_HEIGHT);
    }
}

void drawSpriteMirror(uint16_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Sprite, uint8_t ver)
{
    uint8_t Mirr = w - 1;
    for (uint16_t PixGroup = 0; PixGroup <= w * h; PixGroup++)
    {
        if (PixGroup == 1)
        {
            Mirr++;
        }

        for (int8_t Pixel = 1; Pixel >= 0; Pixel--)
        {
            uint16_t color = getColor(((Sprite[Mirr] & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)), ver);
            drawPixel(x, y, (color == 255) ? getColor(
                    (Background[x % (BG_SPRITE_WIDTH * 2) / 2 + y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH] &
                     ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4), ver) : color);

            x++;
        }
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            x -= w * 2;
            y++;
            Mirr += w * 2;
        }
        Mirr--;
    }
}

void drawBackgroundTile(uint16_t x, uint8_t y, uint8_t w, uint8_t h)
{
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            x -= w * 2;
            y++;
        }
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            uint16_t color = getColor(((Background[PixGroup] & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)));

            for (auto &r: walls)
            {
                if (pointInRect(x, y, r.x, r.y, r.width, r.height))
                {
                    color = (color == BACKGROUND_DARK) ? FOREGROUND_DARK : FOREGROUND_LIGHT;
                    break;
                }
            }

            drawPixel(x, y, color);

            x++;
        }
    }
}

void drawInteractables()
{
    drawSprite(210, 115, BUTTON_WIDTH, BUTTON_HEIGHT, Button);
    drawSpriteMirror(210 + BUTTON_WIDTH * 2 - 2, 115, BUTTON_WIDTH, BUTTON_HEIGHT, Button);                 //Button 1
    drawSprite(230, 65, BUTTON_WIDTH, BUTTON_HEIGHT, Button);
    drawSpriteMirror(230 + BUTTON_WIDTH * 2 - 2, 65, BUTTON_WIDTH, BUTTON_HEIGHT, Button);                  //Button 2

    drawSprite(150, 230, WATER_WIDTH, WATER_HEIGHT, WaterBlue);
    drawSpriteMirror(150 + WATER_WIDTH * 2 - 2, 230, WATER_WIDTH, WATER_HEIGHT, WaterBlue);                 //water
    drawSprite(215, 230, WATER_WIDTH, WATER_HEIGHT, WaterBlue, 1);
    drawSpriteMirror(215 + WATER_WIDTH * 2 - 2, 230, WATER_WIDTH, WATER_HEIGHT, WaterBlue, 1);                 //lava
    drawSprite(189, 180, WATER_WIDTH, WATER_HEIGHT, WaterBlue, 2);
    drawSpriteMirror(189 + WATER_WIDTH * 2 - 2, 180, WATER_WIDTH, WATER_HEIGHT, WaterBlue,
                     2);                 //shrek-cum

    drawSprite(55, 40, DOOR_WIDTH, DOOR_HEIGHT, DoorCorner, 2);
    drawSpriteMirror(55 + DOOR_WIDTH * 2 - 2, 40, DOOR_WIDTH, DOOR_HEIGHT, DoorCorner,
                     2);                     //DoorTop left
    drawSprite(55, 40 + DOOR_HEIGHT, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2);
    drawSpriteMirror(55 + DOOR_WIDTH * 2 - 2, 40 + DOOR_HEIGHT, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge,
                     2);         //DoorFrame left
    drawSprite(55, 40 + DOOR_HEIGHT * 2, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2);
    drawSpriteMirror(55 + DOOR_WIDTH * 2 - 2, 40 + DOOR_HEIGHT * 2, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge,
                     2);       //DoorFrame left

    drawSprite(80, 40, DOOR_WIDTH, DOOR_HEIGHT, DoorCorner, 2);
    drawSpriteMirror(80 + DOOR_WIDTH * 2 - 2, 40, DOOR_WIDTH, DOOR_HEIGHT, DoorCorner,
                     2);                     //DoorTop right
    drawSprite(80, 40 + DOOR_HEIGHT, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2);
    drawSpriteMirror(80 + DOOR_WIDTH * 2 - 2, 40 + DOOR_HEIGHT, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge,
                     2);         //DoorFrame right
    drawSprite(80, 40 + DOOR_HEIGHT * 2, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2);
    drawSpriteMirror(80 + DOOR_WIDTH * 2 - 2, 40 + DOOR_HEIGHT * 2, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge,
                     2);       //DoorFrame right

    drawSprite(61, 48, SIGN_WIDTH, SIGN_HEIGHT, SignBlue);
    drawSpriteMirror(61 + SIGN_WIDTH * 2 - 2, 48, SIGN_WIDTH, SIGN_HEIGHT, SignBlue);                       //Blue sign
    drawSprite(86, 48, SIGN_WIDTH, SIGN_HEIGHT, SignRed);
    drawSpriteMirror(86 + SIGN_WIDTH * 2 - 2, 48, SIGN_WIDTH, SIGN_HEIGHT, SignRed);                        //Red sign

    drawSprite(280, 72, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge);
    drawSprite(280 + PLATFORM_WIDTH * 2, 72, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformMiddle);
    drawSpriteMirror(280 + PLATFORM_WIDTH * 4 - 2, 72, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge);      //Platform 1

    drawSprite(10, 112, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, 1);
    drawSprite(10 + PLATFORM_WIDTH * 2, 112, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformMiddle, 1);
    drawSpriteMirror(10 + PLATFORM_WIDTH * 4 - 2, 112, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge,
                     1);    //Platform 2

    drawSprite(12, 12, DIA_WIDTH, DIA_HEIGHT, DiaBlue);
    drawSpriteMirror(12 + DIA_WIDTH * 2 - 2, 12, DIA_WIDTH, DIA_HEIGHT, DiaBlue);                          //Dia Blue 1

    drawSprite(164, 216, DIA_WIDTH, DIA_HEIGHT, DiaBlue);
    drawSpriteMirror(164 + DIA_WIDTH * 2 - 2, 216, DIA_WIDTH, DIA_HEIGHT, DiaBlue);                        //DiaBlue 2

    drawSprite(26, 12, DIA_WIDTH, DIA_HEIGHT, DiaRed, 1);
    drawSpriteMirror(26 + DIA_WIDTH * 2 - 2, 12, DIA_WIDTH, DIA_HEIGHT, DiaRed, 1);                         //DiaRed 1

    drawSprite(229, 216, DIA_WIDTH, DIA_HEIGHT, DiaRed, 1);
    drawSpriteMirror(229 + DIA_WIDTH * 2 - 2, 216, DIA_WIDTH, DIA_HEIGHT, DiaRed, 1);                       //DiaRed 2

    drawSprite(86, 148, LEVER_BASE_WIDTH, LEVER_BASE_HEIGHT, LeverBase, 1);
    drawSpriteMirror(86 + LEVER_BASE_WIDTH * 2 - 2, 148, LEVER_BASE_WIDTH, LEVER_BASE_HEIGHT, LeverBase, 1); //LeverBase

    drawSprite(94, 142, LEVER_TOP_WIDTH, LEVER_TOP_HEIGHT, LeverTop, 1);                                 //LeverTop
}

uint16_t getColor(uint8_t Color, uint8_t ver)
{
    switch (Color)
    {
        case 0:             //0000
            return BLACK;

        case 1:             //0001
            return PLAYER_RED;

        case 2:             //0010
            return PLAYER_ORANGE;

        case 3:             //0011
            return PLAYER_YELLOW;

        case 4:             //0100
            if (ver == 0)
            {
                return PLAYER_DARK_BLUE;
            } else if (ver == 1)
            {
                return PLAYER_RED;
            } else
            {
                return INTER_BROWN;
            }

        case 5:             //0101
            if (ver == 0)
            {
                return PLAYER_BLUE;
            } else if (ver == 1)
            {
                return PLAYER_ORANGE;
            } else
            {
                return SWAMP_GREEN;
            }

        case 6:             //0110
            if (ver == 0)
            {
                return PLAYER_LIGHT_BLUE;
            } else if (ver == 1)
            {
                return PLAYER_YELLOW;
            } else
            {
                return SWAMP_GREEN;
            }

        case 7:             //0111

        case 8:             //1000
            return INTER_BROWN;

        case 9:             //1001
            return INTER_GOLD;

        case 10:            //1010
            if (ver == 0)
            {
                return INTER_PURPLE;
            } else
            {
                return INTER_YELLOW;
            }
        case 11:            //1011

        case 12:            //1100
            return BACKGROUND_LIGHT;

        case 13:            //1101
            return BACKGROUND_DARK;

        case 14:            //1110
            return 0xFFFF;  //white

        case 15:            //1111
            return 255;
        default:
            return 255;
    }
}