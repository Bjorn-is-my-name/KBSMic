#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include "LCD.cpp"
#include "Player1.c"
#include "Player2.c"
#include "Background.c"
#include "Nunchuk.cpp"

void init_timer0();

void setFreq(uint8_t);

void drawBackgroundTile(uint16_t, uint8_t, uint8_t, uint8_t);

void clearSprite(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *);

bool pointInRect(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);

void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *);

void drawBackground();

uint16_t getColor(uint8_t);

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

#define SENDINGDATA_LEN 9
#define SENDINGBIT_START_VALUE (- 2)

#define STARTBIT_VALUE (-1)
#define STARTBIT_MIN 3
#define STARTBIT_MAX 6
#define ZERO_MAX 2
#define ONE_MIN 1
#define ONE_MAX 4

#define INITIAL_Y_VEL 8
#define FRAME_TIME 32 //(1000/30FPS)-1 = 32

#define BG_SPRITE_AMOUNT 192
#define BG_SPRITE_WIDTH 10
#define BG_SPRITE_ACTUAL_WIDTH 20
#define BG_SPRITE_HEIGHT 10

#define NUM_OF_WALLS 16

struct {
public:
    uint16_t x = 13;
    uint8_t y = 170;
    uint16_t xOld = x;
    uint8_t yOld = y;
    int8_t yVelocity = 0;
    bool jumping = false;
} player1;

struct {
public:
    uint16_t x = 13;
    uint8_t y = 210;
    uint16_t xOld = x;
    uint8_t yOld = y;
//    uint8_t animation
} player2;

struct Rect {
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

// Check to see if the current bit is done sending
bool dataIsSend = false;
// Data to send over IR
uint16_t sendingData = 0;
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
uint16_t receivedData = 0;
uint8_t bitCounter = 0;
bool isDataBit = false;

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

                TCCR0A &= ~(1 << COM0A1); // Enabe TC0
            } else {
                sendingBit = SENDINGBIT_START_VALUE; // Once all bits are send, reset for next run
                sendingData = player1.x;
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

    // Start the screen and send startup commands
    START_UP();



    // Check nunckuk connection
    while (!startNunchuk(NUNCHUK_ADDRESS)) {
        fillRect(0, 0, 320, 240, PLAYER_RED);
    }

    drawBackground();
    volatile int frameCounter = 0; //#TODO reset deze ergens en hem verplaatsen

    while (true) {
        if (intCurrentMs > FRAME_TIME) { //30 FPS
            intCurrentMs = 0;
            update();
            draw();
            frameCounter++;
        }
    }
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

    // Get the nunchuck input data
    if (!getState(NUNCHUK_ADDRESS))
        return;

    // Check for movement to right (only move when not against the wall)
    if (state.joy_x_axis > 140 && player1.x + PLAYER_ACTUAL_WIDTH < SCREEN_WIDTH)
        player1.x += 2;

        // Check for movement to left (only move when not against the wall)
    else if (state.joy_x_axis < 100 && player1.x > 0)
        player1.x -= 2;

    // If jumping, lower the velocity until it hits the max (negative) velocity
    if (player1.jumping) {
        player1.y -= player1.yVelocity;

        if (player1.yVelocity > -INITIAL_Y_VEL)
            player1.yVelocity--;
    }

    // If the player is back on the ground, stop jumping
    if (player1.y == SCREEN_HEIGHT - PLAYER_HEIGHT) {
        player1.jumping = false;
        player1.yVelocity = 0;
    }

    // Set the player to jump when the nunchuck movement is high enough and not jumping already
    if (state.accel_z_axis < 0xFF && !player1.jumping) {
        player1.jumping = true;
        player1.yVelocity = INITIAL_Y_VEL;
    }
}

void draw() {
    clearSprite(player1.x, player1.y, player1.xOld, player1.yOld, PLAYER_WIDTH, PLAYER_HEIGHT, Player1);
    drawSprite(player1.x, player1.y, PLAYER_WIDTH, PLAYER_HEIGHT, Player1);

    clearSprite(player2.x, player2.y, player2.xOld, player2.yOld, PLAYER_WIDTH, PLAYER_HEIGHT, Player2);
    drawSprite(player2.x, player2.y, PLAYER_WIDTH, PLAYER_HEIGHT, Player2);
}

