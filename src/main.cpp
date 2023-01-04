#include <main.h>

volatile int frameCounter = 0;

// Structs
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
} player2;

struct Collect
{
    uint16_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
    uint8_t version;

    void draw()
    {
        if (x == NULL)
            return;
        drawSprite(x, y, DIA_WIDTH, DIA_HEIGHT, Diamond, version);
        drawSpriteMirror(x + DIA_WIDTH * 2, y, DIA_WIDTH, DIA_HEIGHT, Diamond, version);
    }
};

struct Platform
{
    Rect bounds;
    uint16_t minX;
    uint16_t maxX;
    uint8_t minY;
    uint8_t maxY;
    uint8_t version;

    void MoveMax()
    {
        if (bounds.x == NULL)
            return;
        if (maxX == minX)
        {
            MoveMaxY();
        }
        else if (maxY == minY)
        {
            MoveMaxX();
        }
    }
    void MoveMin()
    {
        if (bounds.x == NULL)
            return;
        if (maxX == minX)
        {
            MoveMinY();
        }
        else if (maxY == minY)
        {
            MoveMinX();
        }
    }

    void MoveMaxX()
    {
        if (bounds.x > minX && frameCounter % 3 == 0)
        {
            clearPlat(bounds.x + 3, bounds.y, bounds.x, bounds.y, bounds.width, bounds.height);
            bounds.x -= 3;
            this->draw();
        }
    }
    void MoveMaxY()
    {
        if (bounds.y < maxY && frameCounter % 3 == 0)
        {
            clearPlat(bounds.x, bounds.y + 3, bounds.x, bounds.y, bounds.width, bounds.height);
            bounds.y += 3;
            this->draw();
        }
    }
    void MoveMinX()
    {
        if (bounds.x > minX && frameCounter % 3 == 0)
        {
            clearPlat(bounds.x + 3, bounds.y, bounds.x, bounds.y, bounds.width, bounds.height);
            bounds.x -= 3;
            this->draw();
        }
    }
    void MoveMinY()
    {
        if (bounds.y > minY && frameCounter % 3 == 0)
        {
            clearPlat(bounds.x, bounds.y - 3, bounds.x, bounds.y, bounds.width, bounds.height);
            bounds.y -= 3;
            this->draw();
        }
    }

    bool isMax()
    {
        if (bounds.x == NULL)
            return;
        if ((bounds.x >= maxX && bounds.y >= maxY) || (bounds.x <= minX && bounds.y <= minY))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void draw()
    {
        if (bounds.x == NULL)
            return;
        if (maxX == minX)
        {
            drawPlatH(bounds.x, bounds.y, bounds.width, bounds.height, version);
        }
        else if (maxY == minY)
        {
            drawPlatV(bounds.x, bounds.y, bounds.width, bounds.height, version);
        }
    }
};

struct lever
{
    Rect bounds;
    bool state;
    bool standingOn;
    Platform *plat[2];
    uint8_t Version;
    void activate()
    {
        if (bounds.x == NULL)
            return;
        if (state)
        {
            // move platform
            plat[0]->MoveMax();
            plat[1]->MoveMax();
        }
        else
        {
            // move platform back
            plat[0]->MoveMin();
            plat[1]->MoveMin();
        }
        if (!(plat[0]->isMax()))
        {
            this->draw();
        }
    }

    void draw()
    {
        if (bounds.x == NULL)
            return;
        drawLever(bounds.x - 7, bounds.y + 1, 1);
        if (state)
        {
            drawSpriteMirror((bounds).x - 7, (bounds).y - 7, LEVER_TOP_WIDTH, LEVER_TOP_HEIGHT, LeverTop, Version);
        }
        else
        {
            drawSprite((bounds).x + 1, (bounds).y - 7, LEVER_TOP_WIDTH, LEVER_TOP_HEIGHT, LeverTop, Version);
        }
    }
};

struct button
{
    Rect bounds;
    bool standingOn;
    Platform *plat[2];
    uint8_t Version;
    button *connectedButton;

    void activate()
    {
        if (bounds.x == NULL)
            return;
        if (standingOn || connectedButton->standingOn)
        {
            // move platform
            plat[0]->MoveMax();
            plat[1]->MoveMax();
        }
        else
        {
            // move platform back
            plat[0]->MoveMin();
            plat[1]->MoveMin();
        }
        if (!(plat[0]->isMax()) || standingOn || connectedButton->standingOn)
        {
            this->draw();
        }
    }

    void draw()
    {
        if (bounds.x == NULL)
            return;
        drawSprite((bounds).x - 4, (bounds).y - 3, BUTTON_WIDTH, BUTTON_HEIGHT, Button, Version);
        drawSpriteMirror((bounds).x - 4 + BUTTON_WIDTH * 2, (bounds).y - 3, BUTTON_WIDTH, BUTTON_HEIGHT, Button, Version);
    }

