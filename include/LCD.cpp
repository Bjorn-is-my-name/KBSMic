#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <SPI.h>
#include "font.c"
// Defines
#define WIDTH 320
#define HEIGHT 240
#define SOFTWARE_RESET 0x01
#define COLUMN_ADDRESS_SET 0x2A
#define PAGE_ADDRESS_SET 0x2B
#define MEMORY_WRITE 0x2C
#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8

//TOUCH
/** STMPE610 Address **/
#define STMPE_ADDR 0x41

/** Reset Control **/
#define STMPE_SYS_CTRL1 0x03
#define STMPE_SYS_CTRL1_RESET 0x02

/** Clock Contrl **/
#define STMPE_SYS_CTRL2 0x04

/** Touchscreen controller setup **/
#define STMPE_TSC_CTRL 0x40
#define STMPE_TSC_CTRL_EN 0x01
#define STMPE_TSC_CTRL_XYZ 0x00
#define STMPE_TSC_CTRL_XY 0x02

/** Interrupt control **/
#define STMPE_INT_CTRL 0x09
#define STMPE_INT_CTRL_POL_HIGH 0x04
#define STMPE_INT_CTRL_POL_LOW 0x00
#define STMPE_INT_CTRL_EDGE 0x02
#define STMPE_INT_CTRL_LEVEL 0x00
#define STMPE_INT_CTRL_ENABLE 0x01
#define STMPE_INT_CTRL_DISABLE 0x00

/** Interrupt enable **/
#define STMPE_INT_EN 0x0A
#define STMPE_INT_EN_TOUCHDET 0x01
#define STMPE_INT_EN_FIFOTH 0x02
#define STMPE_INT_EN_FIFOOF 0x04
#define STMPE_INT_EN_FIFOFULL 0x08
#define STMPE_INT_EN_FIFOEMPTY 0x10
#define STMPE_INT_EN_ADC 0x40
#define STMPE_INT_EN_GPIO 0x80

/** Interrupt status **/
#define STMPE_INT_STA 0x0B
#define STMPE_INT_STA_TOUCHDET 0x01

/** ADC control **/
#define STMPE_ADC_CTRL1 0x20
#define STMPE_ADC_CTRL1_12BIT 0x08
#define STMPE_ADC_CTRL1_10BIT 0x00

/** ADC control **/
#define STMPE_ADC_CTRL2 0x21
#define STMPE_ADC_CTRL2_1_625MHZ 0x00
#define STMPE_ADC_CTRL2_3_25MHZ 0x01
#define STMPE_ADC_CTRL2_6_5MHZ 0x02

/** Touchscreen controller configuration **/
#define STMPE_TSC_CFG 0x41
#define STMPE_TSC_CFG_1SAMPLE 0x00
#define STMPE_TSC_CFG_2SAMPLE 0x40
#define STMPE_TSC_CFG_4SAMPLE 0x80
#define STMPE_TSC_CFG_8SAMPLE 0xC0
#define STMPE_TSC_CFG_DELAY_10US 0x00
#define STMPE_TSC_CFG_DELAY_50US 0x08
#define STMPE_TSC_CFG_DELAY_100US 0x10
#define STMPE_TSC_CFG_DELAY_500US 0x18
#define STMPE_TSC_CFG_DELAY_1MS 0x20
#define STMPE_TSC_CFG_DELAY_5MS 0x28
#define STMPE_TSC_CFG_DELAY_10MS 0x30
#define STMPE_TSC_CFG_DELAY_50MS 0x38
#define STMPE_TSC_CFG_SETTLE_10US 0x00
#define STMPE_TSC_CFG_SETTLE_100US 0x01
#define STMPE_TSC_CFG_SETTLE_500US 0x02
#define STMPE_TSC_CFG_SETTLE_1MS 0x03
#define STMPE_TSC_CFG_SETTLE_5MS 0x04
#define STMPE_TSC_CFG_SETTLE_10MS 0x05
#define STMPE_TSC_CFG_SETTLE_50MS 0x06
#define STMPE_TSC_CFG_SETTLE_100MS 0x07

/** FIFO level to generate interrupt **/
#define STMPE_FIFO_TH 0x4A

/** Current filled level of FIFO **/
#define STMPE_FIFO_SIZE 0x4C

