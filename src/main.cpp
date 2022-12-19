#include "Includes.c"

struct Rect
{
    uint16_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
};

void initTimer0();
void checkWallCollision();
void CheckPlatformCollision();
void checkCollision(Rect &);
bool rectangleCollision(uint16_t, uint8_t, Rect &);

void setFreq(uint8_t);
void drawBackgroundTile(uint16_t, uint8_t, uint8_t, uint8_t);
void clearSprite(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *);
bool pointInRect(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);
void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t ver = 0);
void drawSpriteMirror(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t *, uint8_t ver = 0);
void drawBackground();
void drawInteractables();
uint16_t getColor(uint8_t, uint8_t ver = 0);
void update();
void draw();
void clearPlat(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);

struct
{
public:
    uint16_t x = 180;
    uint8_t y = 20;
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
    // uint8_t animation
}player2;

struct Collect
{
    uint16_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
};

struct lever
{
    Rect bounds;
    bool state;
    bool standingOn;
};

struct Platform
{
    Rect bounds;
    uint16_t minX;
    uint16_t maxX;
    uint8_t minY;
    uint8_t maxY;
    uint8_t version;
    bool standingOn;

    Platform(Rect Bounds, uint16_t MinX, uint16_t MaxX, uint8_t MinY, uint8_t MaxY, uint8_t Version = 0){
        this->bounds = Bounds;
        this->minX = MinX;
        this->maxX = MaxX;
        this->minY = MinY;
        this->maxY = MaxY;
        this->version = Version;
        this->standingOn = false;
    }

    void MoveMaxX(){
        clearPlat(maxY, maxX, bounds.x, bounds.y, bounds.width, bounds.height);
        bounds.x = maxX;
        //draw sprite
        drawSprite(bounds.x, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, version);
        drawSprite(bounds.x + PLATFORM_WIDTH * 2, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformMiddle, version);
        drawSpriteMirror(bounds.x + PLATFORM_WIDTH * 4, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, version);
    }
    void MoveMaxY(){
        clearPlat(maxY, maxX, bounds.x, bounds.y, bounds.width, bounds.height);
        bounds.y = maxY;
        //draw sprite
        drawSprite(bounds.x, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, version);
        drawSprite(bounds.x + PLATFORM_WIDTH * 2, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformMiddle, version);
        drawSpriteMirror(bounds.x + PLATFORM_WIDTH * 4, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, version);
    }
    void MoveMinX(){
        clearPlat(maxY, maxX, bounds.x, bounds.y, bounds.width, bounds.height);
        bounds.x = minX;
        //draw sprite
        drawSprite(bounds.x, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, version);
        drawSprite(bounds.x + PLATFORM_WIDTH * 2, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformMiddle, version);
        drawSpriteMirror(bounds.x + PLATFORM_WIDTH * 4, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, version);
    }
    void MoveMinY(){
        clearPlat(maxY, maxX, bounds.x, bounds.y, bounds.width, bounds.height);
        bounds.y = minY;
        drawSprite(bounds.x, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, version);
        drawSprite(bounds.x + PLATFORM_WIDTH * 2, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformMiddle, version);
        drawSpriteMirror(bounds.x + PLATFORM_WIDTH * 4, bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, version);
    }
};


Rect Door1 = {55, 40, DOOR_WIDTH * 4, DOOR_HEIGHT * 3};
Rect Door2 = {80, 40, DOOR_WIDTH * 4, DOOR_HEIGHT * 3};

Rect Platform1Bounds = {280, 72, 15, 8};
Rect Platform2Bounds = {10, 112, 15, 8};

Platform Platform1 = {Platform1Bounds, 280, 280, 72, 112};
Platform Platform2 = {Platform2Bounds, 10, 10, 112, 142, 1};

Rect Button1 = {214, 118, 3, 2};
Rect Button2 = {234, 68, 3, 2};

Rect Lever1Bounds = {91, 148, 3, 2};
lever Lever1 = {Lever1Bounds, false};

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

Platform *platforms[] = {
    &Platform1,
    &Platform2
};

lever levers[] = {
    Lever1
};