    void setConnectedButton(button *btn)
    {
        this->connectedButton = btn;
    }
};

struct Liquid
{
    Rect bounds;
    uint8_t Version;
    void draw()
    {
        if (bounds.x == NULL)
            return;
        drawLiquid(bounds.x, bounds.y, bounds.width, bounds.height, Version);
    }
};

// Level objects
Platform Platform1, Platform2, Platform3, Platform4, Platform5, Platform6, Platform7, Platform8, Platform9;
lever Lever1, Lever2, Lever3, Lever4;
button button1, button2, button3, button4;
Collect Dia1, Dia2, Dia3, Dia4;
Liquid liq1, liq2, liq3, liq4;
Rect Door1, Door2;
Rect walls[20];

// Arrays to store the objects (mainly for looping purposes)
Platform *platforms[] =
    {
        &Platform1,
        &Platform2,
        &Platform3,
        &Platform4,
        &Platform5,
        &Platform6,
        &Platform7,
        &Platform8,
        &Platform9};

lever *levers[] =
    {
        &Lever1,
        &Lever2,
        &Lever3,
        &Lever4};

button *buttons[] =
    {
        &button1,
        &button2,
        &button3,
        &button4};

Collect *Dias[] =
    {
        &Dia1,
        &Dia2,
        &Dia3,
        &Dia4};

Liquid *liquids[] =
    {
        &liq1,
        &liq2,
        &liq3,
        &liq4};

const uint8_t liveCount[MAX_LIVES + 1] = {
    //+1 since we start at 0.
    // Patterns for all the numbers and letters.
    0x3F, // 0
    0x6,  // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D  // 5
          // 0x7D, // 6
          // 0x7,  // 7
          // 0x7F, // 8
          // 0x6F, // 9
          // 0x77, // A
          // 0x7C, // B
          // 0x39, // C
          // 0x5E, // D
          // 0x79, // E
          // 0x71, // F
};

// IR sending variables
bool dataIsSend = false;                    // Check to see if the current bit is done sending
uint32_t sendingData = 0;                   // Data to send over IR
int8_t sendingBit = SENDINGBIT_START_VALUE; // The data bit to send;
uint16_t onTime = 0;

// Time variables
uint8_t msTime;
volatile uint32_t currentMs = 0;   // Keeps track of the milliseconds passed since startup
volatile uint8_t intCurrentMs = 0; // Keeps track of the milliseconds passed and resets every frame

// Time lengths for IR data sending
uint16_t startTime; // Length of startbit
uint8_t zeroTime;   // Length of 0
uint8_t oneTime;    // Length of 1
uint8_t offTime;    // Length of pause between bits

// IR receiving variables
uint32_t startMs = 0;          // Used to determin the length of each received bit
bool startBitReceived = false; // Tracks if the staring bit was received before accepting data
uint32_t receivedData = 0;     // All the received bits
uint8_t bitCounter = 0;        // Current bit (used for bitshifting 1's in to receivedData)
bool isDataBit = false;        // Differentiates data from pauses

// Other variables
uint8_t lives = 5;
uint8_t score = START_SCORE;
uint8_t level = 1;
bool levelCompleted = false;
bool playerDied = false;

// All the gamestates
enum gameState
{
    MENU,
    GAME,
    LEVELSELECT,
    SETTINGS,
    PAUSE,
    GAMEOVER,
    PLAYER_SELECT_SCREEN
} currentGameState;

// IR receiving protocol
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
            {
                receivedData &= ~(1 << bitCounter++);
            }
            else if (difference > ONE_MIN && difference < ONE_MAX) // Check if its a one
            {
                receivedData |= (1 << bitCounter++);
            }

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

// IR sending protocol (with timer to keep track of ms)
ISR(TIMER0_COMPA_vect)
{
    static uint16_t counter = 0;
    static uint8_t msCounter = 0;

    // After enough interupts, a ms has passed
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
        }
        else // If the waiting time is passed, send the next bit
        {
            if (sendingBit++ < SENDINGDATA_LEN)
            {
                if (sendingBit == STARTBIT_VALUE) // Send start bit
                {
                    onTime = startTime;
                }
                else // Send data
                {
                    onTime = ((sendingData >> sendingBit) & 1) ? oneTime : zeroTime; // Set the time corresponding to the bit
                }

                TCCR0A &= ~(1 << COM0A1); // Enable TC0
            }
            else
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

int main(void)
{
    // Setup IR led
    DDRD |= (1 << DDD6);
    initTimer0();

    // Setup IR recieving
    PORTD |= (1 << PORTD2); // pull up
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);

    // Setup wire
    Wire.begin();

    // Start the SPI communication and send screen startup commands
    setupSPI();
    START_UP();

    // Always set a start frequency so the game loop can run (because ms timer is in ISR of IR sending)
    setFreq(IR_38KHZ);

    // Check if the frequency has already been set
    {
        uint8_t freq = EEPROM_read(20);
        if (freq == IR_38KHZ || freq == IR_56KHZ)
        {
            setFreq(freq);
            currentGameState = MENU;
            drawMenu();
        }
        else
        {
            currentGameState = PLAYER_SELECT_SCREEN;
            drawPlayerSelectScreen();
        }
    }

    // Enable global interupts
    sei();

    // Set and show lives.
    lives = getLives();
    showLives(lives);

    // Check nunckuk connection
    if (!startNunchuk(NUNCHUK_ADDRESS))
    {
        fillScreen(0);

        while (!startNunchuk(NUNCHUK_ADDRESS))
        {
            drawString("Nunchuk", 60, 80, 5, PLAYER_RED);
            drawString("not", 60, 120, 5, PLAYER_RED);
            drawString("found", 60, 160, 5, PLAYER_RED);

            drawString("Nunchuk", 60, 80, 5, PLAYER_YELLOW);
            drawString("not", 60, 120, 5, PLAYER_YELLOW);
            drawString("found", 60, 160, 5, PLAYER_YELLOW);
        }

        switch (currentGameState)
        {
        case MENU:
            drawMenu();
            break;
        case PLAYER_SELECT_SCREEN:
            drawPlayerSelectScreen();
            break;
        }
    }

    // Draws the score on the screen.
    fillRect(SCORE_POS, 0, 40, 18, BACKGROUND_LIGHT);
    drawScore(score, false);

    while (true)
    {
        if (intCurrentMs > FRAME_TIME)
        {
            // 30 FPS
            intCurrentMs = 0;
            frameCounter++;

            // Get the nunchuk input data (skip the code if the nunchuk is disconnected)
            if (!getState(NUNCHUK_ADDRESS))
            {
                continue;
            }

            if (currentGameState == GAME)
            {
                // Game code
                // Checks if the score is 0 or lower. If so, the player loses a life.
                if (score <= 0)
                {
                    lives--;
                    setLives(lives);
                    playerDied = true;
                }
                // Draws the score on the screen.
                if (frameCounter % 30 == 0)
                {
                    drawScore(score, true); // Draws over the previous score.
                    score--;
                    drawScore(score, false);
                }

                // Game code
                Update();
                DrawPlayers();
                // Check if player is dead.
                if (playerDied)
                {
                    // Show lives on 7 segments display.
                    showLives(getLives());
                    // Reset all player positions and draw the interactables.
                    clearWholeSprite(player1.x, player1.y, PLAYER_WIDTH, PLAYER_HEIGHT);
                    player1.x = 13;
                    player1.y = 170;
                    playerDied = false;

                    // Reset score and redraw it.
                    drawScore(score, true);
                    score = START_SCORE;
                    drawScore(score, false);

                    // Reset framecounter
                    frameCounter = 0;

                    drawInteractables();
                }

                // If level is completed update the highscore and set score to START_SCORE, also add 1 to level.
                if (levelCompleted)
                {
                    updateHighscore(score, level);
                    levelCompleted = false;
                    score = START_SCORE;
                    level++;
                }
            }
            else if (currentGameState == MENU)
            {
                if (state.c_button && !state.c_button_old)
                {
                    currentGameState = LEVELSELECT;
                    drawLevelSelectScreen();
                }
                else if (state.z_button && !state.z_button_old)
                {
                    currentGameState = PLAYER_SELECT_SCREEN;
                    drawPlayerSelectScreen();
                }
            }
            else if (currentGameState == LEVELSELECT)
            {
                if (state.c_button && !state.c_button_old)
                {
                    currentGameState = GAME;
                    level1();
                }
                else if (state.z_button && !state.z_button_old)
                {
                    currentGameState = MENU;
                    drawMenu();
                }
            }
            else if (currentGameState == PLAYER_SELECT_SCREEN)
            {
                if (state.c_button && !state.c_button_old)
                {
                    EEPROM_write(20, IR_38KHZ);
                    setFreq(IR_38KHZ);
                    currentGameState = MENU;
                    drawMenu();
                }
                else if (state.z_button && !state.z_button_old)
                {
                    EEPROM_write(20, IR_56KHZ);
                    setFreq(IR_56KHZ);
                    currentGameState = MENU;
                    drawMenu();
                }
            }
        }
    }
}

void initTimer0()
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
        msTime = 37;
        startTime = 189;
        zeroTime = 37;
        oneTime = 113;
        offTime = 37;
    }
    else
    {
        msTime = 55;
        startTime = 279;
        zeroTime = 55;
        oneTime = 167;
        offTime = 55;
    }
}