/** Current status of FIFO **/
#define STMPE_FIFO_STA 0x4B
#define STMPE_FIFO_STA_RESET 0x01
#define STMPE_FIFO_STA_OFLOW 0x80
#define STMPE_FIFO_STA_FULL 0x40
#define STMPE_FIFO_STA_EMPTY 0x20
#define STMPE_FIFO_STA_THTRIG 0x10

/** Touchscreen controller drive I **/
#define STMPE_TSC_I_DRIVE 0x58
#define STMPE_TSC_I_DRIVE_20MA 0x00
#define STMPE_TSC_I_DRIVE_50MA 0x01

/** Data port for TSC data address **/
#define STMPE_TSC_DATA_X 0x4D
#define STMPE_TSC_DATA_Y 0x4F
#define STMPE_TSC_FRACTION_Z 0x56

/** GPIO **/
#define STMPE_GPIO_SET_PIN 0x10
#define STMPE_GPIO_CLR_PIN 0x11
#define STMPE_GPIO_DIR 0x13
#define STMPE_GPIO_ALT_FUNCT 0x17
//END OF TOUCH

// Colors-------------------------------------------------
#define BLACK 0x0000                    ///<   0,   0,   0

#define PLAYER_DARK_BLUE 0x007f         ///<   0,   3,  31
#define PLAYER_BLUE 0x033f              ///<   0,  25,  31
#define PLAYER_LIGHT_BLUE 0x05BF        ///<   0,  45,  31

#define PLAYER_RED 0xF803               ///< 255,   0,   0
#define PLAYER_YELLOW 0xFE03            ///<  31,  48,   3
#define PLAYER_ORANGE 0xFAC3            ///<  31,  22,   3

#define INTER_BROWN 0x49A2              ///<   9,  13,   2
#define INTER_GOLD 0xFCE5               ///<  31,  39,   5
#define INTER_PURPLE 0xD25F             ///<  26,  18,  31
#define INTER_YELLOW 0xC600             ///<  24,  48    0

#define SWAMP_GREEN 0x23C0              ///<   4,  30,   0

#define BACKGROUND_DARK 0x1900          ///<   3,   8,   0
#define BACKGROUND_LIGHT 0x2961         ///<   5,  11,   1

#define FOREGROUND_LIGHT 0x7346         ///<  14,  26,   6
#define FOREGROUND_DARK 0x5A85          ///<  11,  20,   5
//--------------------------------------------------------

// Prototypes
void setupSPI();

void drawPixel(uint16_t, uint16_t, uint16_t);

void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h);

void SPI_CS_LOW();

void SPI_CS_HIGH();

void SPI_DC_LOW();

void SPI_DC_HIGH();

void SPI_WRITE8(uint8_t);

void SPI_WRITE16(uint16_t);

void SPI_WRITE_COMMAND(uint8_t);

void SEND_COMMAND_WITH_ARGUMENTS(uint8_t, uint8_t *, uint8_t);

void init_LCD();

void fillRect(uint16_t x, uint8_t y, uint16_t width, uint8_t height, uint16_t color);

void setupSPI()
{
    /*
    SS pins on internal pull-up (DC & CD)
    SS (DC & CD) & MOSI & SCK pins on output
    SCK Frequentie -> /2
    Master Mode & Enable SPI
    Data order MSB-first & set to MSB & set to DataMode 0
    */
    PORTB |= (1 << PORTB1) | (1 << PORTB2);
    DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
    SPSR |= (1 << SPI2X);
    SPCR |= (1 << MSTR) | (1 << SPE);
    SPCR &= ~((1 << DORD) | (1 << CPOL) | (1 << CPHA));
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x < WIDTH) && (y < HEIGHT))
    {
        // Set coords and send color
        SPI_CS_LOW();
        setAddrWindow(x, y, 1, 1);
        SPI_WRITE16(color);
        SPI_CS_HIGH();
    }
}

void fillRect(uint16_t x, uint8_t y, uint16_t width, uint8_t height, uint16_t color)
{
    for (uint16_t this_x = 0; this_x < width; ++this_x)
    {
        for (int this_y = 0; this_y < height; ++this_y)
        {
            drawPixel(this_x + x, this_y + y, color);
        }
    }
}