Collect Dias[] = {
    Collect{12, 12, DIA_WIDTH * 2, DIA_HEIGHT},
    Collect{164, 216, DIA_WIDTH * 2, DIA_HEIGHT},
    Collect{26, 12, DIA_WIDTH * 2, DIA_HEIGHT},
    Collect{229, 216, DIA_WIDTH * 2, DIA_HEIGHT}
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

    drawBackground();
    drawInteractables();
    volatile int frameCounter = 0; // #TODO reset deze ergens en hem verplaatsen

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
                update();
                draw();
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

void update()
{
    player1.xOld = player1.x;
    player1.yOld = player1.y;

    player2.xOld = player2.x;
    player2.yOld = player2.y;

    player1.y += player1.yVelocity;
    player1.yVelocity += GRAVITY;

    if(state.z_button == 1){
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

    if( rectangleCollision(player1.x, player1.y, Button1) ||
        rectangleCollision(player1.x, player1.y, Button2) ||
        rectangleCollision(player2.x, player2.y, Button1) ||
        rectangleCollision(player2.x, player2.y, Button2))
    {
        if(!Platform1.standingOn){
            Platform1.MoveMaxY();
            Platform1.standingOn = true;
        }
    }else if(Platform1.standingOn){
        Platform1.MoveMinY();
        Platform1.standingOn = false;
    }else{
        drawSprite(210, 115, BUTTON_WIDTH, BUTTON_HEIGHT, Button);
        drawSpriteMirror(210 + BUTTON_WIDTH * 2, 115, BUTTON_WIDTH, BUTTON_HEIGHT, Button); // Button 1

        drawSprite(230, 65, BUTTON_WIDTH, BUTTON_HEIGHT, Button);
        drawSpriteMirror(230 + BUTTON_WIDTH * 2, 65, BUTTON_WIDTH, BUTTON_HEIGHT, Button); // Button 2
    }

        for(lever L : levers){
            if(rectangleCollision(player1.x, player1.y, L.bounds) || rectangleCollision(player2.x, player2.y, L.bounds))
            {
                if(!L.standingOn)
                {
                    L.state != L.state;
                    L.standingOn = true;
                }
            }else if(L.standingOn)
            {
                L.standingOn = false;
            }
        }

    // Jumping and falling mechanics
    if (state.c_button == 1 && !player1.jumping)
    {
        player1.jumping = true;
        player1.yVelocity -= INITIAL_Y_VEL;
    }
}

void CheckPlatformCollision(){
    for (auto &platform : platforms)
    {
        checkCollision(platform->bounds);
        if (rectangleCollision(player1.x, player1.y, platform->bounds))
        {
            drawSprite(platform->bounds.x, platform->bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, platform->version);
            drawSprite(platform->bounds.x + PLATFORM_WIDTH * 2, platform->bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformMiddle, platform->version);
            drawSpriteMirror(platform->bounds.x + PLATFORM_WIDTH * 4, platform->bounds.y, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, platform->version);
        }
    }

}

void checkWallCollision()
{
    for (auto &wall : walls)
    {
        checkCollision(wall);
    }
}

void checkCollision(Rect &bounds){
    // Check if the player is colliding with the wall.
    if (rectangleCollision(player1.x, player1.y, bounds))
    {
        // Check if the player is colliding with the wall from the top
        if (player1.yOld + PLAYER_HEIGHT <= bounds.y)
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
    drawSprite(210, 115, BUTTON_WIDTH, BUTTON_HEIGHT, Button);
    drawSpriteMirror(210 + BUTTON_WIDTH * 2, 115, BUTTON_WIDTH, BUTTON_HEIGHT, Button); // Button 1

    drawSprite(230, 65, BUTTON_WIDTH, BUTTON_HEIGHT, Button);
    drawSpriteMirror(230 + BUTTON_WIDTH * 2, 65, BUTTON_WIDTH, BUTTON_HEIGHT, Button); // Button 2


    drawSprite(150, 230, WATER_WIDTH, WATER_HEIGHT, WaterBlue);
    drawSpriteMirror(150 + WATER_WIDTH * 2, 230, WATER_WIDTH, WATER_HEIGHT, WaterBlue); // water


    drawSprite(215, 230, WATER_WIDTH, WATER_HEIGHT, WaterBlue, 1);
    drawSpriteMirror(215 + WATER_WIDTH * 2, 230, WATER_WIDTH, WATER_HEIGHT, WaterBlue, 1); // lava


    drawSprite(189, 180, WATER_WIDTH, WATER_HEIGHT, WaterBlue, 2);
    drawSpriteMirror(189 + WATER_WIDTH * 2, 180, WATER_WIDTH, WATER_HEIGHT, WaterBlue, 2); // shrek-cum


    drawSprite(55, 40, DOOR_WIDTH, DOOR_HEIGHT, DoorCorner, 2);
    drawSpriteMirror(55 + DOOR_WIDTH * 2, 40, DOOR_WIDTH, DOOR_HEIGHT, DoorCorner, 2); // DoorTop left

    drawSprite(55, 40 + DOOR_HEIGHT, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2);
    drawSpriteMirror(55 + DOOR_WIDTH * 2, 40 + DOOR_HEIGHT, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2); // DoorFrame left

    drawSprite(55, 40 + DOOR_HEIGHT * 2, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2);
    drawSpriteMirror(55 + DOOR_WIDTH * 2, 40 + DOOR_HEIGHT * 2, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2); // DoorFrame left


    drawSprite(80, 40, DOOR_WIDTH, DOOR_HEIGHT, DoorCorner, 2);
    drawSpriteMirror(80 + DOOR_WIDTH * 2, 40, DOOR_WIDTH, DOOR_HEIGHT, DoorCorner, 2); // DoorTop right

    drawSprite(80, 40 + DOOR_HEIGHT, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2);
    drawSpriteMirror(80 + DOOR_WIDTH * 2, 40 + DOOR_HEIGHT, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2); // DoorFrame right

    drawSprite(80, 40 + DOOR_HEIGHT * 2, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2);
    drawSpriteMirror(80 + DOOR_WIDTH * 2, 40 + DOOR_HEIGHT * 2, DOOR_WIDTH, DOOR_HEIGHT, DoorEdge, 2); // DoorFrame right


    drawSprite(61, 48, SIGN_WIDTH, SIGN_HEIGHT, SignBlue);
    drawSpriteMirror(61 + SIGN_WIDTH * 2, 48, SIGN_WIDTH, SIGN_HEIGHT, SignBlue); // Blue sign

    drawSprite(86, 48, SIGN_WIDTH, SIGN_HEIGHT, SignRed);
    drawSpriteMirror(86 + SIGN_WIDTH * 2, 48, SIGN_WIDTH, SIGN_HEIGHT, SignRed); // Red sign


    drawSprite(10, 112, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, 1);
    drawSprite(10 + PLATFORM_WIDTH * 2, 112, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformMiddle, 1);
    drawSpriteMirror(10 + PLATFORM_WIDTH * 4, 112, PLATFORM_WIDTH, PLATFORM_HEIGHT, PlatformEdge, 1); // Platform 2


    drawSprite(12, 12, DIA_WIDTH, DIA_HEIGHT, DiaBlue);
    drawSpriteMirror(12 + DIA_WIDTH * 2, 12, DIA_WIDTH, DIA_HEIGHT, DiaBlue); // Dia Blue 1


    drawSprite(164, 216, DIA_WIDTH, DIA_HEIGHT, DiaBlue);
    drawSpriteMirror(164 + DIA_WIDTH * 2, 216, DIA_WIDTH, DIA_HEIGHT, DiaBlue); // DiaBlue 2


    drawSprite(26, 12, DIA_WIDTH, DIA_HEIGHT, DiaRed, 1);
    drawSpriteMirror(26 + DIA_WIDTH * 2, 12, DIA_WIDTH, DIA_HEIGHT, DiaRed, 1); // DiaRed 1


    drawSprite(229, 216, DIA_WIDTH, DIA_HEIGHT, DiaRed, 1);
    drawSpriteMirror(229 + DIA_WIDTH * 2, 216, DIA_WIDTH, DIA_HEIGHT, DiaRed, 1); // DiaRed 2


    drawSprite(86, 148, LEVER_BASE_WIDTH, LEVER_BASE_HEIGHT, LeverBase, 1);
    drawSpriteMirror(86 + LEVER_BASE_WIDTH * 2, 148, LEVER_BASE_WIDTH, LEVER_BASE_HEIGHT, LeverBase, 1); // LeverBase


    drawSprite(94, 142, LEVER_TOP_WIDTH, LEVER_TOP_HEIGHT, LeverTop, 1); // LeverTop
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

    case 4: // 0100
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

    case 5: // 0101
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

    case 6: // 0110
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

    case 10: // 1010
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