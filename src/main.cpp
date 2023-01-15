#include <main.h> //Include the main header file

volatile int frameCounter = 0;

// Struct for Controllable player
struct
{
public:
    // X position op player1
    uint16_t x;
    // Y position op player1
    uint8_t y;
    // Old X position op player1
    uint16_t xOld = x;
    // Old Y position op player1
    uint8_t yOld = y;
    // Y velocity op player1
    int8_t yVelocity = 0;
    // Jumping state op player1
    bool jumping = false;
} player1;

// Struct for other player
struct
{
public:
    // X position op player2
    uint16_t x;
    // Y position op player2
    uint8_t y;
    // Old X position op player2
    uint16_t xOld = x;
    // Old Y position op player2
    uint8_t yOld = y;
} player2;

// Struct for Diamonds
struct Diamond
{
    // X position op diamond
    uint16_t x;
    // Y position op diamond
    uint8_t y;
    // Width op diamond
    uint8_t w;
    // Height op diamond
    uint8_t h;
    // Version op diamond
    uint8_t version;
    // Collected state op diamond
    bool collected;

    // Draw the diamond
    void draw()
    {
        // If x is null (diamond not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // Draw the diamond_sprite and the mirrored version
        drawSprite(x, y, DIA_WIDTH, DIA_HEIGHT, Diamond_Sprite, version);
        drawSpriteMirror(x + DIA_WIDTH * 2, y, DIA_WIDTH, DIA_HEIGHT, Diamond_Sprite, version);
    }

    bool isInitialized() const
    {
        return (x != 0);
    }
};

// Struct for Platforms
struct Platform
{
    // Bounds op platform in the form of a Rect object
    Rect bounds;
    // Minimum X position op platform
    uint16_t minX;
    // Maximum X position op platform
    uint16_t maxX;
    // Minimum Y position op platform
    uint8_t minY;
    // Maximum Y position op platform
    uint8_t maxY;
    // Version op platform (colour)
    uint8_t version;

    // Move the platform to the maximum position
    void MoveMax()
    {
        // If x is null (platform not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // If the platform is horizontal
        if (maxX == minX)
        {
            if (minY > maxY)
            {
                MoveMinY();
            }
            else
            {
                MoveMaxY();
            }
            // Move the platform to the maximum Y position
        }
        // If the platform is vertical
        else if (maxY == minY)
        {
            if (minX > maxX)
            {
                MoveMinX();
            }
            else
            {
                // Move the platform to the maximum X position
                MoveMaxX();
            }
        }
    }

    void MoveMin()
    {
        // If x is null (platform not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // If the platform is horizontal
        if (maxX == minX)
        {
            if (minY > maxY)
            {
                MoveMaxY();
            }
            else
            {
                // Move the platform to the minimum Y position
                MoveMinY();
            }
        }
        // If the platform is vertical
        else if (maxY == minY)
        {
            if (minY > maxY)
            {
                MoveMaxX();
            }
            else
            {
                // Move the platform to the minimum X position
                MoveMinX();
            }
        }
    }

    // Move the platform to the maximum X position
    void MoveMaxX()
    {
        // If the platform is not at the maximum X position and the frameCounter is a multiple of 3
        if (bounds.x > minX && frameCounter % 3 == 0)
        {
            // Clear the platform at the current position
            clearPlat(bounds.x + 3, bounds.y, bounds.x, bounds.y, bounds.width, bounds.height);
            // Move the platform to the maximum X position with a speed of 3
            bounds.x -= 3;
            this->draw();
        }
    }

    // Move the platform to the maximum Y position
    void MoveMaxY()
    {
        // If the platform is not at the maximum Y position and the frameCounter is a multiple of 3
        if (bounds.y < maxY && frameCounter % 3 == 0)
        {
            // Clear the platform at the current position
            clearPlat(bounds.x, bounds.y + 3, bounds.x, bounds.y, bounds.width, bounds.height);
            // Move the platform to the maximum Y position with a speed of 3
            bounds.y += 3;
            this->draw();
        }
    }

    // Move the platform to the minimum X position
    void MoveMinX()
    {
        // If the platform is not at the minimum X position and the frameCounter is a multiple of 3
        if (bounds.x > minX && frameCounter % 3 == 0)
        {
            // Clear the platform at the current position
            clearPlat(bounds.x + 3, bounds.y, bounds.x, bounds.y, bounds.width, bounds.height);
            // Move the platform to the minimum X position with a speed of 3
            bounds.x -= 3;
            this->draw();
        }
    }

    // Move the platform to the minimum Y position
    void MoveMinY()
    {
        // If the platform is not at the minimum Y position and the frameCounter is a multiple of 3
        if (bounds.y > minY && frameCounter % 3 == 0)
        {
            // Clear the platform at the current position
            clearPlat(bounds.x, bounds.y - 3, bounds.x, bounds.y, bounds.width, bounds.height);
            // Move the platform to the minimum Y position with a speed of 3
            bounds.y -= 3;
            this->draw();
        }
    }

    // Check if the platform is at the maximum position
    bool isMax()
    {
        // If x is null (platform not initialized) return (don't continue)
        if (!isInitialized())
            return true;
        // If the platform's bounds are directly at the maximum position or the minimum position
        if ((bounds.x >= maxX && bounds.y >= maxY) || (bounds.x <= minX && bounds.y <= minY))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // Draw the platform
    void draw()
    {
        // If x is null (platform not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // If the platform is horizontal
        if (maxX == minX)
        {
            // Draw the platform horizontally
            drawPlatH(bounds.x, bounds.y, bounds.width, bounds.height, version);
        }
        // If the platform is vertical
        else if (maxY == minY)
        {
            // Draw the platform vertically
            drawPlatV(bounds.x, bounds.y, bounds.width, bounds.height, version);
        }
    }

    bool isInitialized()
    {
        return (bounds.x != 0);
    }
};

// Struct for Levers
struct lever
{
    // Bounds of the lever in the form of a Rect object
    Rect bounds;
    // State of the lever (true = on, false = off)
    bool state;
    // State of player standing on the lever (true = on, false = off)
    bool standingOn;
    // Pointer to the platforms that the lever controls
    Platform *plat[2];
    // Version of the lever (colour)
    uint8_t Version;

    // Activate the lever
    void activate()
    {
        // If x is null (lever not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // If the lever is on
        if (state)
        {
            // Move platform
            plat[0]->MoveMax();
            if (plat[1] != nullptr)
                plat[1]->MoveMax();
        }
        else
        {
            // Move platform back
            plat[0]->MoveMin();
            if (plat[1] != nullptr)
                plat[1]->MoveMin();
        }
        // If controlled platform is not at the maximum position
        if (!(plat[0]->isMax()))
        {
            // Draw the lever again
            this->draw();
        }
    }

    // Draw the lever
    void draw()
    {
        // If x is null (lever not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // Draw the leverBase
        drawLever(bounds.x - 7, bounds.y + 1);
        // If the lever is on
        if (state)
        {
            // Draw the leverTop to the right
            drawSpriteMirror((bounds).x - 7, (bounds).y - 7, LEVER_TOP_WIDTH, LEVER_TOP_HEIGHT, LeverTop, Version);
        }
        else
        {
            // Draw the leverTop to the left
            drawSprite((bounds).x + 1, (bounds).y - 7, LEVER_TOP_WIDTH, LEVER_TOP_HEIGHT, LeverTop, Version);
        }
    }

    bool isInitialized()
    {
        return (bounds.x != 0);
    }
};

// Struct for Buttons
struct button
{
    // Bounds of the button in the form of a Rect object
    Rect bounds;
    // State of player standing on the button (true = on, false = off)
    bool standingOn;
    // Pointer to the platforms that the button controls
    Platform *plat[2];
    // Version of the button (colour)
    uint8_t Version;
    // Pointer to the button that is connected to this button
    button *connectedButton;

    // Activate the button
    void activate()
    {
        // If x is null (button not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // If player is standing on the button or player is standing on the connected button
        if (standingOn || connectedButton->standingOn)
        {
            // Move platform
            plat[0]->MoveMax();
            plat[1]->MoveMax();
        }
        else
        {
            // Move platform back
            plat[0]->MoveMin();
            plat[1]->MoveMin();
        }
        // If controlled platform is not at the maximum position or player is standing on the button or player is standing on the connected button
        if (!(plat[0]->isMax()) || standingOn || connectedButton->standingOn)
        {
            // Draw the button
            this->draw();
        }
    }

    // Draw the button
    void draw()
    {
        // If x is null (button not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // Draw the button
        drawSprite((bounds).x - 4, (bounds).y - 3, BUTTON_WIDTH, BUTTON_HEIGHT, Button, Version);
        drawSpriteMirror((bounds).x - 4 + BUTTON_WIDTH * 2, (bounds).y - 3, BUTTON_WIDTH, BUTTON_HEIGHT, Button, Version);
    }

    // Function for setting the connected button
    void setConnectedButton(button *btn)
    {
        this->connectedButton = btn;
    }

    bool isInitialized()
    {
        return (bounds.x != 0);
    }
};

// Struct for Liquids
struct Liquid
{
    // Bounds of the liquid in the form of a Rect object
    Rect bounds;
    // Version of the liquid (colour)
    uint8_t Version;

    // Draw the liquid
    void draw()
    {
        // If x is null (liquid not initialized) return (don't draw)
        if (!isInitialized())
            return;
        // Draw the liquid
        drawLiquid(bounds.x - 5, bounds.y, bounds.width + 10, bounds.height, Version);
    }

