// Includes
#include <avr/io.h>
#include <avr/interrupt.h>

#include <Wire.h>

#include <Defines.c>
#include <EEPROM.c>
#include <LCD.cpp>
#include <Nunchuk.cpp>
#include <Sprites.c>

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
void drawHighScoreMenu();

void drawBackground();
void drawBackgroundTile(uint16_t, uint8_t, uint8_t, uint8_t);
void setPlayerPos(uint8_t);
void drawPlayers();
void clearSprite(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *);
void clearWholeSprite(uint16_t, uint8_t, uint8_t, uint8_t);
void drawSprite(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t ver = 0);
void drawSpriteMirror(uint16_t, uint8_t, uint8_t, uint8_t, const uint8_t *, uint8_t ver = 0);
void drawInteractables();
void clearPlat(uint16_t, uint8_t, uint16_t, uint8_t, uint8_t, uint8_t);
void drawPlatH(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawPlatV(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawLiquid(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
void drawLever(uint16_t, uint8_t);
void drawScore(uint8_t, bool);
uint16_t getcolour(uint8_t, uint8_t ver = 0);

// Collision functions
void checkWallCollision();
void checkPlatformCollision();
void checkCollision(Rect &);
void checkPoolCollision();
bool rectangleCollision(uint16_t, uint8_t, Rect &);
void checkButtons();
void checkLevers();
void checkDias();
void checkFinish();
bool pointInRect(uint16_t, uint8_t, uint16_t, uint8_t, uint16_t, uint8_t);

// Other functions
void update();
void level1();
void level2();
void level3();
void clearLevel();
uint8_t getLives();
void setLives(uint8_t &);
void showLives(uint8_t);
void updateHighscore(uint8_t, uint8_t);