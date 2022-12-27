/*!
 *
 *  @file Adafruit_STMPE610.cpp
 *
 *  This is a library for the Adafruit STMPE610 Resistive
 *  touch screen controller breakout
 *  ----> http://www.adafruit.com/products/1571
 *
 *  Check out the links above for our tutorials and wiring diagrams
 *  These breakouts use SPI or I2C to communicate
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing
 *  products from Adafruit!
 *
 *  Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 *  MIT license, all text above must be included in any redistribution
 */

#ifndef _ADAFRUIT_STMPE610H_
#define _ADAFRUIT_STMPE610H_

#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>
#include <Defines.c>

/*!
 *  @brief  Class for working with points
 */
class TS_Point
{
public:
  TS_Point();
  TS_Point(int16_t x, int16_t y, int16_t z);

  bool operator==(TS_Point);
  bool operator!=(TS_Point);

  int16_t x; /**< x coordinate **/
  int16_t y; /**< y coordinate **/
  int16_t z; /**< z coordinate **/
};

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          STMPE610
 */
class Adafruit_STMPE610
{
public:
  Adafruit_STMPE610(uint8_t cspin, uint8_t mosipin, uint8_t misopin,
                    uint8_t clkpin);
  Adafruit_STMPE610(uint8_t cspin, SPIClass *theSPI = &SPI);
  Adafruit_STMPE610(TwoWire *theWire = &Wire);

  boolean begin(uint8_t i2caddr = STMPE_ADDR);

  void writeRegister8(uint8_t reg, uint8_t val);
  uint16_t readRegister16(uint8_t reg);
  uint8_t readRegister8(uint8_t reg);
  void readData(uint16_t *x, uint16_t *y, uint8_t *z);
  uint16_t getVersion();
  boolean touched();
  boolean bufferEmpty();
  uint8_t bufferSize();
  TS_Point getPoint();

private:
  uint8_t spiIn();
  void spiOut(uint8_t x);

  TwoWire *_wire;
  SPIClass *_spi;
  int8_t _CS, _MOSI, _MISO, _CLK;
  uint8_t _i2caddr;

  int m_spiMode;
};

#endif