    bool isInitialized()
    {
        return (bounds.x != 0);
    }
};

// Level objects
Platform Platform1, Platform2, Platform3, Platform4, Platform5, Platform6, Platform7, Platform8, Platform9;
lever Lever1, Lever2, Lever3, Lever4, Lever5;
button button1, button2, button3;
Diamond Dia1, Dia2, Dia3, Dia4, Dia5, Dia6;
Liquid liq1, liq2, liq3, liq4;
Rect Door1, Door2;

// Arrays to store the wall objects
Rect walls[20];

// Arrays to store the objects (mainly for looping purposes)
// Platform array
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
// Lever array
lever *levers[] =
    {
        &Lever1,
        &Lever2,
        &Lever3,
        &Lever4,
        &Lever5};
// Button array
button *buttons[] =
    {
        &button1,
        &button2,
        &button3};
// Diamond array
Diamond *Dias[] =
    {
        &Dia1,
        &Dia2,
        &Dia3,
        &Dia4};
// Liquid array
Liquid *liquids[] =
    {
        &liq1,
        &liq2,
        &liq3,
        &liq4};
// Lives data array
const uint8_t liveCount[] = {
    // Patterns for all the numbers and letters on the 7-segment display
    0x3F, // 0
    0x6,  // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D  // 5
};

// IR sending variables
// For checking to see if the current bit is done sending
bool dataIsSend = false;
// Data to send over IR
uint16_t sendingData = 0;
// Keeps track of the variable send
uint8_t sendingPackageNumber = 0;
uint8_t packageLength = 3;
// The data bit to send;
int8_t sendingBit = SENDINGBIT_START_VALUE;
// Controls the on and off time of the IR LED
uint16_t onTime = 0;

// Time variables
// Controls the time between next frequentie pulse
uint8_t msTime;
// Keeps track of the milliseconds passed since startup
volatile uint32_t currentMs = 0;
// Keeps track of the milliseconds passed and resets every frame
volatile uint8_t intCurrentMs = 0;

// Time lengths for IR data sending
// Length of startbit
uint16_t startTime;
// Length of 0
uint8_t zeroTime;
// Length of 1
uint8_t oneTime;
// Length of pause between bits
uint8_t offTime;

// IR receiving variables
// Used to determin the length of each received bit
uint32_t startMs = 0;
// Tracks if the staring bit was received before accepting data
bool startBitReceived = false;
// All the received bits
uint16_t receivedData = 0;
// Header of the incoming data
uint8_t packageHeader = 0;
// Lenght of the incoming package
uint8_t receivingPackageLength = 0;
// Current bit (used for bitshifting 1's in to receivedData)
uint8_t bitCounter = 0;
// Differentiates data from pauses
bool isDataBit = false;

// Other variables
uint8_t lives = MAX_LIVES;
uint8_t score = START_SCORE;
uint8_t level = 1;
uint8_t currentlyPlayingLevel = 0;
uint8_t currentlyPlayingLevelReceived = 0;
uint8_t level2Unlocked = 0;
uint8_t level3Unlocked = 0;
uint8_t player_accel = 0;
bool levelCompleted = false;
bool playerDied = false;

// All the gamestates
// All the gamestates
enum gameState
{
    MENU,
    GAME,
    LEVEL_SELECT_SCREEN,
    SETTINGS,
    PAUSE,
    GAMEOVER,
    PLAYER_SELECT_SCREEN,
    HIGHSCORE_SCREEN
} currentGameState;

// IR receiving protocol
ISR(PCINT2_vect)
{
    // Check for the pin state (high or low)
    isDataBit = ((PIND >> PIND2) & 1) != 0;

    if (isDataBit)
    {
        // Calculate the length to determin the value
        uint8_t difference = currentMs - startMs;

        // If the start bit has been send, check what the data is
        if (startBitReceived)
        {
            if (bitCounter < 3)
            {
                // Check if it's a 0
                if (difference < ZERO_MAX)
                {
                    packageHeader &= ~(1 << bitCounter++);
                }
                // Else it's a 1
                else
                {
                    packageHeader |= (1 << bitCounter++);
                }
            }

            // Check if the full header is received
            if (bitCounter == PACKAGE_HEADER_LENGTH)
            {
                // Set the package length based on the header
                switch (packageHeader)
                {
                case PACKAGE1_HEADER:
                    receivingPackageLength = PACKAGE1_LENGTH;
                    break;
                case PACKAGE2_HEADER:
                    receivingPackageLength = PACKAGE2_LENGTH;
                    break;
                case PACKAGE3_HEADER:
                    receivingPackageLength = PACKAGE3_LENGTH;
                    break;
                case PACKAGE4_HEADER:
                    receivingPackageLength = PACKAGE4_LENGTH;
                    break;
                case PACKAGE5_HEADER:
                    receivingPackageLength = PACKAGE5_LENGTH;
                    break;
                }
            }

            // Check what the data is (header already received)
            if (bitCounter >= 3)
            {
                // Check if its a zero
                if (difference < ZERO_MAX)
                {
                    receivedData &= ~(1 << (bitCounter - PACKAGE_HEADER_LENGTH - 1));
                }
                // Else its a one
                else
                {
                    receivedData |= (1 << (bitCounter - PACKAGE_HEADER_LENGTH - 1));
                }

                // If all bits are send read the data
                if (bitCounter == receivingPackageLength)
                {
                    switch (packageHeader)
                    {
                    case PACKAGE1_HEADER:
                        // If the other player died, kill (reset the level) for this player aswell
                        playerDied |= receivedData;
                        break;
                    case PACKAGE2_HEADER:
                        // Extra scope to prevent variable usage in other cases
                        {
                            // Keep the lives synchronizes
                            uint8_t receivedLives = receivedData;

                            if (receivedLives < lives)
                            {
                                lives = receivedLives;
                            }
                            break;
                        }
                    case PACKAGE3_HEADER:
                        // The level that the other player is playing
                        currentlyPlayingLevelReceived = receivedData;
                        break;
                    case PACKAGE4_HEADER:
                        // Other player x position
                        player2.xOld = player2.x;
                        player2.x = receivedData;
                        break;
                    case PACKAGE5_HEADER:
                        // Other player y position
                        player2.yOld = player2.y;
                        player2.y = receivedData;
                        break;
                    }

                    // Reset for the next package
                    startBitReceived = false;
                }
                // Update the bit counter
                bitCounter++;
            }
        }

        // Check if it's a start bit
        if (difference > STARTBIT_MIN && difference < STARTBIT_MAX)
        {
            startBitReceived = true;
            bitCounter = 0;
            receivedData = 0;
        }
    }
    // Save the time from startup to now
    startMs = currentMs;
}

// IR sending protocol (with timer to keep track of ms)
ISR(TIMER0_COMPA_vect)
{
    // Counters to keep track of time
    static uint16_t counter = 0;
    static uint8_t msCounter = 0;

    // After enough interupts, a ms has passed
    if (++msCounter > msTime)
    {
        currentMs++;
        intCurrentMs++;
        msCounter = 0;
    }

    // Check if the bit is done sending
    if (++counter > onTime)
    {
        // If data was send, wait before sending the next bit
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
            // Check if the package is done sending
            // Send the next bit
            if (++sendingBit < packageLength)
            {
                // Send start bit
                if (sendingBit == STARTBIT_VALUE)
                {
                    onTime = startTime;
                }
                // Send data
                else
                {
                    // Set the time corresponding to the bit
                    onTime = ((sendingData >> sendingBit) & 1) ? oneTime : zeroTime;
                }
                // Enable TC0
                TCCR0A &= ~(1 << COM0A1);
            }
            // Set everything up for the next package
            else
            {
                sendingBit = SENDINGBIT_START_VALUE;

                // If the last package is send, restart at the first one
                if (++sendingPackageNumber > 5)
                {
                    sendingPackageNumber = 1;
                }

                // Set the package data and length and put the headerbits at the front
                switch (sendingPackageNumber)
                {
                case 1:
                    // Send if the player died
                    sendingData = (PACKAGE1_HEADER | (playerDied << PACKAGE_HEADER_LENGTH));
                    packageLength = PACKAGE1_LENGTH;
                    break;
                case 2:
                    // Send the number of lives
                    sendingData = (PACKAGE2_HEADER | (getLives() << PACKAGE_HEADER_LENGTH));
                    packageLength = PACKAGE2_LENGTH;
                    break;
                case 3:
                    // Send the level currently playing (0 is menu)
                    sendingData = (PACKAGE3_HEADER | (currentlyPlayingLevel << PACKAGE_HEADER_LENGTH));
                    packageLength = PACKAGE3_LENGTH;
                    break;
                case 4:
                    // Send the x position
                    sendingData = (PACKAGE4_HEADER | (player1.x << PACKAGE_HEADER_LENGTH));
                    packageLength = PACKAGE4_LENGTH;
                    break;
                case 5:
                    // Send the y position
                    sendingData = (PACKAGE5_HEADER | (player1.y << PACKAGE_HEADER_LENGTH));
                    packageLength = PACKAGE5_LENGTH;
                    break;
                default:
                    sendingData = 0;
                    packageLength = EMPTY_PACKAGE_LENGTH;
                    break;
                }
            }
        }

        // Flip between sending a bit and waiting
        dataIsSend = !dataIsSend;
        counter = 0;
    }
}