void clearSprite(uint16_t x, uint8_t y, uint16_t xOld, uint8_t yOld, uint8_t w, uint8_t h, const uint8_t *Sprite) {
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++) {
        if (PixGroup % w == 0 && PixGroup != 0) {
            xOld -= w * 2;
            yOld++;
        }
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++) {
            uint16_t color = getColor(((Sprite[PixGroup] & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)));

            if (color != 255 && !pointInRect(xOld, yOld, x, y, w, h)) {
                uint8_t idx =
                        ((yOld / BG_SPRITE_HEIGHT % 2) ? (xOld + BG_SPRITE_WIDTH) : xOld) % BG_SPRITE_ACTUAL_WIDTH / 2 +
                        yOld % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
                drawPixel(xOld, yOld, getColor((Background[idx] & ((xOld % 2) ? 0x0F : 0xF0)) >> ((xOld % 2) ? 0 : 4)));
            }

            xOld++;
        }
    }
}

bool pointInRect(uint16_t pointX, uint8_t pointY, uint16_t x, uint8_t y, uint8_t w, uint8_t h) {
    return pointX >= x && pointX <= x + w * 2 - 1 && pointY >= y && pointY <= y + h - 1;
}

void drawSprite(uint16_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Sprite) {
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++) {
        if (PixGroup % w == 0 && PixGroup != 0) {
            x -= w * 2;
            y++;
        }
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++) {
            uint16_t color = getColor(((Sprite[PixGroup] & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)));

            if (color == 255) {
                uint8_t idx = ((y / BG_SPRITE_HEIGHT % 2) ? (x + BG_SPRITE_WIDTH) : x) % BG_SPRITE_ACTUAL_WIDTH / 2 +
                              y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
                color = getColor((Background[idx] & ((x % 2) ? 0x0F : 0xF0)) >> ((x % 2) ? 0 : 4));
            }

            drawPixel(x, y, color);

            x++;
        }
    }
}

void drawBackground() {
    for (uint8_t y = 0; y < 24; y++) {
        if (y % 2 == 0)
            for (uint8_t x = 0; x < 16; x++)
                drawBackgroundTile(x * BG_SPRITE_ACTUAL_WIDTH, y * BG_SPRITE_HEIGHT, BG_SPRITE_WIDTH, BG_SPRITE_HEIGHT);
        else
            for (uint8_t x = 0; x < 17; x++)
                drawBackgroundTile(x * BG_SPRITE_ACTUAL_WIDTH - BG_SPRITE_WIDTH, y * BG_SPRITE_HEIGHT, BG_SPRITE_WIDTH,
                                   BG_SPRITE_HEIGHT);
    }
}

void drawBackgroundTile(uint16_t x, uint8_t y, uint8_t w, uint8_t h) {
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++) {
        if (PixGroup % w == 0 && PixGroup != 0) {
            x -= w * 2;
            y++;
        }
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++) {
            uint16_t color = getColor(((Background[PixGroup] & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)));

            for (auto &r: walls) {
                if (pointInRect(x, y, r.x, r.y, r.w, r.h)) {
                    color = (color == BACKGROUND_DARK) ? FOREGROUND_DARK : FOREGROUND_LIGHT;
                    break;
                }
            }

            drawPixel(x, y, color);

            x++;
        }
    }
}

uint16_t getColor(uint8_t Color) {
    switch (Color) {
        case 0:             //0000
            return BLACK;

        case 1:             //0001
            return PLAYER_RED;

        case 2:             //0010
            return PLAYER_ORANGE;

        case 3:             //0011
            return PLAYER_YELLOW;

        case 4:             //0100
            return PLAYER_DARK_BLUE;

        case 5:             //0101
            return PLAYER_BLUE;

        case 6:             //0110
            return PLAYER_LIGHT_BLUE;

        case 7:             //0111
            return SWAMP_GREEN;

        case 8:             //1000
            return INTER_BROWN;

        case 9:             //1001
            return INTER_GOLD;

        case 10:            //1010
            return INTER_PURPLE;

        case 11:            //1011
            return INTER_YELLOW;

        case 12:            //1100
            //if background
            return BACKGROUND_LIGHT;
            //else return FOREGROUND_LIGHT;

        case 13:            //1101
            //if background
            return BACKGROUND_DARK;
            //else return FOREGROUND_DARK;

        case 14:            //1110
            return 0xFFFF;  //white

        case 15:            //1111
            return 255;
        default:
            return 255;
    }
}