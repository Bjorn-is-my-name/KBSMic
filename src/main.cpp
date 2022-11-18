#include <avr/interrupt.h>
#include <Wire.h>
#include <Nunchuk.h>
#include <Adafruit_ILI9341.h>

#define NUNCHUK_ADDRESS 0x52

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

void move();

int main(void) {
    uint8_t pos[] = {0, 0};
    
    sei();
    
    Wire.begin();
    
    tft.begin();
    tft.fillScreen(ILI9341_BLUE);
    
    if (!Nunchuk.begin(NUNCHUK_ADDRESS))
        return 1;
    
    while (1) {
        [&pos]()
        {
            if (!Nunchuk.getState(NUNCHUK_ADDRESS))
                return (false);
            
            if (Nunchuk.state.joy_x_axis > 140) {
                pos[0]++;
            }
            else if (Nunchuk.state.joy_x_axis < 100) {
                pos[0]--;
            }

            if (Nunchuk.state.joy_y_axis > 140) {
                pos[1]++;
            }
            else if (Nunchuk.state.joy_y_axis < 100) {
                pos[1]--;
            }
        }();
        
        tft.fillRect(pos[1], pos[0], 20, 20, ILI9341_BLUE);        
        tft.fillRect(pos[1], pos[0], 20, 20, ILI9341_WHITE);
        
        posOld[0] = pos[0];
        posOld[1] = pos[1];
    }

    return (0);
}