// Main Function
int main(void)
{
    // If errors are gotten at startup, uncomment this code to clear EEPROM (Mainly for testing purposes)
    EEPROM_clear_entire_mem();

    // Setup IR led
    // Set pin D6 as output
    DDRD |= (1 << DDD6);
    // Initialise timer 0
    initTimer0();

    // Setup IR recieving
    // Set pin D2 on pull up mode
    PORTD |= (1 << PORTD2);
    // Enable pin change interupts
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18);

    // Setup wire
    Wire.begin();

    // Start the SPI communication and send screen startup commands
    setupSPI();
    START_UP();

    // Start with 5 lives
    EEPROM_update(LIVES_ADDR, 5);

    // Always set a start frequency so the game loop can run (because ms timer is in ISR of IR sending)
    setFreq(IR_38KHZ);

    // Check in the eeprom if the frequency has already been set
    {
        if (getFreq() == IR_38KHZ || getFreq() == IR_56KHZ)
        {
            setFreq(EEPROM_read(20));
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

    // If the nunchuk is not found, show an animated message on the screen.
    if (!startNunchuk(NUNCHUK_ADDRESS))
    {
        // Make screen black
        fillScreen(0);
        // Stay looping until the nunchuk is found
        while (!startNunchuk(NUNCHUK_ADDRESS))
        {
            drawString("Nunchuk", 60, 80, 5, PLAYER_RED);
            drawString("not", 60, 120, 5, PLAYER_RED);
            drawString("found", 60, 160, 5, PLAYER_RED);

            drawString("Nunchuk", 60, 80, 5, PLAYER_YELLOW);
            drawString("not", 60, 120, 5, PLAYER_YELLOW);
            drawString("found", 60, 160, 5, PLAYER_YELLOW);
        }

        // Draw either the menu or player select screen
        switch (currentGameState)
        {
        case MENU:
            drawMenu();
            break;
        case PLAYER_SELECT_SCREEN:
            drawPlayerSelectScreen();
            break;
        case LEVEL_SELECT_SCREEN:
        case GAME:
        case SETTINGS:
        case GAMEOVER:
        case PAUSE:
            currentGameState = MENU;
            drawMenu();
            break;
        }
    }

    uint8_t currentHighlightedButton;
    bool normalState = true;
    // Loop forever
    while (true)
    {
        // If the gametime is more than the FRAME_TIME
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
                    // Draws over the previous score.
                    drawScore(score, true);
                    score--;
                    drawScore(score, false);
                    showLives(lives);
                }

                // Game code
                update();
                drawPlayers();
                // If level is completed update the highscore and set score to START_SCORE, also add 1 to level.
                if (levelCompleted)
                {
                    updateHighscore(score, level);
                    levelCompleted = false;
                    score = START_SCORE;
                    level++;
                }
                if (state.z_button && !state.z_button_old)
                {
                    currentHighlightedButton = 0;
                    currentGameState = LEVEL_SELECT_SCREEN;
                    drawLevelSelectScreen();
                }
            }
            // Menu code
            else if (currentGameState == MENU)
            {
                // Highlight the selected option
                // Go up
                if (state.joy_y_axis > 156 && normalState)
                {
                    normalState = false;
                    if (currentHighlightedButton > 0)
                    {
                        currentHighlightedButton--;
                    }
                }
                // Go down
                else if (state.joy_y_axis < 100 && normalState)
                {
                    normalState = false;
                    if (currentHighlightedButton < 2)
                    {
                        currentHighlightedButton++;
                    }
                }
                else
                {
                    normalState = true;
                }

                // Draw a border around the selected option
                if (currentHighlightedButton == 0)
                {
                    drawBorder(265, 5, 50, 50, 5, WHITE);           // H button
                    drawBorder(110, 50, 116, 50, 5, PLAYER_ORANGE); // Play button
                }
                else if (currentHighlightedButton == 1)
                {
                    drawBorder(265, 5, 50, 50, 5, PLAYER_ORANGE);   // H button
                    drawBorder(110, 50, 116, 50, 5, WHITE);         // Play button
                    drawBorder(65, 130, 212, 50, 5, PLAYER_ORANGE); // Settings button
                }
                else if (currentHighlightedButton == 2)
                {
                    drawBorder(65, 130, 212, 50, 5, WHITE);         // Settings button
                    drawBorder(110, 50, 116, 50, 5, PLAYER_ORANGE); // Play button
                }

                // Checks if button is pressed
                if (state.c_button && !state.c_button_old)
                {
                    if (currentHighlightedButton == 0)
                    {
                        currentHighlightedButton = 0;
                        currentGameState = HIGHSCORE_SCREEN;
                        drawHighScoreMenu();
                    }
                    else if (currentHighlightedButton == 1)
                    {
                        currentHighlightedButton = 0;
                        currentGameState = LEVEL_SELECT_SCREEN;
                        drawLevelSelectScreen();
                    }
                    else if (currentHighlightedButton == 2)
                    {
                        currentHighlightedButton = 0;
                        currentGameState = PLAYER_SELECT_SCREEN;
                        drawPlayerSelectScreen();
                    }
                }
            }
            // Level select menu
            else if (currentGameState == LEVEL_SELECT_SCREEN)
            {
                // Only allow player 1 to start a level, player 2 atou loads the same level
                if (currentlyPlayingLevel != currentlyPlayingLevelReceived && getFreq() == IR_38KHZ)
                {
                    currentGameState = GAME;
                    currentlyPlayingLevel = currentlyPlayingLevelReceived;

                    // Check what level player started, start the same level
                    switch (currentlyPlayingLevelReceived)
                    {
                    case 1:
                        level = 1;
                        level1();
                        break;
                    case 2:
                        level = 2;
                        level2();
                        break;
                    case 3:
                        level = 3;
                        level3();
                        break;
                    default:
                        currentHighlightedButton = 1;
                        currentGameState = MENU;
                        break;
                    }
                }
                else
                {
                    // Highlight the selected level
                    // Go up
                    if (state.joy_y_axis > 156 && normalState)
                    {
                        normalState = false;
                        if (currentHighlightedButton < 4)
                        {
                            currentHighlightedButton++;
                        }
                    }
                    // Go down
                    else if (state.joy_y_axis < 100 && normalState)
                    {
                        normalState = false;
                        if (currentHighlightedButton > 0)
                        {
                            currentHighlightedButton--;
                        }
                    }
                    else
                    {
                        normalState = true;
                    }

                    // Draw a border around the selected level
                    switch (currentHighlightedButton)
                    {
                    case 0:
                        drawBorder(45, 191, 229, 42, 5, WHITE); // Level 1 button
                        if (level2Unlocked)
                        {
                            drawBorder(45, 131, 229, 42, 5, PLAYER_BLUE); // Level 2 button
                        }
                        else
                        {
                            drawBorder(45, 131, 229, 42, 5, PLAYER_RED); // Level 2 button
                        }
                        break;
                    case 1:
                        drawBorder(45, 131, 229, 42, 5, WHITE); // Level 2 button
                        if (level3Unlocked)
                        {
                            drawBorder(45, 70, 229, 42, 5, PLAYER_BLUE); // Level 3 button
                        }
                        else
                        {
                            drawBorder(45, 70, 229, 42, 5, PLAYER_RED); // Level 3 button
                        }
                        drawBorder(45, 191, 229, 42, 5, PLAYER_BLUE); // Level 1 button
                        break;
                    case 2:
                        drawBorder(45, 70, 229, 42, 5, WHITE); // Level 3 button
                        if (level2Unlocked)
                        {
                            drawBorder(45, 131, 229, 42, 5, PLAYER_BLUE); // Level 2 button
                        }
                        else
                        {
                            drawBorder(45, 131, 229, 42, 5, PLAYER_RED); // Level 2 button
                        }

                        drawBorder(14, 0, 294, 49, 5, PLAYER_BLUE); // Exit button
                        break;
                    case 3:
                        drawBorder(14, 0, 294, 49, 5, WHITE); // Exit button
                        if (level3Unlocked)
                        {
                            drawBorder(45, 70, 229, 42, 5, PLAYER_BLUE); // Level 3 button
                        }
                        else
                        {
                            drawBorder(45, 70, 229, 42, 5, PLAYER_RED); // Level 3 button
                        }

                        break;
                    default:
                        // Code to be executed if currentHighlightedButton is not equal to any of the above cases
                        break;
                    }

                    // Checks if button was pressed and you are player 1
                    if (state.c_button && !state.c_button_old)
                    {
                        // Start the selected level
                        switch (currentHighlightedButton)
                        {
                        case 0:
                            if (getFreq() == IR_56KHZ)
                            {
                                currentGameState = GAME;
                                level = 1;
                                level1();
                            }
                            break;
                        case 1:
                            if (level2Unlocked && getFreq() == IR_56KHZ)
                            {
                                currentGameState = GAME;
                                level = 2;
                                level2();
                            }
                            break;
                        case 2:
                            if (level3Unlocked && getFreq() == IR_56KHZ)
                            {
                                currentGameState = GAME;
                                level = 3;
                                level3();
                            }
                            break;
                        case 3: // Exit button
                            currentGameState = MENU;
                            drawMenu();
                            break;
                        default:
                            currentHighlightedButton = 1;
                            currentGameState = MENU;
                            drawMenu();
                            break;
                        }
                    }
                }

                // Go back to the menu
                if (state.z_button && !state.z_button_old)
                {
                    currentHighlightedButton = 1;
                    currentGameState = MENU;
                    drawMenu();
                    currentlyPlayingLevel = 0;
                }
            }
            // Draw the player select screen
            else if (currentGameState == PLAYER_SELECT_SCREEN)
            {
                // Checks if the joystick is pushed up
                if (state.joy_y_axis > 156)
                {
                    currentHighlightedButton = 0;
                }
                // Checks if the joystick is pushed down
                else if (state.joy_y_axis < 100)
                {
                    currentHighlightedButton = 1;
                }

                // Draw border around the highlighted button
                if (currentHighlightedButton == 0)
                {
                    drawBorder(65, 50, 212, 50, 5, WHITE);          // Player1 button
                    drawBorder(65, 130, 212, 50, 5, PLAYER_ORANGE); // Player2 button
                }
                else if (currentHighlightedButton == 1)
                {
                    drawBorder(65, 130, 212, 50, 5, WHITE);        // Player2 button
                    drawBorder(65, 50, 212, 50, 5, PLAYER_ORANGE); // Player1 button
                }

                // Go back to the menu
                if (state.z_button && !state.z_button_old)
                {
                    currentHighlightedButton = 1;
                    currentGameState = MENU;
                    drawMenu();
                }
                // Checks if button is pressed
                if (state.c_button && !state.c_button_old)
                {
                    // Set the frequency for the IR sending
                    if (currentHighlightedButton == 0)
                    {
                        currentHighlightedButton = 1;
                        EEPROM_write(20, IR_56KHZ);
                        setFreq(IR_56KHZ);
                        currentGameState = MENU;
                        drawMenu();
                    }
                    else
                    {
                        currentHighlightedButton = 1;
                        EEPROM_write(20, IR_38KHZ);
                        setFreq(IR_38KHZ);
                        currentGameState = MENU;
                        drawMenu();
                    }
                }
            }
            // Draw the highscored
            else if (currentGameState == HIGHSCORE_SCREEN)
            {
                // Go back to the menu
                if ((state.z_button && !state.z_button_old) || (state.c_button && !state.c_button_old))
                {
                    currentHighlightedButton = 1;
                    currentGameState = MENU;
                    drawMenu();
                }
            }
        }
    }
}