uint8_t getFreq()
{
    return OCR0A;
}

void Update()
{
    player1.xOld = player1.x;
    player1.yOld = player1.y;

    player1.y += player1.yVelocity;
    player1.yVelocity += GRAVITY;

    // Check for movement to right
    if (state.joy_x_axis > 169)
    {
        player1.x += MOVEMENT_SPEED;
    }
    // Check for movement to left
    else if (state.joy_x_axis < 85)
    {
        player1.x -= MOVEMENT_SPEED;
    }

    checkWallCollision();
    CheckPlatformCollision();

    checkButtons();
    checkLevers();
    checkDias();

    // Jumping and falling mechanics
    if (state.c_button == 1 && !player1.jumping)
    {
        player1.jumping = true;
        player1.yVelocity -= INITIAL_Y_VEL;
    }
}

void checkButtons()
{
    for (button *B : buttons)
    {
        if (rectangleCollision(player1.x, player1.y, B->bounds) || rectangleCollision(player2.x, player2.y, B->bounds))
        {
            B->standingOn = true;
        }
        else
        {
            B->standingOn = false;
        }
        B->activate();
    }
}

void checkLevers()
{
    for (lever *L : levers)
    {
        if (rectangleCollision(player1.x, player1.y, L->bounds) || rectangleCollision(player2.x, player2.y, L->bounds))
        {
            if (!L->standingOn)
            {
                L->state = !L->state;
                L->standingOn = true;
            }
        }
        else if (L->standingOn)
        {
            L->standingOn = false;
        }
        L->activate();
    }
}

