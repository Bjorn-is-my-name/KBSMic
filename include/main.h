// Includes
#include <avr/io.h>
#include <avr/interrupt.h>

#include <Wire.h>
#include <Adafruit_STMPE610.h>
#include <Adafruit_STMPE610.cpp>

#include <EEPROM.c>
#include <LCD.cpp>
#include <Nunchuk.cpp>
#include <Sprites.c>
#include <Defines.c>

struct Rect
{
    uint16_t x;
    uint8_t y;
    uint16_t width;
    uint8_t height;
};

// Setup functions
void initTimer0();
void setFreq(uint8_t);
uint8_t getFreq();

// Drawing functions
void drawMenu();
void drawPlayerSelectScreen();
void drawLevelSelectScreen();
void drawBackground();
void drawBackgroundTile(uint16_t, uint8_t, uint8_t, uint8_t);
void DrawPlayers();
void clearSprite(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *);
void clearWholeSprite(uint16_t, uint8_t, uint8_t, uint8_t);
void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t ver = 0);
void drawSpriteMirror(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t *, uint8_t ver = 0);
void drawInteractables();
void clearPlat(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);
void drawPlatH(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawPlatV(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawLiquid(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawLever(uint16_t, uint8_t, uint8_t);
void drawScore(uint8_t, bool);
uint16_t getcolour(uint8_t, uint8_t ver = 0);

// Collision functions
void checkWallCollision();
void CheckPlatformCollision();
void checkCollision(Rect &);
bool rectangleCollision(uint16_t, uint8_t, Rect &);
void checkButtons();
void checkPoolCollision();
void checkLevers();
void checkDias();
bool pointInRect(uint16_t, uint8_t, uint16_t, uint8_t, uint16_t, uint8_t);

// Other functions
void Update();
void level1();
void level2();
uint8_t getLives();
void setLives(uint8_t &);
void showLives(uint8_t);
void showScore(bool, uint8_t);
void updateHighscore(uint8_t, uint8_t);