// Initialize timer 0
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

// Set frequency of IR LED
void setFreq(uint8_t freq)
{
    // Set frequency
    OCR0A = freq;

    // If frequency is 38kHz
    if (freq == IR_38KHZ)
    {
        // Set times
        msTime = 37;
        startTime = 189;
        zeroTime = 37;
        oneTime = 113;
        offTime = 37;
    }
    else
    {
        // Set times for 56KHz
        msTime = 55;
        startTime = 279;
        zeroTime = 55;
        oneTime = 167;
        offTime = 55;
    }
}

// Get frequency of IR receiver
uint8_t getFreq()
{
    return EEPROM_read(20);
}

// Main Update function
void update()
{
    // Update player 1's old position
    player1.xOld = player1.x;
    player1.yOld = player1.y;
    // Update player 1's y velocity
    player1.y += player1.yVelocity;
    player1.yVelocity += GRAVITY;

    if (player_accel >= PLAYER_MAX_ACCEL)
    {
        player_accel = PLAYER_MAX_ACCEL;
    }
    else
    {
        player_accel += PLAYER_ACCEL;
    }

    // Check for movement to right (only move when not against the wall)
    if (state.joy_x_axis > 140 && player1.x + PLAYER_ACTUAL_WIDTH < SCREEN_WIDTH)
    {
        player1.x += player_accel;
    }
    // Check for movement to left (only move when not against the wall)
    else if (state.joy_x_axis < 100 && player1.x > 0)
    {
        player1.x -= player_accel;
    }

    checkPoolCollision();
    checkWallCollision();
    checkPlatformCollision();
    checkButtons();
    checkLevers();
    checkDias();
    checkFinish();

    // Jumping and falling mechanics
    if (state.c_button && !player1.jumping)
    {
        player1.jumping = true;
        player1.yVelocity -= INITIAL_Y_VEL;
    }

    // Check if the player hasn't moved, if so, reset the acceleration.
    if (player1.xOld == player1.x)
    {
        player_accel = 0;
    }
    // Check if player is dead.
    if (playerDied)
    {
        lives--;
        setLives(lives);
        // Show lives on 7 segments display.
        showLives(getLives());
        // Reset all player positions and draw the interactables.
        clearWholeSprite(player1.xOld, player1.yOld, PLAYER_WIDTH, PLAYER_HEIGHT);

        // Reset player positions
        setPlayerPos(level);

        playerDied = false;

        // Reset score and redraw it.
        drawScore(score, true);
        score = START_SCORE;
        drawScore(score, false);

        // Reset framecounter
        frameCounter = 0;

        drawInteractables();
    }
}

// Function to check for collision with buttons and to activate them
void checkButtons()
{
    // for every button in the buttons array
    for (button *B : buttons)
    {
        // Check if player 1 or player 2 collides with the button
        if (rectangleCollision(player1.x, player1.y, B->bounds) || rectangleCollision(player2.x, player2.y, B->bounds))
        {
            // Change the state of the to true
            B->standingOn = true;
        }
        else
        {
            // Change the state of the to false
            B->standingOn = false;
        }
        // Always try to activate the button
        B->activate();
    }
}

// Function to check for collision with levers and to activate them
void checkLevers()
{
    // For every lever in the lever array
    for (lever *L : levers)
    {
        // If player 1 or player 2 collides with the lever
        if (rectangleCollision(player1.x, player1.y, L->bounds) || rectangleCollision(player2.x, player2.y, L->bounds))
        {
            // If nobody was standing on the lever before
            if (!L->standingOn)
            {
                // Change the state of the lever and set the state of the player standing on top to true
                L->state = !L->state;
                L->standingOn = true;
            }
        }
        // If somebody was standing on the lever before, but not anymore
        else if (L->standingOn)
        {
            // Change the state of the player standing on top to false
            L->standingOn = false;
        }
        // Always try to activate the lever
        L->activate();
    }
}

// Function to check for collision with diamonds and to collect them
void checkDias()
{
    // For every diamond in the diamond array
    for (Diamond *D : Dias)
    {
        // Make a temporary rectangle to check for collision
        Rect temp = {D->x, D->y, D->w, D->h};
        if (getFreq() == IR_56KHZ)
        {
            // If player 1 collides with the correct diamond
            if (rectangleCollision(player1.x, player1.y, temp) && D->version == 0 && !D->collected)
            {
                // COLLECT DIAMOND
                // Update score
                drawScore(score, true);
                score += DIAMOND_POINTS;
                drawScore(score, false);
                // Set diamond to collected
                D->collected = true;
            }
            // If player 2 collides with the wrong diamond
            else if (rectangleCollision(player2.x, player2.y, temp) && D->version == 0 && !D->collected)
            {
                // Draw it again
                D->draw();
            }
            // If player 1 collides with the wrong diamond
            else if (rectangleCollision(player1.x, player1.y, temp) && D->version == 1 && !D->collected)
            {
                // wrong diamond so draw it again
                D->draw();
            }
            // If player 2 collides with the correct diamond
            else if (rectangleCollision(player2.x, player2.y, temp) && D->version == 1 && !D->collected)
            {
                // COLLECT DIAMOND
                // Update score
                drawScore(score, true);
                score += DIAMOND_POINTS;
                drawScore(score, false);
                // Set diamond to collected
                D->collected = true;
            }
        }
        else
        {
            // If player 1 collides with the correct diamond
            if (rectangleCollision(player1.x, player1.y, temp) && D->version == 1 && !D->collected)
            {
                // COLLECT DIAMOND
                // Update score
                drawScore(score, true);
                score += DIAMOND_POINTS;
                drawScore(score, false);
                // Set diamond to collected
                D->collected = true;
            }
            // If player 2 collides with the wrong diamond
            else if (rectangleCollision(player2.x, player2.y, temp) && D->version == 1 && !D->collected)
            {
                // Draw it again
                D->draw();
            }
            // If player 1 collides with the wrong diamond
            else if (rectangleCollision(player1.x, player1.y, temp) && D->version == 0 && !D->collected)
            {
                // Wrong diamond so draw it again
                D->draw();
            }
            // If player 2 collides with the correct diamond
            else if (rectangleCollision(player2.x, player2.y, temp) && D->version == 0 && !D->collected)
            {
                // COLLECT DIAMOND
                // Update score
                drawScore(score, true);
                score += DIAMOND_POINTS;
                drawScore(score, false);
                // Set diamond to collected
                D->collected = true;
            }
        }
    }
}

// Function to check for collision with finishdoors and to finish the level
void checkFinish()
{
    if (getFreq() == IR_56KHZ)
    {
        if (rectangleCollision(player1.x, player1.y, Door1) && rectangleCollision(player2.x, player2.y, Door2))
        {
            // Unlock next level
            if (level == 1)
            {
                level2Unlocked = true;
            }
            else if (level == 2)
            {
                level3Unlocked = true;
            }
            // Update highscore
            updateHighscore(score, level);
            // Set the current game state to the level select screen
            currentGameState = LEVEL_SELECT_SCREEN;
            // Draw the level select screen
            drawLevelSelectScreen();
        }
    }
    else
    {
        if (rectangleCollision(player1.x, player1.y, Door2) && rectangleCollision(player2.x, player2.y, Door1))
        {
            // unlock next level
            if (level == 1)
            {
                level2Unlocked = true;
            }
            else if (level == 2)
            {
                level3Unlocked = true;
            }
            // Update highscore
            updateHighscore(score, level);
            // Set the current game state to the level select screen
            currentGameState = LEVEL_SELECT_SCREEN;
            // Draw the level select screen
            drawLevelSelectScreen();
        }
    }
}