void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{
    // Keep track of address window coords, only reset when different from last time
    static uint16_t old_x1 = 0xFFFF, old_x2 = 0xFFFF;
    static uint16_t old_y1 = 0xFFFF, old_y2 = 0xFFFF;

    uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);

    // If addr-x changed, set to new x
    if (x1 != old_x1 || x2 != old_x2)
    {
        SPI_WRITE_COMMAND(COLUMN_ADDRESS_SET);
        // Set x-range (columns effected)
        SPI_WRITE16(x1);
        SPI_WRITE16(x2);
        old_x1 = x1;
        old_x2 = x2;
    }

    // If addr-y changed, set to new y
    if (y1 != old_y1 || y2 != old_y2)
    {
        SPI_WRITE_COMMAND(PAGE_ADDRESS_SET);
        // Set y-range (rows effected)
        SPI_WRITE16(y1);
        SPI_WRITE16(y2);
        old_y1 = y1;
        old_y2 = y2;
    }

    // Write to ram
    SPI_WRITE_COMMAND(MEMORY_WRITE);
}

void SPI_CS_LOW()
{
    // Set CS pin to low
    PORTB &= ~(1 << PB2);
}

void SPI_CS_HIGH()
{
    // Set CS pin to high
    PORTB |= (1 << PB2);
}

void SPI_DC_LOW()
{
    // Set DC pin to low
    PORTB &= ~(1 << PB1);
}

void SPI_DC_HIGH()
{
    // Set DC pin to high
    PORTB |= (1 << PB1);
}

void SPI_WRITE8(uint8_t data)
{
    SPDR = data;

    // Wait for the data to finish sending
    while (!((SPSR >> SPIF) & 1));
}

void SPI_WRITE16(uint16_t data)
{
    // Send left byte first (Because of MSB)
    SPI_WRITE8(data >> 8);
    SPI_WRITE8(data);
}

void SPI_WRITE_COMMAND(uint8_t cmd)
{
    SPI_DC_LOW();
    SPI_WRITE8(cmd);
    SPI_DC_HIGH();
}

void SEND_COMMAND_WITH_ARGUMENTS(uint8_t cmd, uint8_t *args, uint8_t len)
{
    SPI_CS_LOW();

    // Send the command
    SPI_WRITE_COMMAND(cmd);

    // Send the arguments
    for (uint8_t idx = 0; idx < len; idx++)
    {
        SPI_WRITE8(*args);
        args++;
    }

    SPI_CS_HIGH();
}

void drawChar(uint8_t ascii, uint16_t posX, uint16_t posY, uint16_t size, uint16_t color)
{
    for (int i = 0; i < FONT_X; i++)
    {
        uint8_t temp = pgm_read_byte(&simpleFont[ascii - 0x20][i]);
        for (uint8_t f = 0; f < 8; f++)
        {
            if ((temp >> f) & 0x01)
            {
                fillRect(posX + i * size, posY + f * size, size, size, color);
            }

        }
    }
}

void drawString(const char *string, uint16_t posX, uint16_t posY, uint16_t size, uint16_t color)
{
    while (*string)
    {
        drawChar(*string, posX, posY, size, color);
        *string++;
        if (posX < WIDTH)
        {
            posX += FONT_SPACE * size;                                     /* Move cursor right            */
        }
    }
}

void drawBorder(uint16_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t thickness, uint16_t color)
{
    // Draw top border
    for (uint16_t i = x; i < x + width; i++)
    {
        for (int t = 0; t < thickness; t++)
        {
            drawPixel(i, y + t, color);
        }
    }

    // Draw bottom border
    for (uint16_t i = x; i < x + width; i++)
    {
        for (int t = 0; t < thickness; t++)
        {
            drawPixel(i, y + height - t - 1, color);
        }
    }

    // Draw left border
    for (uint8_t j = y; j < y + height; j++)
    {
        for (int t = 0; t < thickness; t++)
        {
            drawPixel(x + t, j, color);
        }
    }

    // Draw right border
    for (uint8_t j = y; j < y + height; j++)
    {
        for (int t = 0; t < thickness; t++)
        {
            drawPixel(x + width - t - 1, j, color);
        }
    }
}


