#include "Includes.c"

struct Rect
{
    uint16_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
};
volatile int frameCounter = 0;

void initTimer0();
void checkWallCollision();
void CheckPlatformCollision();
void checkCollision(Rect &);
bool rectangleCollision(uint16_t, uint8_t, Rect &);
void checkButtons();
void checkLevers();
void checkDias();

void setFreq(uint8_t);
uint8_t getFreq();
void drawBackgroundTile(uint16_t, uint8_t, uint8_t, uint8_t);
void clearSprite(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *);
bool pointInRect(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);
void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t ver = 0);
void drawSpriteMirror(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t *, uint8_t ver = 0);
void drawBackground();
void drawInteractables();
uint16_t getColor(uint8_t, uint8_t ver = 0);
void Update();
void DrawPlayers();

void level1();
void level2();

void clearPlat(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);
void drawPlatH(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawPlatV(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawLiquid(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawLever(uint16_t, uint8_t, uint8_t);

struct
{
public:
    uint16_t x = 20;
    uint8_t y = 120;
    uint16_t xOld = x;
    uint8_t yOld = y;
    int8_t yVelocity = 0;
    bool jumping = false;
}player1;

struct
{
public:
    uint16_t x = 229;
    uint8_t y = 40;
    uint16_t xOld = x;
    uint8_t yOld = y;
}player2;

struct Collect
{
    uint16_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
    uint8_t version;

    void draw()
    {
        if(x == NULL) return;
        drawSprite(x, y, DIA_WIDTH, DIA_HEIGHT, DiaBlue, version);
        drawSpriteMirror(x + DIA_WIDTH * 2, y, DIA_WIDTH, DIA_HEIGHT, DiaBlue, version);
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
        if(bounds.x == NULL) return;
        if(maxX == minX){
            MoveMaxY();
        }else if(maxY == minY){
            MoveMaxX();
        }
    }
    void MoveMin()
    {
        if(bounds.x == NULL) return;
        if(maxX == minX){
            MoveMinY();
        }else if(maxY == minY){
            MoveMinX();
        }
    }
    void MoveMaxX()
    {
        if(bounds.x < maxX && frameCounter%3==0)
        {
            clearPlat(bounds.x-3, bounds.y, bounds.x, bounds.y, bounds.width, bounds.height);
            bounds.x+=3;
            this->draw();
        }
    }
    void MoveMaxY()
    {
        if(bounds.y < maxY && frameCounter%3==0)
        {
            clearPlat(bounds.x, bounds.y+3, bounds.x, bounds.y, bounds.width, bounds.height);
            bounds.y+=3;
            this->draw();
        }
    }
    void MoveMinX()
    {
        if(bounds.x > minX && frameCounter%3==0)
        {
            clearPlat(bounds.x+3, bounds.y, bounds.x, bounds.y, bounds.width, bounds.height);
            bounds.x-=3;
            this->draw();
        }
    }
    void MoveMinY()
    {
        if(bounds.y > minY && frameCounter%3==0)
        {
            clearPlat(bounds.x, bounds.y-3, bounds.x, bounds.y, bounds.width, bounds.height);
            bounds.y-=3;
            this->draw();
        }
    }
    bool isMax()
    {
        if(bounds.x == NULL) return;
        if((bounds.x >= maxX && bounds.y >= maxY) || (bounds.x <= minX && bounds.y <= minY))
        {
            return true;
        }else
        {
            return false;
        }
    }

    void draw()
    {
        if(bounds.x == NULL) return;
        if(maxX == minX){
            drawPlatH(bounds.x, bounds.y, (bounds.width)*2, bounds.height, version);
        }else if(maxY == minY){
            drawPlatV(bounds.x, bounds.y, (bounds.width)*2, bounds.height, version);
        }
        
    }
};

struct lever
{
    Rect bounds;
    bool state;
    bool standingOn;
    Platform *plat;
    uint8_t Version;
    void activate()
    {
        if(bounds.x == NULL) return;
        if(state)
        {
            //move platform
            plat->MoveMax();
        }else
        {
            //move platform back
            plat->MoveMin();
        }
        if(!(plat->isMax()))
        {
            this->draw();
        }
    }

    void draw(){
        if(bounds.x == NULL) return;
        drawLever(bounds.x - 7, bounds.y + 1, 1);
        if(state)
        {
            drawSpriteMirror((bounds).x -7 , (bounds).y - 7, LEVER_TOP_WIDTH, LEVER_TOP_HEIGHT, LeverTop, Version);
        }else
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
    button *connectedButton;

    void activate()
    {
        if(bounds.x == NULL) return;
        if(standingOn || connectedButton->standingOn)
        {
            // move platform
            plat[0]->MoveMax();
            plat[1]->MoveMax();
        }else
        {
            //move platform back
            plat[0]->MoveMin();
            plat[1]->MoveMin();
        }
        if(!(plat[0]->isMax()))
        {
            this->draw();
        }

    }

    void draw(){
        if(bounds.x == NULL) return;
        drawSprite((bounds).x - 4, (bounds).y - 3, BUTTON_WIDTH, BUTTON_HEIGHT, Button);
        drawSpriteMirror((bounds).x - 4 + BUTTON_WIDTH * 2, (bounds).y - 3, BUTTON_WIDTH, BUTTON_HEIGHT, Button);
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
        if(bounds.x == NULL) return;
        drawLiquid((bounds).x, (bounds).y, WATER_WIDTH*4, WATER_HEIGHT, Version);
    }
};

Platform Platform1 = {};
Platform Platform2 = {};
Platform Platform3 = {};
Platform Platform4 = {};
Platform Platform5 = {};
Platform Platform6 = {};
Platform Platform7 = {};
Platform Platform8 = {};
Platform Platform9 = {};

lever Lever1 = {};
lever Lever2 = {};
lever Lever3 = {};
lever Lever4 = {};

button button1 = {};
button button2 = {};
button button3 = {};
button button4 = {};

Collect Dia1 = {};
Collect Dia2 = {};
Collect Dia3 = {};
Collect Dia4 = {};

Liquid liq1 = {};
Liquid liq2 = {};
Liquid liq3 = {};
Liquid liq4 = {};
Liquid liq5 = {};
Liquid liq6 = {};

Rect Door1 = {};
Rect Door2 = {};

Rect walls[] =              // MAX 30 WALLS
{
    {},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}
};

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
    &Platform9
};

lever *levers[] =
{
    &Lever1,
    &Lever2,
    &Lever3,
    &Lever4
};

button *buttons[] =
{
    &button1,
    &button2,
    &button3,
    &button4
};

Collect *Dias[] =
{
    &Dia1,
    &Dia2,
    &Dia3,
    &Dia4
};

Liquid *liquids[] = 
{
    &liq1,
    &liq2,
    &liq3,
    &liq4,
    &liq5,
    &liq6
};

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
    MENU,
    GAME,
    LEVELSELECT
} currentGameState;

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

ISR(TIMER0_COMPA_vect) // Toggle IR light
{
    static  uint16_t counter = 0;
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

    // Setup IR sending
    DDRD |= (1 << DDD6);
    initTimer0();

    // Setup IR recieving
    PORTD |= (1 << PORTD2); // pull up
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);

    setFreq(IR_38KHZ);
    currentGameState = GAME;

    // Setup screen
    Wire.begin();
    setupSPI();

    sei();

    // Start the screen and send startup commands
    START_UP();

    // Check nunckuk connection
    while (!startNunchuk(NUNCHUK_ADDRESS))
    {
        fillRect(0, 0, 320, 240, PLAYER_RED);
    }

    level1();
    
    while (true)
    {
        if (intCurrentMs > FRAME_TIME)
        {
            // 30 FPS
            intCurrentMs = 0;
            frameCounter++;
            if (currentGameState == GAME)
            {
                // Game code
                Update();
                DrawPlayers();
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

uint8_t getFreq(){
    return OCR0A;
}

void Update()
{
    player1.xOld = player1.x;
    player1.yOld = player1.y;

    player2.xOld = player2.x;
    player2.yOld = player2.y;

    player1.y += player1.yVelocity;
    player1.yVelocity += GRAVITY;

    if(state.z_button == 1)          //Testing purposes only ---------------------------------------
    {
        player2.y++;
        player2.x = 229;
    }
    // Get the nunchuk input data
    if (!getState(NUNCHUK_ADDRESS))
    {
        return;
    }
    

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
    for(button *B : buttons)
    {
        if(rectangleCollision(player1.x, player1.y, B->bounds) || rectangleCollision(player2.x, player2.y, B->bounds))
        {
            B->standingOn = true;
        }else
        {
            B->standingOn = false;
        }
        B->activate();
    }
}

void checkLevers()
{
    for(lever *L : levers)
    {
        if(rectangleCollision(player1.x, player1.y, L->bounds) || rectangleCollision(player2.x, player2.y, L->bounds))
        {
            if(!L->standingOn)
            {
                L->state = !L->state;
                L->standingOn = true;
            }
        }else if(L->standingOn)
        {
            L->standingOn = false;
        }
        L->activate();
    }
}

void checkDias(){
    for(Collect *D : Dias)
    {
        Rect temp = {D->x, D->y, D->w, D->h};
        if(rectangleCollision(player1.x, player1.y, temp) && D->version == 0)
        {
            //COLLECT DIAMOND
        }else if(rectangleCollision(player2.x, player2.y, temp) && D->version == 0){
            D->draw();
        }else if(rectangleCollision(player1.x, player1.y, temp) && D->version == 1){
            D->draw();
        }else if(rectangleCollision(player2.x, player2.y, temp) && D->version == 1){
            //COLLECT DIAMOND
        }
    }
}

void CheckPlatformCollision(){
    for (Platform *platform : platforms)
    {
        if (rectangleCollision(player1.x, player1.y, platform->bounds))
        {
            if(platform->maxX == platform->minX){
                drawPlatH(platform->bounds.x, platform->bounds.y, (platform->bounds.width)*2, platform->bounds.height, platform->version);
            }else if(platform->maxY == platform->minY){
                drawPlatV(platform->bounds.x, platform->bounds.y, (platform->bounds.width)*2, platform->bounds.height, platform->version);
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
        // Check if the player is colliding with the wall from the top
        if (player1.yOld + PLAYER_HEIGHT <= bounds.y || player1.y < bounds.y)
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
        else if (player1.xOld + PLAYER_ACTUAL_WIDTH <= bounds.x)
        { // Check if the player is colliding with the wall from the left
            player1.x = bounds.x - PLAYER_ACTUAL_WIDTH;
            
        }
        else if (player1.xOld >= bounds.x + (bounds.width * 2)) 
        { // Check if the player is colliding with the wall from the right
            player1.x = bounds.x + (bounds.width * 2);
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

            if (color != 255 && !pointInRect(xOld, yOld, x, y, w, h))
            {
                uint8_t idx = ((yOld / BG_SPRITE_HEIGHT % 2) ? (xOld + BG_SPRITE_WIDTH) : xOld) % BG_SPRITE_ACTUAL_WIDTH / 2 + yOld % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
                drawPixel(xOld, yOld, getColor((Background[idx] & ((xOld % 2) ? 0x0F : 0xF0)) >> ((xOld % 2) ? 0 : 4)));
            }
            xOld++;
        }
    }
}

void clearPlat(uint16_t x, uint8_t y, uint16_t xOld, uint8_t yOld, uint8_t w, uint8_t h)
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
            uint8_t idx = ((yOld / BG_SPRITE_HEIGHT % 2) ? (xOld + BG_SPRITE_WIDTH) : xOld) % BG_SPRITE_ACTUAL_WIDTH / 2 + yOld % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
            drawPixel(xOld, yOld, getColor((Background[idx] & ((xOld % 2) ? 0x0F : 0xF0)) >> ((xOld % 2) ? 0 : 4)));
            xOld++;
        }
    }
}

void drawPlatH(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawLineH(x, y, x+w, BLACK);
    drawRect(x, y+1, w, h-2, INTER_BROWN);
    drawRect(x+1, y+2, w-2, h-4, INTER_GOLD);
    drawRect(x+2, y+3, w-4, h-6, getColor(PLATFORM_MIDDLE_COLOR, ver));
    drawLineH(x, y+7, x+w, BLACK);
}

void drawPlatV(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawRect(x+1, y, w-2, h, INTER_BROWN);
    drawRect(x+2, y+1, w-4, h-2, INTER_GOLD);
    drawRect(x+3, y+2, w-6, h-4, getColor(PLATFORM_MIDDLE_COLOR, ver));
}

void drawLiquid(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawLineH(x+1, y, x+w-1, getColor(LIQUID_TOP_COLOR, ver));
    drawLineH(x+1, y+1, x+w-1, getColor(LIQUID_MIDDLE_COLOR, ver));
    drawLineH(x+1, y+2, x+w-1, getColor(LIQUID_BOTTOM_COLOR, ver));
    drawLineH(x+1, y+3, x+w-1, getColor(LIQUID_BOTTOM_COLOR, ver));
}

void drawDoor(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawRect(x+1, y+1, w-2, h-1, SWAMP_GREEN);
    drawRect(x+2, y+2, w-4, h-2, SWAMP_GREEN);
    drawRect(x+3, y+3, w-6, h-3, BLACK);
    fillRect(x+4, y+4, w-8, h-4, INTER_BROWN);

    if(ver == 0)
    {
        drawSprite(x + 6, y + 8, SIGN_WIDTH, SIGN_HEIGHT, SignBlue, ver);
        drawSpriteMirror(x + 6 + SIGN_WIDTH * 2, y + 8, SIGN_WIDTH, SIGN_HEIGHT, SignBlue, ver);
    }else
    {
        drawSprite(x+6, y+8, SIGN_WIDTH, SIGN_HEIGHT, SignRed, ver);
        drawSpriteMirror(x + 6 + SIGN_WIDTH * 2, y + 8, SIGN_WIDTH, SIGN_HEIGHT, SignRed, ver);

    }
}

void drawLever(uint16_t x, uint8_t y, uint8_t ver){
    drawRect(x, y, LEVER_BASE_WIDTH*4, LEVER_BASE_HEIGHT, BLACK);
    drawRect(x+1, y+1, LEVER_BASE_WIDTH*4-2, LEVER_BASE_HEIGHT-3, INTER_GOLD);
    drawLineH(x+1, y+3, LEVER_BASE_WIDTH*4-2, INTER_BROWN);
}

bool pointInRect(uint16_t pointX, uint8_t pointY, uint16_t x, uint8_t y, uint8_t w, uint8_t h)
{
    return pointX >= x && pointX <= x + w * 2 - 1 && pointY >= y && pointY <= y + h - 1;
}

bool rectangleCollision(uint16_t playerX, uint8_t playerY, Rect &bounds)
{
    return playerX + PLAYER_ACTUAL_WIDTH > bounds.x && playerX < bounds.x + (bounds.width * 2) && playerY + PLAYER_HEIGHT > bounds.y && playerY < bounds.y + bounds.height;
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
    for(button *B : buttons){
        B->draw();
    }

    for(lever *L : levers){
        L->draw();
    }

    for(Platform *P : platforms){
        P->draw();
    }

    for(Collect *D : Dias){
        D->draw();
    }

    drawDoor(Door1.x, Door1.y, Door1.width, Door1.height, 0);
    drawDoor(Door2.x, Door2.y, Door2.width, Door2.height, 1);
    
    drawLiquid(150, 230, WATER_WIDTH*4, WATER_HEIGHT, 0); // water
    drawLiquid(215, 230, WATER_WIDTH*4, WATER_HEIGHT, 1); // lava
    drawLiquid(189, 180, WATER_WIDTH*4, WATER_HEIGHT, 2); // poison
}

void level1(){
    //initialize all walls in lvl 1
    walls[0] = Rect{0, 0, 5, 240};
    walls[1] = Rect{10, 0, 155, 10};
    walls[2] = Rect{310, 10, 5, 230};
    walls[3] = Rect{10, 230, 155, 10};
    walls[4] = Rect{10, 40, 20, 40};
    walls[5] = Rect{50, 70, 115, 10};
    walls[6] = Rect{153, 50, 30, 20};
    walls[7] = Rect{40, 110, 75, 10};
    walls[8] = Rect{180, 120, 50, 10};
    walls[9] = Rect{280, 120, 15, 20};
    walls[10] = Rect{290, 140, 10, 10};
    walls[11] = Rect{10, 150, 70, 10};
    walls[12] = Rect{150, 150, 5, 30};
    walls[13] = Rect{150, 180, 52, 10};
    walls[14] = Rect{280, 210, 15, 20};
    walls[15] = Rect{10, 190, 40, 10};

    Door1 = {55, 40, DOOR_WIDTH * 4, DOOR_HEIGHT * 3};                              //door blue
    Door2 = {80, 40, DOOR_WIDTH * 4, DOOR_HEIGHT * 3};                              //door red

    liq1 = {150, 230, WATER_WIDTH*4, WATER_HEIGHT, 0};                              //water
    liq2 = {215, 230, WATER_WIDTH*4, WATER_HEIGHT, 1};                              //lava
    liq3 = {189, 180, WATER_WIDTH*4, WATER_HEIGHT, 2};                              //poison
    liq4 = {};
    liq5 = {};

    Platform1 = {{280, 70, 15, 8}, 280, 280, 72, 112};                              //main purple platform
    Platform2 = {{10, 112, 15, 8}, 10, 10, 112, 142, 1};                            //yellow platform
    Platform3 = {{40, 10, 4, 30}, 40, 70, 10, 10};                                  //diamonds purple platform

    Lever1 = {{93, 149, 1, 1}, false, false, &Platform2, 1};                        // lever for yellow platform

    button1 = {{146, 108, BUTTON_WIDTH, 2}, false, {&Platform1, &Platform3}};       //button for purple platform
    button2 = {{181, 48, BUTTON_WIDTH, 2}, false, {&Platform1, &Platform3}};        //button for purple platform

    // Connect buttons to eachother
    button1.setConnectedButton(&button2);
    button2.setConnectedButton(&button1);

    Dia1 = {12, 12, DIA_WIDTH * 2, DIA_HEIGHT, 0};      //blue diamond upperleft
    Dia2 = {164, 216, DIA_WIDTH * 2, DIA_HEIGHT, 0};    //blue diamond bottom
    Dia3 = {26, 12, DIA_WIDTH * 2, DIA_HEIGHT, 1};      //red diamond upperleft
    Dia4 = {229, 216, DIA_WIDTH * 2, DIA_HEIGHT, 1};    //red diamond bottom

    //draw everything
    drawBackground();
    drawInteractables();
}

void level2(){

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
        else
        {
            return INTER_YELLOW;
        }

    case 11: // 1011

    case 12: // 1100
        return BACKGROUND_LIGHT;

    case 13: // 1101
        return BACKGROUND_DARK;

    case 14:           // 1110
        return 0xFFFF; // white

    case 15: // 1111
        return 255;
    default:
        return 255;
    }
}