// Function to check for collision with platforms and draw them again
void checkPlatformCollision()
{
    // For every platform in the platform array
    for (Platform *platform : platforms)
    {
        // If player1 collides with platform
        if (rectangleCollision(player1.x, player1.y, platform->bounds))
        {
            // If the platform is horizontal
            if (platform->maxX == platform->minX)
            {
                // Draw platform
                drawPlatH(platform->bounds.x, platform->bounds.y, platform->bounds.width, platform->bounds.height, platform->version);
            }
            // If the platform is vertical
            else if (platform->maxY == platform->minY)
            {
                // draw platform
                drawPlatV(platform->bounds.x, platform->bounds.y, platform->bounds.width, platform->bounds.height, platform->version);
            }
        }
        // Stop player
        checkCollision(platform->bounds);
    }
}

// Function to check for collision with walls
void checkWallCollision()
{
    // For every wall in the walls array
    for (auto wall : walls)
    {
        checkCollision(wall);
    }
}

// Function to check for collision with a rectangle and update the player position accordingly
void checkCollision(Rect &bounds)
{
    // Check if the player is colliding with the wall.
    if (rectangleCollision(player1.x, player1.y, bounds))
    {
        if (player1.xOld + PLAYER_ACTUAL_WIDTH <= bounds.x)
        {
            // Check if the player is colliding with the wall from the left
            player1.x = bounds.x - PLAYER_ACTUAL_WIDTH;
        }
        else if (player1.xOld >= bounds.x + bounds.width)
        {
            // Check if the player is colliding with the wall from the right
            player1.x = bounds.x + bounds.width;
        }
        // Check if the player is colliding with the wall from the top
        else if (player1.yOld + PLAYER_HEIGHT <= bounds.y || player1.y < bounds.y)
        {
            player1.y = bounds.y - PLAYER_HEIGHT;
            player1.yVelocity = 0;
            player1.jumping = false;
        }
        else if (player1.yOld >= bounds.y + bounds.height)
        {
            // Check if the player is colliding with the wall from the bottom
            player1.y = bounds.y + bounds.height;
            player1.yVelocity = 0;
        }
    }
}

// Function to check for collision with pools
void checkPoolCollision()
{
    // For every liquid in the array
    for (auto &liquid : liquids)
    {
        if (getFreq() == IR_56KHZ)
        {
            // If player1 is colliding with the pool
            if (rectangleCollision(player1.x, player1.y, liquid->bounds))
            {
                // If player1 is not in the right pool
                if (player1.y < liquid->bounds.y && !(liquid->Version == 0))
                {
                    // Set playerDied to true
                    playerDied = true;
                }

                // Else if player2 is colliding with the pool
            }
            else if (rectangleCollision(player2.x, player2.y, liquid->bounds))
            {
                // If player2 is not in the right pool
                if (player2.y < liquid->bounds.y && !(liquid->Version == 1))
                {
                    // Set playerDied to true
                    playerDied = true;
                }
            }
        }
        else
        {
            // If player1 is colliding with the pool
            if (rectangleCollision(player1.x, player1.y, liquid->bounds))
            {
                // If player1 is not in the right pool
                if (player1.y < liquid->bounds.y && !(liquid->Version == 1))
                {
                    // Set playerDied to true
                    playerDied = true;
                }

                // Else if player2 is colliding with the pool
            }
            else if (rectangleCollision(player2.x, player2.y, liquid->bounds))
            {
                // If player2 is not in the right pool
                if (player2.y < liquid->bounds.y && !(liquid->Version == 0))
                {
                    // Set playerDied to true
                    playerDied = true;
                }
            }
        }
    }
}

// Function to set the player position correctly
void setPlayerPos(uint8_t currentLevel)
{
    if (currentLevel == 1)
    {
        if (getFreq() == IR_56KHZ)
        {
            // player 1
            player1.x = PLAYER_ONE_X_LVL_ONE;
            player1.xOld = player1.x;
            player1.y = PLAYER_ONE_Y_LVL_ONE;
            player1.yOld = player1.y;
            // player 2
            player2.x = PLAYER_TWO_X_LVL_ONE;
            player2.xOld = player2.x;
            player2.y = PLAYER_TWO_Y_LVL_ONE;
            player2.yOld = player2.y;
        }
        else
        {
            // player 1
            player1.x = PLAYER_TWO_X_LVL_ONE;
            player1.xOld = player1.x;
            player1.y = PLAYER_TWO_Y_LVL_ONE;
            player1.yOld = player1.y;
            // player 2
            player2.x = PLAYER_ONE_X_LVL_ONE;
            player2.xOld = player2.x;
            player2.y = PLAYER_ONE_Y_LVL_ONE;
            player2.yOld = player2.y;
        }
    }
    else if (currentLevel == 2)
    {
        if (getFreq() == IR_56KHZ)
        {
            // player 1
            player1.x = PLAYER_ONE_X_LVL_TWO;
            player1.xOld = player1.x;
            player1.y = PLAYER_ONE_Y_LVL_TWO;
            player1.yOld = player1.y;
            // player 2
            player2.x = PLAYER_TWO_X_LVL_TWO;
            player2.xOld = player2.x;
            player2.y = PLAYER_TWO_Y_LVL_TWO;
            player2.yOld = player2.y;
        }
        else
        {
            // player 1
            player1.x = PLAYER_TWO_X_LVL_TWO;
            player1.xOld = player1.x;
            player1.y = PLAYER_TWO_Y_LVL_TWO;
            player1.yOld = player1.y;
            // player 2
            player2.x = PLAYER_ONE_X_LVL_TWO;
            player2.xOld = player2.x;
            player2.y = PLAYER_ONE_Y_LVL_TWO;
            player2.yOld = player2.y;
        }
    }
    else if (currentLevel == 3)
    {
        if (getFreq() == IR_56KHZ)
        {
            // player 1
            player1.x = PLAYER_ONE_X_LVL_THREE;
            player1.xOld = player1.x;
            player1.y = PLAYER_ONE_Y_LVL_THREE;
            player1.yOld = player1.y;
            // player 2
            player2.x = PLAYER_TWO_X_LVL_THREE;
            player2.xOld = player2.x;
            player2.y = PLAYER_TWO_Y_LVL_THREE;
            player2.yOld = player2.y;
        }
        else
        {
            // player 1
            player1.x = PLAYER_TWO_X_LVL_THREE;
            player1.xOld = player1.x;
            player1.y = PLAYER_TWO_Y_LVL_THREE;
            player1.yOld = player1.y;
            // player 2
            player2.x = PLAYER_ONE_X_LVL_THREE;
            player2.xOld = player2.x;
            player2.y = PLAYER_ONE_Y_LVL_THREE;
            player2.yOld = player2.y;
        }
    }
}

// Function to clear and draw Player1 and Player2
void drawPlayers()
{
    if (getFreq() == IR_56KHZ)
    {
        // Clear old playersprite and draw new one
        clearSprite(player1.x, player1.y, player1.xOld, player1.yOld, PLAYER_WIDTH, PLAYER_HEIGHT, Player1);
        drawSprite(player1.x, player1.y, PLAYER_WIDTH, PLAYER_HEIGHT, Player1);
        // Clear old playersprite and draw new one
        clearSprite(player2.x, player2.y, player2.xOld, player2.yOld, PLAYER_WIDTH, PLAYER_HEIGHT, Player2);
        drawSprite(player2.x, player2.y, PLAYER_WIDTH, PLAYER_HEIGHT, Player2);
    }
    else
    {
        // Clear old playersprite and draw new one
        clearSprite(player1.x, player1.y, player1.xOld, player1.yOld, PLAYER_WIDTH, PLAYER_HEIGHT, Player2);
        drawSprite(player1.x, player1.y, PLAYER_WIDTH, PLAYER_HEIGHT, Player2);
        // Clear old playersprite and draw new one
        clearSprite(player2.x, player2.y, player2.xOld, player2.yOld, PLAYER_WIDTH, PLAYER_HEIGHT, Player1);
        drawSprite(player2.x, player2.y, PLAYER_WIDTH, PLAYER_HEIGHT, Player1);
    }
}

// Function to clear a given sprite
void clearSprite(uint16_t x, uint8_t y, uint16_t xOld, uint8_t yOld, uint8_t w, uint8_t h, const uint8_t *Sprite)
{
    // For every pixelgroup in the sprite (2 pixels per pixelgroup)
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        // If the current pixelgroup is at the end of a row
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            // Go to the next row
            xOld -= w * 2;
            yOld++;
        }
        // For every pixel in the pixelgroup
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            // set the colour of the pixel to the colour of the spritepixel
            uint16_t colour = getcolour(((pgm_read_byte(&Sprite[PixGroup]) & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)));
            // If the pixel is not transparent and the pixel is not in the new sprite
            if (colour != 255 && !pointInRect(xOld, yOld, x, y, w * 2, h))
            {
                // Draw the pixel with the colour of the background
                uint8_t idx = ((yOld / BG_SPRITE_HEIGHT % 2) ? (xOld + BG_SPRITE_WIDTH) : xOld) % BG_SPRITE_ACTUAL_WIDTH / 2 + yOld % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH;
                drawPixel(xOld, yOld, getcolour((pgm_read_byte(&Background[idx]) & ((xOld % 2) ? 0x0F : 0xF0)) >> ((xOld % 2) ? 0 : 4)));
            }
            // Increment X
            xOld++;
        }
    }
}