void init_LCD()
{
    SPI_CS_LOW();
    SPI_WRITE_COMMAND(SOFTWARE_RESET);
    SPI_CS_HIGH();

    uint8_t startup_commands[] = {
            0xef, 3, 0x03, 0x80, 0x02,
            0xcf, 3, 0x00, 0xC1, 0x30,
            0xed, 4, 0x64, 0x03, 0x12, 0x81,
            0xe8, 3, 0x85, 0x00, 0x78,
            0xcb, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
            0xf7, 1, 0x20,
            0xea, 2, 0x00, 0x00,

            0xc0, 1, 0x23,
            0xc1, 1, 0x10,
            0xc5, 2, 0x3e, 0x28,
            0xc7, 1, 0x86,
            0x36, 1, 0x28,
            0x37, 1, 0x00,
            0x3a, 1, 0x55,
            0xb1, 2, 0x00, 0x18,
            0xb6, 3, 0x08, 0x82, 0x27,
            0xf2, 1, 0x00,
            0x26, 1, 0x01,
            0xe0, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
            0xe1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
            0x11, 0x80,
            0x29, 0x80,
            0x00};

    // Send all commands with arguments
    uint8_t cmd, x, numArgs;
    uint8_t *addr = startup_commands;
    while ((cmd = *(addr++)) > 0)
    {
        x = *(addr++);
        numArgs = x & 0x7F;
        SEND_COMMAND_WITH_ARGUMENTS(cmd, addr, numArgs);
        addr += numArgs;
        // Wait after the last 2 commands. idk why needed tho, adafruit uses it and won't work without it
        if (x & 0x80)
        {
            _delay_ms(150);
        }
    }
}

//Touch screen functions


/*
void writeRegister8(uint8_t reg, uint8_t val)
{
    SPI_CS_LOW();
    SPI_WRITE_COMMAND(reg);
    SPI_WRITE8(val);
    SPI_CS_HIGH();
}


bool touchBegin()
{
    writeRegister8(STMPE_SYS_CTRL1, STMPE_SYS_CTRL1_RESET);
    _delay_ms(10);

    for (uint8_t i = 0; i < 65; i++)
    {
        readRegister8(i);
    }
    writeRegister8(STMPE_SYS_CTRL2, 0x0); // turn on clocks!
    writeRegister8(STMPE_TSC_CTRL, STMPE_TSC_CTRL_XYZ | STMPE_TSC_CTRL_EN); // XYZ and enable!
    Serial.println(readRegister8(STMPE_TSC_CTRL), HEX);
    writeRegister8(STMPE_INT_EN, STMPE_INT_EN_TOUCHDET);
    writeRegister8(STMPE_ADC_CTRL1, STMPE_ADC_CTRL1_10BIT | (0x6 << 4)); // 96 clocks per conversion
    writeRegister8(STMPE_ADC_CTRL2, STMPE_ADC_CTRL2_6_5MHZ);
    writeRegister8(STMPE_TSC_CFG, STMPE_TSC_CFG_4SAMPLE | STMPE_TSC_CFG_DELAY_1MS | STMPE_TSC_CFG_SETTLE_5MS);
    writeRegister8(STMPE_TSC_FRACTION_Z, 0x6);
    writeRegister8(STMPE_FIFO_TH, 1);
    writeRegister8(STMPE_FIFO_STA, STMPE_FIFO_STA_RESET);
    writeRegister8(STMPE_FIFO_STA, 0); // unreset
    writeRegister8(STMPE_TSC_I_DRIVE, STMPE_TSC_I_DRIVE_50MA);
    writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints
    writeRegister8(STMPE_INT_CTRL, STMPE_INT_CTRL_POL_HIGH | STMPE_INT_CTRL_ENABLE);
    return true;
}

boolean touched()
{
    return (readRegister8(STMPE_TSC_CTRL) & 0x80);
}

void readData(uint16_t *x, uint16_t *y, uint8_t *z)
{
    uint8_t data[4];

    for (unsigned char & i : data)
    {
        i = readRegister8(0xD7); // _spi->transfer(0x00);
        Serial.print("0x");
        Serial.print(i, HEX);
        Serial.print(" / ");
    }
    *x = data[0];
    *x <<= 4;
    *x |= (data[1] >> 4);
    *y = data[1] & 0x0F;
    *y <<= 8;
    *y |= data[2];
    *z = data[3];
}
*/