void checkDias()
{
    for (Collect *D : Dias)
    {
        Rect temp = {D->x, D->y, D->w, D->h};
        if (rectangleCollision(player1.x, player1.y, temp) && D->version == 0)
        {
            // COLLECT DIAMOND
        }
        else if (rectangleCollision(player2.x, player2.y, temp) && D->version == 0)
        {
            D->draw();
        }
        else if (rectangleCollision(player1.x, player1.y, temp) && D->version == 1)
        {
            D->draw();
        }
        else if (rectangleCollision(player2.x, player2.y, temp) && D->version == 1)
        {
            // COLLECT DIAMOND
        }
    }
}

void CheckPlatformCollision()
{
    for (Platform *platform : platforms)
    {
        if (rectangleCollision(player1.x, player1.y, platform->bounds))
        {
            if (platform->maxX == platform->minX)
            {
                drawPlatH(platform->bounds.x, platform->bounds.y, platform->bounds.width, platform->bounds.height, platform->version);
            }
            else if (platform->maxY == platform->minY)
            {
                drawPlatV(platform->bounds.x, platform->bounds.y, platform->bounds.width, platform->bounds.height, platform->version);
            }
        }
        checkCollision(platform->bounds);
    }
}

void checkWallCollision()
{
    for (auto &wall : walls)
    {
        checkCollision(wall);
    }
}

void checkCollision(Rect &bounds)
{
    // Check if the player is colliding with the wall.
    if (rectangleCollision(player1.x, player1.y, bounds))
    {
        if (player1.xOld + PLAYER_ACTUAL_WIDTH <= bounds.x)
        { // Check if the player is colliding with the wall from the left
            player1.x = bounds.x - PLAYER_ACTUAL_WIDTH;
        }
        else if (player1.xOld >= bounds.x + bounds.width)
        { // Check if the player is colliding with the wall from the right
            player1.x = bounds.x + bounds.width;
        }
        else if (player1.yOld + PLAYER_HEIGHT <= bounds.y || player1.y < bounds.y) // Check if the player is colliding with the wall from the top
        {
            player1.y = bounds.y - PLAYER_HEIGHT;
            player1.yVelocity = 0;
            player1.jumping = false;
        }
        else if (player1.yOld >= bounds.y + bounds.height)
        { // Check if the player is colliding with the wall from the bottom
            player1.y = bounds.y + bounds.height;
            player1.yVelocity = 0;
        }
    }
}

void DrawPlayers()
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

            if (color != 255 && !pointInRect(xOld, yOld, x, y, w * 2, h))
            {
                uint8_t idx = ((yOld / BG_SPRITE_HEIGHT % 2) ? (xOld + BG_SPRITE_WIDTH) : xOld) % BG_SPRITE_ACTUAL_WIDTH / 2 + yOld % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
                drawPixel(xOld, yOld, getColor((Background[idx] & ((xOld % 2) ? 0x0F : 0xF0)) >> ((xOld % 2) ? 0 : 4)));
            }
            xOld++;
        }
    }
}

void clearWholeSprite(uint16_t x, uint8_t y, uint8_t w, uint8_t h)
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
            uint8_t idx = ((y / BG_SPRITE_HEIGHT % 2) ? (x + BG_SPRITE_WIDTH) : x) % BG_SPRITE_ACTUAL_WIDTH / 2 + y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
            drawPixel(x, y, getColor((Background[idx] & ((x % 2) ? 0x0F : 0xF0)) >> ((x % 2) ? 0 : 4)));
            x++;
        }
    }
}