// Function to clear a give sprite in is entirety
void clearWholeSprite(uint16_t x, uint8_t y, uint8_t w, uint8_t h)
{
    x -= 2;
    w += 4;
    // For every pixelgroup in the sprite (2 pixels per group)
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        // If the pixel group is at the end of the row
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            // Move to the next row
            x -= w * 2;
            y++;
        }
        // For every pixel in the pixel group
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            // Get the colour of the pixel of the background
            // Draw the pixel
            uint8_t idx = ((pgm_read_byte(&Background[(((y / BG_SPRITE_HEIGHT % 2) ? (x + BG_SPRITE_WIDTH) : x) % BG_SPRITE_ACTUAL_WIDTH / 2 + y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH)]) &
                            ((x % 2) ? 0x0F : 0xF0)) >>
                           ((x % 2) ? 0 : 4));
            drawPixel(x, y, getcolour(idx));
            // Increment the x position
            x++;
        }
    }
}

// Function to clear a given platform
void clearPlat(uint16_t x, uint8_t y, uint16_t xOld, uint8_t yOld, uint8_t w, uint8_t h)
{
    // For every pixelgroup in the platform (2 pixels per pixel group)
    for (uint16_t PixGroup = 0; PixGroup < w / 2 * h; PixGroup++)
    {
        // If the pixel group is at the end of a row
        if (PixGroup % (w / 2) == 0 && PixGroup != 0)
        {
            // Move back to the start of the row and move down a row
            xOld -= w;
            yOld++;
        }
        // For every pixel in the pixel group
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            // Get the colour of the pixel from the background sprite
            // Draw the pixel
            drawPixel(xOld, yOld, getcolour((pgm_read_byte(&Background[(((yOld / BG_SPRITE_HEIGHT % 2) ? (xOld + BG_SPRITE_WIDTH) : xOld) % BG_SPRITE_ACTUAL_WIDTH / 2 + yOld % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH)]) & ((xOld % 2) ? 0x0F : 0xF0)) >> ((xOld % 2) ? 0 : 4)));
            // Move to the next pixel
            xOld++;
        }
    }
}

// Function to draw a Horizontal Platform
void drawPlatH(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawLineH(x, y, x + w, BLACK);
    drawRect(x, y + 1, w, h - 2, INTER_BROWN);
    drawRect(x + 1, y + 2, w - 2, h - 4, INTER_GOLD);
    drawRect(x + 2, y + 3, w - 4, h - 6, getcolour(PLATFORM_MIDDLE_COLOUR, ver));
    drawLineH(x, y + 7, x + w, BLACK);
}

// Function to draw a Vertical Platform
void drawPlatV(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawRect(x + 1, y, w - 2, h, INTER_BROWN);
    drawRect(x + 2, y + 1, w - 4, h - 2, INTER_GOLD);
    drawRect(x + 3, y + 2, w - 6, h - 4, getcolour(PLATFORM_MIDDLE_COLOUR, ver));
}

// Function to draw a liquid
void drawLiquid(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawLineH(x + 1, y, x + w - 1, getcolour(LIQUID_TOP_COLOUR, ver));
    drawLineH(x + 1, y + 1, x + w - 1, getcolour(LIQUID_MIDDLE_COLOUR, ver));
    drawLineH(x + 1, y + 2, x + w - 1, getcolour(LIQUID_BOTTOM_COLOUR, ver));
    drawLineH(x + 1, y + 3, x + w - 1, getcolour(LIQUID_BOTTOM_COLOUR, ver));
}

// Function to draw a door
void drawDoor(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t ver)
{
    drawRect(x, y, w, h, BLACK);
    drawRect(x + 1, y + 1, w - 2, h - 1, SWAMP_GREEN);
    drawRect(x + 2, y + 2, w - 4, h - 2, SWAMP_GREEN);
    drawRect(x + 3, y + 3, w - 6, h - 3, BLACK);
    fillRect(x + 4, y + 4, w - 8, h - 4, INTER_BROWN);

    // If ver is 0, draw the door as blue, otherwise draw it as red
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

// Function to draw a lever
void drawLever(uint16_t x, uint8_t y)
{
    drawRect(x, y, LEVER_BASE_WIDTH, LEVER_BASE_HEIGHT, BLACK);
    drawRect(x + 1, y + 1, LEVER_BASE_WIDTH - 2, LEVER_BASE_HEIGHT - 3, INTER_GOLD);
    drawLineH(x + 1, y + 3, LEVER_BASE_WIDTH - 2, INTER_BROWN);
}

// Function to check if a point is in a rectangle
bool pointInRect(uint16_t pointX, uint8_t pointY, uint16_t x, uint8_t y, uint16_t w, uint8_t h)
{
    // If the point is inside a rectangle return true
    return pointX >= x && pointX <= x + w - 1 && pointY >= y && pointY <= y + h - 1;
}

// Function to check if a given player collides with a given rectangle
bool rectangleCollision(uint16_t playerX, uint8_t playerY, Rect &bounds)
{
    // If the player is colliding with the rectangle return true
    return playerX + PLAYER_ACTUAL_WIDTH > bounds.x && playerX < bounds.x + bounds.width && playerY + PLAYER_HEIGHT > bounds.y && playerY < bounds.y + bounds.height;
}

// Function to draw a given sprite
void drawSprite(uint16_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Sprite, uint8_t ver)
{
    // Loop through every pixel group (2 pixels per group)
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        // If the current group is at the width of the sprite
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            // Move the x position back to the start of the sprite and move the y position down
            x -= w * 2;
            y++;
        }
        // For each pixel in the pixel group
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            // Set the colour of the pixel to the colour of the pixel in the sprite
            uint16_t colour = getcolour(((pgm_read_byte(&Sprite[PixGroup]) & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)), ver);
            // If the colour is transparent
            if (colour == 255)
            {
                // Set the colour of the pixel to the colour of the pixel in the background
                colour = getcolour((pgm_read_byte(&Background[(((y / BG_SPRITE_HEIGHT % 2) ? (x + BG_SPRITE_WIDTH) : x) % BG_SPRITE_ACTUAL_WIDTH / 2 + y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH)]) &
                                    ((x % 2) ? 0x0F : 0xF0)) >>
                                       ((x % 2) ? 0 : 4),
                                   ver);
            }
            // Draw the pixel with the current colour
            drawPixel(x, y, colour);
            // Increment the x position
            x++;
        }
    }
}

// Function to draw the background in its entirety
void drawBackground()
{
    // Loop through the amount of rows of background tiles
    for (uint8_t y = 0; y < 24; y++)
    {
        // If the row is even, draw the tiles normally in every column
        if (y % 2 == 0)
            for (uint8_t x = 0; x < 16; x++)
                drawBackgroundTile(x * BG_SPRITE_ACTUAL_WIDTH, y * BG_SPRITE_HEIGHT, BG_SPRITE_WIDTH, BG_SPRITE_HEIGHT);
        // If the row is odd, draw the tiles mirrored in every column
        else
            for (uint8_t x = 0; x < 17; x++)
                drawBackgroundTile(x * BG_SPRITE_ACTUAL_WIDTH - BG_SPRITE_WIDTH, y * BG_SPRITE_HEIGHT, BG_SPRITE_WIDTH, BG_SPRITE_HEIGHT);
    }
}

// Function to draw a given sprite mirrored
void drawSpriteMirror(uint16_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Sprite, uint8_t ver)
{
    uint8_t yBackup = y;
    // Loop through all the pixelgroups of the sprite (2 pixels per pixelgroup)
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        // If the current pixelgroup is at the end of a row
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            // Set X back to the original value and increment Y
            x -= w * 2;
            y++;
        }
        // For each pixel in the pixelgroup
        for (int8_t Pixel = 1; Pixel >= 0; Pixel--)
        {
            // Set the colour of the pixel to the colour of the pixel of the sprite
            uint16_t colour = getcolour(((pgm_read_byte(&Sprite[w - 1 - PixGroup % w + (y - yBackup) * w]) & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)), ver);
            // If the colour is transparent
            if (colour == 255)
            {
                // Set the colour to the colour of the background
                colour = getcolour((pgm_read_byte(&Background[(((y / BG_SPRITE_HEIGHT % 2) ? (x + BG_SPRITE_WIDTH) : x) % BG_SPRITE_ACTUAL_WIDTH / 2 + y % BG_SPRITE_HEIGHT * BG_SPRITE_WIDTH)]) &
                                    ((x % 2) ? 0x0F : 0xF0)) >>
                                       ((x % 2) ? 0 : 4),
                                   ver);
            }
            // Draw the pixel with the current colour
            drawPixel(x, y, colour);
            // Increment X
            x++;
        }
    }
}

