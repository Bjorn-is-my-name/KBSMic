#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <SPI.h>

// Defines
#define WIDTH 320
#define HEIGHT 240
#define SOFTWARE_RESET 0x01
#define COLUMN_ADDRESS_SET 0x2A
#define PAGE_ADDRESS_SET 0x2B
#define MEMORY_WRITE 0x2C

#define ILI9341_BLACK 0x0000       ///<   0,   0,   0
#define ILI9341_OLIVE 0x7BE0       ///< 123, 125,   0
#define ILI9341_LIGHTGREY 0xC618   ///< 198, 195, 198
#define ILI9341_DARKGREY 0x7BEF    ///< 123, 125, 123
#define ILI9341_BLUE 0x001F        ///<   0,   0, 255
#define ILI9341_GREEN 0x07E0       ///<   0, 255,   0
#define ILI9341_CYAN 0x07FF        ///<   0, 255, 255
#define ILI9341_RED 0xF800         ///< 255,   0,   0
#define ILI9341_MAGENTA 0xF81F     ///< 255,   0, 255
#define ILI9341_YELLOW 0xFFE0      ///< 255, 255,   0
#define ILI9341_WHITE 0xFFFF       ///< 255, 255, 255
#define ILI9341_ORANGE 0xFD20      ///< 255, 165,   0

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
void START_UP();


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
    if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT))
    {
        // Set coords and send color
        SPI_CS_LOW();
        setAddrWindow(x, y, 1, 1);
        SPI_WRITE16(color);
        SPI_CS_HIGH();
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

void START_UP()
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