void clearPlat(uint16_t x, uint8_t y, uint16_t xOld, uint8_t yOld, uint8_t w, uint8_t h)
{
    for (uint16_t PixGroup = 0; PixGroup < w / 2 * h; PixGroup++)
    {
        if (PixGroup % (w / 2) == 0 && PixGroup != 0)
        {
            xOld -= w;
            yOld++;
        }
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            uint8_t idx = ((yOld / BG_SPRITE_HEIGHT % 2) ? (xOld + BG_SPRITE_WIDTH) : xOld) % BG_SPRITE_ACTUAL_WIDTH / 2 + yOld % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
            drawPixel(xOld, yOld, getColor((Background[idx] & ((xOld % 2) ? 0x0F : 0xF0)) >> ((xOld % 2) ? 0 : 4)));
            xOld++;
        }
    }
}

void drawPlatH(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawLineH(x, y, x + w, BLACK);
    drawRect(x, y + 1, w, h - 2, INTER_BROWN);
    drawRect(x + 1, y + 2, w - 2, h - 4, INTER_GOLD);
    drawRect(x + 2, y + 3, w - 4, h - 6, getColor(PLATFORM_MIDDLE_COLOR, ver));
    drawLineH(x, y + 7, x + w, BLACK);
}

void drawPlatV(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawRect(x + 1, y, w - 2, h, INTER_BROWN);
    drawRect(x + 2, y + 1, w - 4, h - 2, INTER_GOLD);
    drawRect(x + 3, y + 2, w - 6, h - 4, getColor(PLATFORM_MIDDLE_COLOR, ver));
}

void drawLiquid(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawLineH(x + 1, y, x + w - 1, getColor(LIQUID_TOP_COLOR, ver));
    drawLineH(x + 1, y + 1, x + w - 1, getColor(LIQUID_MIDDLE_COLOR, ver));
    drawLineH(x + 1, y + 2, x + w - 1, getColor(LIQUID_BOTTOM_COLOR, ver));
    drawLineH(x + 1, y + 3, x + w - 1, getColor(LIQUID_BOTTOM_COLOR, ver));
}

void drawDoor(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawRect(x + 1, y + 1, w - 2, h - 1, SWAMP_GREEN);
    drawRect(x + 2, y + 2, w - 4, h - 2, SWAMP_GREEN);
    drawRect(x + 3, y + 3, w - 6, h - 3, BLACK);
    fillRect(x + 4, y + 4, w - 8, h - 4, INTER_BROWN);

    if (ver == 0)
    {
        drawSprite(x + 6, y + 8, SIGN_WIDTH, SIGN_HEIGHT, SignBlue, ver);
        drawSpriteMirror(x + 6 + SIGN_WIDTH * 2, y + 8, SIGN_WIDTH, SIGN_HEIGHT, SignBlue, ver);
    }
    else
    {
        drawSprite(x + 6, y + 8, SIGN_WIDTH, SIGN_HEIGHT, SignRed, ver);
        drawSpriteMirror(x + 6 + SIGN_WIDTH * 2, y + 8, SIGN_WIDTH, SIGN_HEIGHT, SignRed, ver);
    }
}

void drawLever(uint16_t x, uint8_t y, uint8_t ver)
{
    drawRect(x, y, LEVER_BASE_WIDTH * 4, LEVER_BASE_HEIGHT, BLACK);
    drawRect(x + 1, y + 1, LEVER_BASE_WIDTH * 4 - 2, LEVER_BASE_HEIGHT - 3, INTER_GOLD);
    drawLineH(x + 1, y + 3, LEVER_BASE_WIDTH * 4 - 2, INTER_BROWN);
}

bool pointInRect(uint16_t pointX, uint8_t pointY, uint16_t x, uint8_t y, uint16_t w, uint8_t h)
{
    return pointX >= x && pointX <= x + w - 1 && pointY >= y && pointY <= y + h - 1;
}

bool rectangleCollision(uint16_t playerX, uint8_t playerY, Rect &bounds)
{
    return playerX + PLAYER_ACTUAL_WIDTH > bounds.x && playerX < bounds.x + bounds.width && playerY + PLAYER_HEIGHT > bounds.y && playerY < bounds.y + bounds.height;
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
                uint8_t idx = ((y / BG_SPRITE_HEIGHT % 2) ? (x + BG_SPRITE_WIDTH) : x) % BG_SPRITE_ACTUAL_WIDTH / 2 + y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
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
                drawBackgroundTile(x * BG_SPRITE_ACTUAL_WIDTH - BG_SPRITE_WIDTH, y * BG_SPRITE_HEIGHT, BG_SPRITE_WIDTH, BG_SPRITE_HEIGHT);
    }
}