// Function to draw a background tile
void drawBackgroundTile(uint16_t x, uint8_t y, uint8_t w, uint8_t h)
{
    // loop through all the pixelgroups in the sprite (2 pixels per pixelgroup)
    for (uint16_t PixGroup = 0; PixGroup < w * h; PixGroup++)
    {
        // If the current pixelgroup is at the width of the sprite
        if (PixGroup % w == 0 && PixGroup != 0)
        {
            // Set x back to the start of the sprite and increment y
            x -= w * 2;
            y++;
        }
        // For each pixel in the pixelgroup
        for (uint8_t Pixel = 0; Pixel <= 1; Pixel++)
        {
            // Get the colour of the pixel for the background
            uint16_t colour = getcolour(((pgm_read_byte(&Background[PixGroup]) & ((Pixel) ? 0x0F : 0xF0)) >> ((Pixel) ? 0 : 4)));
            // for each initialized wall
            for (auto r : walls)
            {
                // If the current pixel is in the wall
                if (pointInRect(x, y, r.x, r.y, r.width, r.height))
                {
                    // Set the colour to the wall colour
                    colour = (colour == BACKGROUND_DARK) ? FOREGROUND_DARK : FOREGROUND_LIGHT;
                    break;
                }
            }
            // Draw the pixel with the current colour
            drawPixel(x, y, colour);
            // Increment x
            x++;
        }
    }
}

// Function to draw all the initialized interactables
void drawInteractables()
{
    // Draw all the initialized buttons
    for (button *B : buttons)
    {
        B->draw();
    }
    // Draw all the initialized levers
    for (lever *L : levers)
    {
        L->draw();
    }
    // Draw all the initialized platforms
    for (Platform *P : platforms)
    {
        P->draw();
    }
    // Draw all the initialized diamonds
    for (Diamond *D : Dias)
    {
        D->draw();
    }
    // Draw all the initialized liquids
    for (Liquid *L : liquids)
    {
        L->draw();
    }
    // Draw both doors
    drawDoor(Door1.x, Door1.y, Door1.width, Door1.height, 0);
    drawDoor(Door2.x, Door2.y, Door2.width, Door2.height, 1);
}

