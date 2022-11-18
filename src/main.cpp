/*
 * read data from nunchuk and write to Serial
 */
#include <util/delay.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <Nunchuk.h>
#include "Adafruit_ILI9341.h"
#include "SPI.h"
#include <Adafruit_STMPE610.h>
#include <IRremote.h>


#define NUNCHUK_ADDRESS 0x52
#define WAIT        0
#define BAUDRATE    9600
#define CHUNKSIZE    32
#define BUFFERLEN    256
int rect_pos_x = 0;
int rect_pos_y = 0;


//SCHERM ZOOI
#define TS_MINX 150
#define TS_MINY 130
#define TS_MAXX 3800
#define TS_MAXY 4000

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;

// what to show
#define STATE
//#define MEM
//#define CAL

// prototypes
bool show_memory(void);

bool move_rect(void);

bool show_calibration(void);

/*
 * main
 */
int main(void) {
    // enable global interrupts
    sei();
    // use Serial for printing nunchuk data
    Serial.begin(BAUDRATE);

    // join I2C bus as master
    Wire.begin();

    tft.begin();

    if (!ts.begin()) {
        Serial.println("Couldn't start touchscreen controller");
        while (1);
    }
    Serial.println("Touchscreen started");

    tft.fillScreen(ILI9341_BLUE);


    // handshake with nunchuk
    if (!Nunchuk.begin(NUNCHUK_ADDRESS)) {
        Serial.println("******** No nunchuk found");
        Serial.flush();
        return (1);
    }

    /*
     * get identificaton (nunchuk should be 0xA4200000)
     */
    Serial.print("-------- Nunchuk with Id: ");
    Serial.println(Nunchuk.id);

    while (1) {
        if (!move_rect()) {
            Serial.println("******** No nunchuk found");
        }
    }
    return (0);
}

bool move_rect(void) {
    if (!Nunchuk.getState(NUNCHUK_ADDRESS))
        return (false);
//    uint8_t		joy_x_axis;
//    uint8_t		joy_y_axis;
//    uint16_t	accel_x_axis;
//    uint16_t	accel_y_axis;
//    uint16_t	accel_z_axis;
//    uint8_t		z_button;
//    uint8_t		c_button;
    if (Nunchuk.state.joy_x_axis > 140) {
        rect_pos_x += 1;
        tft.fillRect(rect_pos_y, rect_pos_x - 20, 20, 20, ILI9341_BLUE);
    } else if (Nunchuk.state.joy_x_axis < 100) {
        rect_pos_x -= 1;
        tft.fillRect(rect_pos_y, rect_pos_x + 20, 20, 20, ILI9341_BLUE);
    }

    if (Nunchuk.state.joy_y_axis > 140) {
        rect_pos_y += 1;
        tft.fillRect(rect_pos_y - 20, rect_pos_x, 20, 20, ILI9341_BLUE);
    } else if (Nunchuk.state.joy_y_axis < 100) {
        rect_pos_y -= 1;
        tft.fillRect(rect_pos_y + 20, rect_pos_x, 20, 20, ILI9341_BLUE);
    }

    if (rect_pos_x <= 0) {
        rect_pos_x = 0;
    }
    if (rect_pos_y <= 0) {
        rect_pos_y = 0;
    }
    if (rect_pos_x >= 300) {
        rect_pos_x = 300;
    }
    if (rect_pos_y >= 220) {
        rect_pos_y = 220;
    }

    Serial.println(rect_pos_y);
//    Serial.println(rect_pos_x);
    tft.fillRect(rect_pos_y, rect_pos_x, 20, 20, ILI9341_WHITE);
    _delay_ms(1);
    return (true);
}