void drawSpriteMirror(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t *Sprite, uint8_t ver)
{
    uint8_t yBackup = y;

    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            x -= w * 2;
            y++;
        }

        for (int8_t Pixel = 1; Pixel >= 0; Pixel--)
        {
            uint16_t color = getColor(((Sprite[w - 1 - PixGroup % w + (y - yBackup) * w] & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)), ver);

            if (color == 255)
            {
                uint8_t idx = ((y / BG_SPRITE_HEIGHT % 2) ? (x + BG_SPRITE_WIDTH) : x) % BG_SPRITE_ACTUAL_WIDTH / 2 + y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
                color = getColor((Background[idx] & ((x % 2) ? 0x0F : 0xF0)) >> ((x % 2) ? 0 : 4), ver);
            }

            drawPixel(x, y, color);

            x++;
        }
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

            for (auto &r : walls)
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
    for (button *B : buttons)
    {
        B->draw();
    }

    for (lever *L : levers)
    {
        L->draw();
    }

    for (Platform *P : platforms)
    {
        P->draw();
    }

    for (Collect *D : Dias)
    {
        D->draw();
    }

    for (Liquid *L : liquids)
    {
        L->draw();
    }

    drawDoor(Door1.x, Door1.y, Door1.width, Door1.height, 0);
    drawDoor(Door2.x, Door2.y, Door2.width, Door2.height, 1);
}

