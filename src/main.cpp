#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <Adafruit_ILI9341.h>
#include <Nunchuk.h>
#include <Background.c>
#include <Player1.c>
#include <Player2.c>

void init_timer0();
void setFreq(uint8_t);
void drawSprite(uint16_t, uint8_t, const uint8_t, const uint8_t, const uint8_t*);
void drawBackground();
void buffer(uint16_t, uint8_t, uint16_t*, uint16_t*);
uint16_t getColor(uint8_t);

#define BG_SPRITE_AMOUNT 192
#define BG_SPRITE_SIZE 20

#define TFT_CS 10
#define TFT_DC 9
#define NUNCHUK_ADDRESS 0x52

#define IR_38KHZ 52
#define IR_56KHZ 35

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 20

#define SENDINGDATA_LEN 8

#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Check to see if the current bit is done sending
bool dataIsSend = false;
// Data to send over IR
uint8_t sendingData = 0b01010101;
// The data bit to send;
int8_t sendingBit = -2;
uint8_t onTime = 0;
unsigned long currentMs = 0;

uint8_t halfMsTime;
uint8_t startTime;
uint8_t zeroTime;
uint8_t oneTime;
uint8_t offTime;

ISR(PCINT2_vect)
{
    PORTD ^= 1;
}

// Toggle IR light
ISR(TIMER0_COMPA_vect)
{
    static uint8_t counter = 0;
    static uint8_t msCounter = 0;

    if(++msCounter >= halfMsTime){
        currentMs++;
        msCounter = 0;
    }

    if (++counter > onTime)
    {
        // If the bit is done sending, wait before sending the next bit
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
            if (++sendingBit < SENDINGDATA_LEN)
            {
                // Send start bit
                if (sendingBit == -1)
                    onTime = startTime;
                // Send data
                else
                    // Set the time corresponding to the bit
                    onTime = ((sendingData >> sendingBit) & 1) ? oneTime : zeroTime;

                // Enabe TC0
                TCCR0A &= ~(1 << COM0A1);
            }
            else
            {
                // Once all bits are send, reset for next run
                sendingBit = -2;
            }
        }

        // Flip between sending a bit and waiting
        dataIsSend = !dataIsSend;
        counter = 0;
    }
}

int main(void) {
    // Player position (x and y flipped because they are flipped on the joystick)
    uint16_t pos[] = {SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2};

    // Setup IR sending
    DDRD |= (1 << DDD6) | 1;
    init_timer0();

    setFreq(IR_38KHZ);

    // Setup screen
    sei();
    Wire.begin();
    tft.begin();
    tft.setRotation(1);

    // Check nunckuk connection
    while(!Nunchuk.begin(NUNCHUK_ADDRESS))
        tft.fillScreen(ILI9341_RED);

    drawBackground();
    // tft.fillScreen(ILI9341_BLACK);
    // buffer(100, 100, Background, Player1);

    // drawSprite(200, 100, 16, 20, 1);

    while (1) {
        // Position change lambda function
        [&pos]()
        {
            // Check nunchuk connection
            if(!Nunchuk.getState(NUNCHUK_ADDRESS))
                return 0;

            // Check for joystick usage

            // Right
            if (Nunchuk.state.joy_x_axis > 140) {
                if (pos[0] + PLAYER_WIDTH < SCREEN_WIDTH)
                    pos[0]++;
            }
            // Left
            else if (Nunchuk.state.joy_x_axis < 100) {
                if (pos[0] > 1)
                    pos[0]--;
            }

            // Upwards acceleration
            if (Nunchuk.state.accel_z_axis <= 10 && pos[1] > 5) {
                pos[1]-= 5;
            }
            else if(pos[1] < SCREEN_HEIGHT - PLAYER_HEIGHT){
                pos[1]++;
            }
        }();

        drawSprite(pos[0], pos[1], 16, 20, Player1);
        drawSprite(200, pos[1], 16, 20, Player2);

    }

    return (0);
}

void init_timer0() 
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
        halfMsTime = 17;
        startTime = 149;
        zeroTime = 17;
        oneTime = 55;
        offTime = 17;
    }
    else
    {
        halfMsTime = 26;
        startTime = 221;
        zeroTime = 26;
        oneTime = 82;
        offTime = 26;
    }
}

void drawSprite(uint16_t x, uint8_t y, const uint8_t w, const uint8_t h, const uint8_t *Sprite){
    for(uint16_t PixGroup = 0; PixGroup < (w/2)*h; PixGroup++){
        if(PixGroup%(w/2)==0 && PixGroup!=0){
            x-=w;
            y++;
        }
        
        for(uint8_t Pixel = 0; Pixel <=1; Pixel++){

            if(Pixel==0){
                if(getColor((Sprite[PixGroup]&0xF0)>>4)!=255){
                    tft.drawPixel(x,y,getColor((Sprite[PixGroup]&0xF0)>>4));
                }
            }else{
                if(getColor((Sprite[PixGroup]&0x0F))!=255){
                    tft.drawPixel(x,y,getColor(Sprite[PixGroup]&0x0F));
                }
            }
            x++;
        }
    } 
}

void drawBackground(){
    uint16_t x = 0;
    uint8_t y = 0;
    for(uint8_t i = 0; i < BG_SPRITE_AMOUNT; i++){
        drawSprite(x, y, BG_SPRITE_SIZE, BG_SPRITE_SIZE, Background);
        if(x>=SCREEN_WIDTH-BG_SPRITE_SIZE){
            x=0;
            y+=BG_SPRITE_SIZE;
        }else
            x+=BG_SPRITE_SIZE;
    }
}

void buffer(/*OldPos, NewPos, UsedSprite*/){
    
}


uint16_t getColor(uint8_t Color){
    switch (Color) {
  	case 0:
		return ILI9341_RED;
		break;
  	case 1:
		return ILI9341_BLACK;
    	break;
  	case 2:
		return ILI9341_GREEN;
		break;
  	case 3:
		return ILI9341_OLIVE;
    	break;
	case 4:
		return ILI9341_ORANGE;
		break;
  	case 5:
		return ILI9341_YELLOW;
    	break;
  	case 6:
		return ILI9341_LIGHTGREY;
		break;
  	case 7:
		return ILI9341_DARKGREY;
    	break;
	case 8:
		return ILI9341_BLUE;
		break;
  	case 9:
		return ILI9341_CYAN;
    	break;
  	case 10:
		return ILI9341_WHITE;
		break;
  	case 11:
		return ILI9341_BACKGROUND_DARK;
    	break;
	case 12:
		return ILI9341_BACKGROUND_LIGHT;
		break;
  	case 13:
		// return ILI9341_---;
    	break;
  	case 14:
		// return ILI9341_---;
		break;
    case 15:
        return 255;
        break;
  	};
}