// Function to initialize level 1
void level1()
{
    currentlyPlayingLevel = 1;

    // Initialize walls
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
    walls[16] = {138, 0, 48, 22};

    // door blue
    Door1 = {55, 40, DOOR_WIDTH, DOOR_HEIGHT};
    // door red
    Door2 = {80, 40, DOOR_WIDTH, DOOR_HEIGHT};

    // water
    liq1 = {155, 230, LIQUID_WIDTH, LIQUID_HEIGHT, 0};
    // poison
    liq3 = {194, 180, LIQUID_WIDTH, LIQUID_HEIGHT, 2};
    // lava
    liq2 = {220, 230, LIQUID_WIDTH, LIQUID_HEIGHT, 1};

    // main purple platform
    Platform1 = {{280, 70, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 280, 280, 72, 112};
    // yellow platform
    Platform2 = {{10, 111, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 10, 10, 111, 141, 1};
    // purple platform for the diamonds
    Platform3 = {{40, 10, PLATFORM_HEIGHT, PLATFORM_WIDTH}, 40, 70, 10, 10};

    // lever for yellow platform
    Lever1 = {{93, 149, 2, 1}, false, false, {&Platform2}, 1};

    // button for purple platform
    button1 = {{146, 108, BUTTON_WIDTH * 2, 2}, false, {&Platform1, &Platform3}, 0};
    // button for purple platform
    button2 = {{181, 48, BUTTON_WIDTH * 2, 2}, false, {&Platform1, &Platform3}, 0};

    // Connect buttons 1 and 2 to eachother
    button1.setConnectedButton(&button2);
    button2.setConnectedButton(&button1);

    // blue diamond upperleft
    Dia1 = {12, 12, DIA_WIDTH * 4, DIA_HEIGHT, 0};
    // blue diamond bottom
    Dia2 = {164, 216, DIA_WIDTH * 4, DIA_HEIGHT, 0};
    // red diamond upperleft
    Dia3 = {26, 12, DIA_WIDTH * 4, DIA_HEIGHT, 1};
    // red diamond bottom
    Dia4 = {229, 216, DIA_WIDTH * 4, DIA_HEIGHT, 1};

    // set player positions
    setPlayerPos(1);

    // draw everything
    drawBackground();
    drawInteractables();
}

// Function to initialize level 2
void level2()
{
    // set player positions
    setPlayerPos(2);

    // initialize walls
    walls[0] = {0, 0, 6, 240};
    walls[1] = {6, 0, 310, 5};
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

    // door blue
    Door1 = {45, 35, DOOR_WIDTH, DOOR_HEIGHT};
    // door red
    Door2 = {10, 35, DOOR_WIDTH, DOOR_HEIGHT};

    // water left
    liq1 = {27, 118, LIQUID_WIDTH, LIQUID_HEIGHT, 0};
    // lava left top
    liq2 = {27, 178, LIQUID_WIDTH, LIQUID_HEIGHT, 1};
    // lava left bottom
    liq3 = {27, 227, LIQUID_WIDTH, LIQUID_HEIGHT, 1};
    // poison middle
    liq4 = {116, 178, 114, LIQUID_HEIGHT, 2};

    // purple platform
    Platform1 = {{5, 27, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 5, 5, 6, 27, 0};
    // yellow platform
    Platform2 = {{40, 27, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 40, 40, 6, 27, 1};
    // Red platform left
    Platform3 = {{127, 5, PLATFORM_HEIGHT, PLATFORM_WIDTH}, 127, 88, 5, 5, 2};
    // Red platform right
    Platform4 = {{260, 5, PLATFORM_HEIGHT, PLATFORM_WIDTH}, 260, 221, 5, 5, 2};
    // Green platform left
    Platform5 = {{65, 88, PLATFORM_HEIGHT, PLATFORM_WIDTH}, 65, 104, 88, 88, 3};
    // Blue platform
    Platform6 = {{73, 118, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 73, 73, 119, 226, 4};
    // Light_blue platform top
    Platform7 = {{250, 203, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 250, 250, 173, 203, 5};
    // Light_blue platform bottom
    Platform8 = {{250, 219, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 250, 250, 189, 219, 5};
    // White Platform
    Platform9 = {{285, 150, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 285, 285, 108, 150, 6};

    // Lever light_blue
    Lever1 = {{280, 33, 2, 1}, true, false, {&Platform7, &Platform8}, 5};
    // Lever yellow
    Lever2 = {{310, 33, 2, 1}, true, false, {&Platform2}, 1};
    // Lever green
    Lever3 = {{299, 176, 2, 1}, false, false, {&Platform5}, 3};
    // Lever purple
    Lever4 = {{299, 233, 2, 1}, true, false, {&Platform1}, 0};
    // lever for white platform
    Lever5 = {{14, 225, 2, 1}, true, false, {&Platform9}, 6};

    // button for blue platform
    button1 = {{127, 233, BUTTON_WIDTH * 2, 2}, false, {&Platform6}, 4};
    // button for blue platform
    button2 = {{12, 176, BUTTON_WIDTH * 2, 2}, false, {&Platform6}, 4};
    // button for red platform
    button3 = {{12, 116, BUTTON_WIDTH * 2, 2}, false, {&Platform3, &Platform4}, 2};

    // Connect buttons 1 and 3 to eachother
    button1.setConnectedButton(&button2);
    button2.setConnectedButton(&button1);

    // blue diamond upperright
    Dia1 = {286, 10, DIA_WIDTH * 4, DIA_HEIGHT, 0};
    // red diamond uppermiddle
    Dia2 = {100, 61, DIA_WIDTH * 4, DIA_HEIGHT, 1};
    // bue diamond upperleft
    Dia3 = {39, 101, DIA_WIDTH * 4, DIA_HEIGHT, 0};
    // red diamond middleleft
    Dia4 = {39, 163, DIA_WIDTH * 4, DIA_HEIGHT, 1};
    // red diamond bottomleft
    Dia5 = {39, 213, DIA_WIDTH * 4, DIA_HEIGHT, 1};
    // blue diamond middleright
    Dia6 = {252, 137, DIA_WIDTH * 4, DIA_HEIGHT, 0};

    // draw everything
    drawBackground();
    drawInteractables();
}

// Function to initialize level 3
void level3()
{
    clearLevel();
    // set player positions
    setPlayerPos(3);

    // initialize walls
    walls[0] = {0, 0, 320, 39};
    walls[1] = {0, 39, 145, 30};
    walls[2] = {0, 69, 89, 41};
    walls[3] = {119, 69, 26, 41};
    walls[4] = {0, 110, 10, 130};
    walls[5] = {10, 190, 20, 10};
    walls[6] = {10, 230, 310, 10};
    walls[7] = {60, 190, 250, 10};
    walls[8] = {310, 110, 10, 120};
    walls[9] = {175, 69, 25, 41};
    walls[10] = {230, 39, 90, 71};

    // door red
    Door2 = {290, 160, DOOR_WIDTH, DOOR_HEIGHT};
    // door blue
    Door1 = {290, 200, DOOR_WIDTH, DOOR_HEIGHT};

    // Poison
    liq1 = {69, 190, 200, LIQUID_HEIGHT, 2};

    // purple platform
    Platform1 = {{145, 93, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 145, 145, 93, 143, 0};
    // Blue platform
    Platform2 = {{89, 69, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 73, 73, 69, 169, 4};
    // Light_blue platform bottom
    Platform3 = {{200, 69, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 200, 200, 69, 169, 5};
    // White Platform
    Platform4 = {{30, 192, PLATFORM_WIDTH, PLATFORM_HEIGHT}, 30, 30, 110, 192, 6};

    // lever for white platform
    Lever5 = {{18, 189, 2, 1}, true, false, {&Platform4}, 6};

    // button for purple platform
    button1 = {{100, 228, BUTTON_WIDTH * 2, 2}, false, {&Platform1}, 0};
    // button for Light_blue platform
    button2 = {{161, 228, BUTTON_WIDTH * 2, 2}, false, {&Platform3}, 5};
    // button for red platform
    button3 = {{216, 228, BUTTON_WIDTH * 2, 2}, false, {&Platform2}, 4};

    // red diamond left
    Dia1 = {97, 149, DIA_WIDTH * 4, DIA_HEIGHT, 1};
    // red diamond upperRight
    Dia2 = {207, 51, DIA_WIDTH * 4, DIA_HEIGHT, 1};
    // bue diamond upperRight
    Dia3 = {154, 51, DIA_WIDTH * 4, DIA_HEIGHT, 0};
    // red diamond middleleft
    Dia4 = {210, 150, DIA_WIDTH * 4, DIA_HEIGHT, 0};

    // draw everything
    drawBackground();
    drawInteractables();
}

void clearLevel()
{
    // Clear all the level data
    for (uint8_t i = 0; i < 20; i++)
    {
        walls[i] = {};
    }

    Door1 = {};
    Door2 = {};
    Platform1 = {};
    Platform2 = {};
    Platform3 = {};
    Platform4 = {};
    Platform5 = {};
    Platform6 = {};
    Platform7 = {};
    Platform8 = {};
    Platform9 = {};
    Lever1 = {};
    Lever2 = {};
    Lever3 = {};
    Lever4 = {};
    Lever5 = {};
    button1 = {};
    button2 = {};
    button3 = {};
    Dia1 = {};
    Dia2 = {};
    Dia3 = {};
    Dia4 = {};
    Dia5 = {};
    Dia6 = {};
    liq1 = {};
    liq2 = {};
    liq3 = {};
    liq4 = {};
}

// Function to convert a 4 bit colour to a 16 bit colour
uint16_t getcolour(uint8_t colour, uint8_t ver)
{
    switch (colour)
    {
    case 0: // 0000
        return BLACK;

    case 1: // 0001
        return PLAYER_RED;

    case 2: // 0010
        return PLAYER_ORANGE;

    case 3: // 0011
        return PLAYER_YELLOW;

    case LIQUID_BOTTOM_COLOUR: // 0100
        // ver is used to determine which liquid is being drawn
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

    case LIQUID_MIDDLE_COLOUR: // 0101
        // ver is used to determine which liquid is being drawn
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

    case LIQUID_TOP_COLOUR: // 0110
        // ver is used to determine which liquid is being drawn
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
            // unused
    case 8: // 1000
        return INTER_BROWN;

    case 9: // 1001
        return INTER_GOLD;

    case PLATFORM_MIDDLE_COLOUR: // 0b00001010
        // ver is used to determine which platform, lever or button is being drawn
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
             // unused
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

// Function to draw the Player Select Screen
void drawPlayerSelectScreen()
{
    fillScreen(0x0);
    drawString("Player1", 80, 60, 4, PLAYER_RED);
    drawString("Player2", 80, 140, 4, PLAYER_RED);
    drawBorder(65, 50, 212, 50, 5, PLAYER_ORANGE);  // Player1 button
    drawBorder(65, 130, 212, 50, 5, PLAYER_ORANGE); // Player2 button
}

// Function to draw the menu screen
void drawMenu()
{
    fillScreen(BLACK);
    // Play button
    drawBorder(110, 50, 116, 50, 5, PLAYER_ORANGE);
    // Settings button
    drawBorder(65, 130, 212, 50, 5, PLAYER_ORANGE);
    drawString("Play", 120, 60, 4, PLAYER_RED);
    drawString("Settings", 75, 140, 4, PLAYER_RED);
    // H button
    drawBorder(265, 5, 50, 50, 5, PLAYER_ORANGE);
    drawString("H", 275, 15, 4, PLAYER_RED);
}

// Function to draw the Level Select screen
void drawLevelSelectScreen()
{
    // Draw over the old menu screen
    fillScreen(BLACK);
    // Draw new screen
    // Level 1 button
    drawBorder(45, 191, 229, 42, 5, PLAYER_BLUE);
    // Level 2 button
    if (level2Unlocked)
    {
        drawBorder(45, 131, 229, 42, 5, PLAYER_BLUE);
    }
    else
    {
        drawBorder(45, 131, 229, 42, 5, PLAYER_RED);
    }
    // Level 3 button
    if (level3Unlocked)
    {
        drawBorder(45, 71, 229, 42, 5, PLAYER_BLUE);
    }
    else
    {
        drawBorder(45, 71, 229, 42, 5, PLAYER_RED);
    }
    // Exit button
    drawBorder(14, 0, 294, 49, 5, PLAYER_BLUE);
    drawString("Level 1", 75, 197, 4, WHITE);
    drawString("Level 2", 75, 137, 4, WHITE);
    drawString("Level 3", 75, 77, 4, WHITE);
    drawString("Back to menu", 50, 13, 3, WHITE);
}

void drawHighScoreMenu()
{
    fillScreen(BLACK);

    // Highscores heading
    drawString("Highscores:", 40, 15, 4, PLAYER_RED);

    // Level 1 high score
    drawString("Level1", 50, 50, 4, PLAYER_ORANGE);
    // Level 1 high score number
    auto *pText = new unsigned char[4];
    pText[0] = EEPROM_read(HIGHSCORE_START_LEVEL_ADDR) / 100 + '0';
    pText[1] = (EEPROM_read(HIGHSCORE_START_LEVEL_ADDR) % 100) / 10 + '0';
    pText[2] = EEPROM_read(HIGHSCORE_START_LEVEL_ADDR) % 10 + '0';
    pText[3] = '\0';
    drawString((const char *)pText, 205, 50, 4, PLAYER_YELLOW);

    // Level 2 high score
    drawString("Level2", 50, 90, 4, PLAYER_ORANGE);
    // Level 2 high score number
    pText[0] = EEPROM_read(HIGHSCORE_START_LEVEL_ADDR + 1) / 100 + '0';
    pText[1] = (EEPROM_read(HIGHSCORE_START_LEVEL_ADDR + 1) % 100) / 10 + '0';
    pText[2] = EEPROM_read(HIGHSCORE_START_LEVEL_ADDR + 1) % 10 + '0';
    drawString((const char *)pText, 205, 90, 4, PLAYER_YELLOW);

    // Level 3 high score
    drawString("Level3", 50, 130, 4, PLAYER_ORANGE);
    // Level 2 high score number
    pText[0] = EEPROM_read(HIGHSCORE_START_LEVEL_ADDR + 2) / 100 + '0';
    pText[1] = (EEPROM_read(HIGHSCORE_START_LEVEL_ADDR + 2) % 100) / 10 + '0';
    pText[2] = EEPROM_read(HIGHSCORE_START_LEVEL_ADDR + 2) % 10 + '0';
    drawString((const char *)pText, 205, 130, 4, PLAYER_YELLOW);

    // Back button
    drawBorder(120, 180, 116, 50, 5, WHITE);
    drawString("Back", 130, 190, 4, PLAYER_RED);

    delete[] pText;
    pText = nullptr;
}

// Function to return the amount of lives left
uint8_t getLives()
{
    return EEPROM_read(LIVES_ADDR);
}

// Function to set the amount of lives left
void setLives(uint8_t &lives)
{
    // If the amount of lives is greater than the maximum amount of lives.
    if (lives > MAX_LIVES)
    {
        // set the amount of lives to the maximum amount of lives (5).
        lives = MAX_LIVES;
    }
    // If the amount of lives is 0.
    else if (lives == 0)
    {
        // Reset the amount of lives to the maximum amount of lives (5).
        // currentGameState = GAMEOVER;
        lives = MAX_LIVES; // Resets the lives to 5 if the player has no lives left.
    }
    // update the amount of lives in the EEPROM.
    EEPROM_update(LIVES_ADDR, lives);
}

// Function to show the amount of lives left on the 7 segment display
void showLives(uint8_t value)
{                                    // Talks to port expander and tells which pins to toggle.
    Wire.beginTransmission(IO_ADDR); // Starts transmission with the port expander on port 0x37.
    Wire.write(~liveCount[value]);   // Sends the reverse of the pattern for the correct letter, reverse because common anode.
    Wire.endTransmission();          // Ends transmission
}

// Function to update the highscore
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

// Function to draw the score on the screen
void drawScore(uint8_t highscore, bool clearScore)
{
    // Set the text colour
    uint16_t colour = PLAYER_BLUE;
    if (clearScore)
    {
        colour = BACKGROUND_LIGHT;
    }
    else if (highscore < 50)
    {
        colour = PLAYER_RED;
    }
    else if (highscore >= 50 && highscore < 100)
    {
        colour = PLAYER_YELLOW;
    }

    // Cast int to const char * (the default .c_str() function is not on the arduino)
    unsigned char *pText = new unsigned char[4];
    // Convert all numbers to a char
    pText[0] = score / 100 + '0';
    pText[1] = (score % 100) / 10 + '0';
    pText[2] = score % 10 + '0';
    pText[3] = '\0';
    // Draw the chars
    drawString((const char *)pText, SCORE_POS, 2, 2, colour);

    // Deallocate the array
    delete[] pText;
    pText = nullptr;
}