void level1()
{
    walls[0] = {0, 0, 10, 240};
    walls[1] = {10, 0, 310, 10};
    walls[2] = {310, 10, 10, 230};
    walls[3] = {10, 230, 310, 10};
    walls[4] = {10, 40, 40, 40};
    walls[5] = {50, 70, 230, 10};
    walls[6] = {153, 50, 60, 20};
    walls[7] = {40, 110, 150, 10};
    walls[8] = {180, 120, 100, 10};
    walls[9] = {280, 120, 30, 20};
    walls[10] = {290, 140, 20, 10};
    walls[11] = {10, 150, 140, 10};
    walls[12] = {150, 150, 10, 30};
    walls[13] = {150, 180, 104, 10};
    walls[14] = {280, 210, 30, 20};
    walls[15] = {10, 190, 80, 10};

    Door1 = {55, 40, DOOR_WIDTH, DOOR_HEIGHT}; // door blue
    Door2 = {80, 40, DOOR_WIDTH, DOOR_HEIGHT}; // door red

    liq1 = {150, 230, LIQUID_WIDTH, LIQUID_HEIGHT, 0}; // water
    liq3 = {189, 180, LIQUID_WIDTH, LIQUID_HEIGHT, 2}; // poison
    liq2 = {215, 230, LIQUID_WIDTH, LIQUID_HEIGHT, 1}; // lava

    Platform1 = {{280, 70, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 280, 280, 72, 112};   // main purple platform
    Platform2 = {{10, 111, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 10, 10, 111, 141, 1}; // yellow platform
    Platform3 = {{40, 10, PLATFORM_HEIGHT, PLATFORM_WIDTH}, 40, 70, 10, 10};       // diamonds purple platform

    Lever1 = {{93, 149, 2, 1}, false, false, {&Platform2}, 1}; // lever for yellow platform

    button1 = {{146, 108, BUTTON_WIDTH*2, 2}, false, {&Platform1, &Platform3}, 0}; // button for purple platform
    button2 = {{181, 48, BUTTON_WIDTH*2, 2}, false, {&Platform1, &Platform3}, 0};  // button for purple platform

    // Connect buttons to eachother
    button1.setConnectedButton(&button2);
    button2.setConnectedButton(&button1);

    Dia1 = {12, 12, DIA_WIDTH * 4, DIA_HEIGHT, 0};   // blue diamond upperleft
    Dia2 = {164, 216, DIA_WIDTH * 4, DIA_HEIGHT, 0}; // blue diamond bottom
    Dia3 = {26, 12, DIA_WIDTH * 4, DIA_HEIGHT, 1};   // red diamond upperleft
    Dia4 = {229, 216, DIA_WIDTH * 4, DIA_HEIGHT, 1}; // red diamond bottom

    player1.x = 13;
    player1.y = 170;

    player2.x = 13;
    player2.y = 210;

    // draw everything
    drawBackground();
    drawInteractables();
}

void level2()
{
    walls[0] = {0, 0, 5, 240};
    walls[1] = {5, 0, 310, 5};
    walls[2] = {315, 0, 5, 240};
    walls[3] = {5, 235, 310, 5};
    walls[4] = {5, 227, 68, 8};
    walls[5] = {5, 178, 68, 17};
    walls[6] = {5, 118, 68, 17};
    walls[7] = {5, 65, 68, 23};
    walls[8] = {35, 35, 5, 30};
    walls[9] = {70, 35, 26, 45};
    walls[10] = {96, 73, 50, 7};
    walls[11] = {103, 178, 147, 17};
    walls[12] = {124, 150, 21, 10};
    walls[13] = {195, 150, 22, 10};
    walls[14] = {205, 80, 40, 7};
    walls[15] = {245, 150, 5, 28};
    walls[16] = {250, 150, 30, 21};
    walls[17] = {260, 35, 55, 10};
    walls[18] = {280, 150, 5, 28};
    walls[19] = {280, 178, 35, 17};

    Door1 = {45, 35, DOOR_WIDTH, DOOR_HEIGHT}; // door blue
    Door2 = {10, 35, DOOR_WIDTH, DOOR_HEIGHT}; // door red

    liq1 = {27, 118, LIQUID_WIDTH, LIQUID_HEIGHT, 0}; // water

    liq2 = {27, 178, LIQUID_WIDTH, LIQUID_HEIGHT, 1}; // lava
    liq3 = {27, 227, LIQUID_WIDTH, LIQUID_HEIGHT, 1}; // lava

    liq4 = {116, 178, 124, LIQUID_HEIGHT, 2}; // poison

    Platform1 = {{5, 27, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 5, 5, 6, 27, 0};           // purple platform
    Platform2 = {{40, 27, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 40, 40, 6, 27, 1};        // yellow platform
    Platform3 = {{127, 5, PLATFORM_HEIGHT, PLATFORM_WIDTH}, 127, 88, 5, 5, 2};        // Red platform left
    Platform4 = {{260, 5, PLATFORM_HEIGHT, PLATFORM_WIDTH}, 260, 221, 5, 5, 2};       // Red platform right
    Platform5 = {{65, 88, PLATFORM_HEIGHT, PLATFORM_WIDTH}, 65, 104, 88, 88, 3};      // Green platform left
    Platform6 = {{73, 118, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 73, 73, 118, 226, 4};    // Blue platform
    Platform7 = {{250, 203, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 250, 250, 173, 203, 5}; // Light_blue platform top
    Platform8 = {{250, 219, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 250, 250, 189, 219, 5}; // Light_blue platform bottom
    Platform9 = {{285, 150, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 285, 285, 150, 108, 6}; // White Platform

    Lever1 = {{280, 34, 2, 1}, true, false, {&Platform7, &Platform8}, 5}; // Lever light_blue
    Lever2 = {{310, 34, 2, 1}, true, false, {&Platform2}, 1};             // Lever yellow
    Lever3 = {{299, 177, 2, 1}, false, false, {&Platform5}, 3};           // Lever green
    Lever4 = {{299, 234, 2, 1}, true, false, {&Platform1}, 0};            // Lever purple

    button1 = {{127, 233, BUTTON_WIDTH*2, 2}, false, {&Platform6}, 4};            // button for blue platform
    button2 = {{12, 225, BUTTON_WIDTH*2, 2}, false, {&Platform9}, 6};             // button for white platform
    button3 = {{12, 176, BUTTON_WIDTH*2, 2}, false, {&Platform6}, 4};             // button for blue platform
    button4 = {{12, 116, BUTTON_WIDTH*2, 2}, false, {&Platform3, &Platform4}, 2}; // button for red platform

    // Connect buttons to eachother
    button1.setConnectedButton(&button3);
    button3.setConnectedButton(&button1);

    Dia1 = {}; // blue diamond upperleft
    Dia2 = {}; // blue diamond bottom
    Dia3 = {}; // red diamond upperleft
    Dia4 = {}; // red diamond bottom

    player1.x = 207;
    player1.y = 215;

    player2.x = 187;
    player2.y = 215;

    // draw everything
    drawBackground();
    drawInteractables();
}

uint16_t getColor(uint8_t Color, uint8_t ver)
{
    switch (Color)
    {
    case 0: // 0000
        return BLACK;

    case 1: // 0001
        return PLAYER_RED;

    case 2: // 0010
        return PLAYER_ORANGE;

    case 3: // 0011
        return PLAYER_YELLOW;

    case LIQUID_BOTTOM_COLOR: // 0100
        if (ver == 0)
        {
            return PLAYER_DARK_BLUE;
        }
        else if (ver == 1)
        {
            return PLAYER_RED;
        }
        else
        {
            return INTER_BROWN;
        }

    case LIQUID_MIDDLE_COLOR: // 0101
        if (ver == 0)
        {
            return PLAYER_BLUE;
        }
        else if (ver == 1)
        {
            return PLAYER_ORANGE;
        }
        else
        {
            return SWAMP_GREEN;
        }

    case LIQUID_TOP_COLOR: // 0110
        if (ver == 0)
        {
            return PLAYER_LIGHT_BLUE;
        }
        else if (ver == 1)
        {
            return PLAYER_YELLOW;
        }
        else
        {
            return SWAMP_GREEN;
        }

    case 7: // 0111

    case 8: // 1000
        return INTER_BROWN;

    case 9: // 1001
        return INTER_GOLD;

    case PLATFORM_MIDDLE_COLOR: // 0b00001010
        if (ver == 0)
        {
            return INTER_PURPLE;
        }
        else if (ver == 1)
        {
            return INTER_YELLOW;
        }
        else if (ver == 2)
        {
            return PLAYER_RED;
        }
        else if (ver == 3)
        {
            return SWAMP_GREEN;
        }
        else if (ver == 4)
        {
            return PLAYER_DARK_BLUE;
        }
        else if (ver == 5)
        {
            return PLAYER_LIGHT_BLUE;
        }
        else if (ver == 6)
        {
            return WHITE;
        }

    case 11: // 1011

    case 12: // 1100
        return BACKGROUND_LIGHT;

    case 13: // 1101
        return BACKGROUND_DARK;

    case 14:          // 1110
        return WHITE; // white

    case 15: // 1111
        return ALPHA;
    default:
        return ALPHA;
    }
}

void drawPlayerSelectScreen()
{
    fillScreen(0x0);
    drawString("Choose your player", 70, 16, 2, PLAYER_RED);
    drawString("Player1(38KHZ)", 10, 140, 2, PLAYER_RED);
    drawString("Player2(56KHZ)", 176, 160, 2, PLAYER_RED);
}

void drawMenu()
{
    fillScreen(0x0);
    drawBorder(110, 50, 116, 50, 5, PLAYER_ORANGE); // Play button
    drawBorder(65, 130, 212, 50, 5, PLAYER_ORANGE); // Settings button
    drawString("Play", 120, 60, 4, PLAYER_RED);
    drawString("Settings", 75, 140, 4, PLAYER_RED);
}

void drawLevelSelectScreen()
{
    // Delete old menu screen
    drawBorder(110, 50, 116, 50, 5, 0x0);
    drawBorder(65, 130, 212, 50, 5, 0x0);
    drawString("Play", 120, 60, 4, 0x0);
    drawString("Settings", 75, 140, 4, 0x0);
    // Draw new screen
    drawBorder(45, 191, 229, 42, 5, PLAYER_BLUE); // Level 1 button
    drawBorder(45, 131, 229, 42, 5, PLAYER_BLUE); // Level 2 button
    drawBorder(45, 70, 229, 42, 5, PLAYER_BLUE);  // Level 3 button
    drawBorder(14, 0, 294, 49, 5, PLAYER_BLUE);   // Exit button
    drawString("Level 1", 75, 197, 4, 0xFFFF);
    drawString("Level 2", 75, 137, 4, 0xFFFF);
    drawString("Level 3", 75, 77, 4, 0xFFFF);
    drawString("Back to menu", 50, 13, 3, 0xFFFF);
}

uint8_t getLives()
{
    return EEPROM_read(LIVES_ADDR);
}

void setLives(uint8_t &lives)
{
    if (lives > MAX_LIVES)
    {
        lives = MAX_LIVES; // Limits the live amount to MAX_LIVES = 5.
    }
    if (lives == 0)
    {
        lives = MAX_LIVES; // Resets the lives to 5 if the player has no lives left.
    }

    EEPROM_update(LIVES_ADDR, lives);
}

void showLives(uint8_t value)
{                                    // Talks to port expander and tells which pins to toggle.
    Wire.beginTransmission(IO_ADDR); // Starts transmission with the port expander on port 0x37.
    Wire.write(~liveCount[value]);   // Sends the reverse of the pattern for the correct letter, reverse because common anode.
    Wire.endTransmission();          // Ends transmission
}

void showScore(bool scoreType, uint8_t level)
{
    // Checks what scoretype is selected and shows the score accordingly in the level or at the menu screen.
    if (scoreType == HIGHSCORE)
    {
        // Draw score on the menu screen.
        //  EEPROM_read(HIGHSCORE_START_LEVEL_ADDR + level);
    }
    else
    {
        // Draw score on the game screen.
    }
}

void updateHighscore(uint8_t score, uint8_t level)
{
    // Checks if level is in range.
    if (level > MAX_LEVEL)
    {
        level = MAX_LEVEL;
    }
    // Checks if the score is higher than the highscore and updates it if it is.
    if (score > EEPROM_read(HIGHSCORE_START_LEVEL_ADDR + level))
    {
        EEPROM_write(HIGHSCORE_START_LEVEL_ADDR + level, score);
    }
}

void drawScore(uint8_t highscore, bool clearScore)
{
    uint16_t color = PLAYER_BLUE;
    if (clearScore)
    {
        color = BACKGROUND_LIGHT;
    }
    else if (highscore < 50)
    {
        color = PLAYER_RED;
    }
    else if (highscore >= 50 && highscore < 100)
    {
        color = PLAYER_YELLOW;
    }

    unsigned char *pText = new unsigned char[4];
    pText[0] = highscore / 100 + '0';
    pText[1] = (highscore % 100) / 10 + '0';
    pText[2] = highscore % 10 + '0';
    pText[3] = '\0';
    drawString((const char *)pText, SCORE_POS, 2, 2, color);

    delete[] pText;
    pText = nullptr;
}