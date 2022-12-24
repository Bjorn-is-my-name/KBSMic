//Nunchuk defines
#define NUNCHUK_ADDRESS 0x52


//IR defines
#define IR_38KHZ 52
#define IR_56KHZ 35
#define SENDINGDATA_LEN 16 // bits - 1
#define SENDINGBIT_START_VALUE (-2)
#define STARTBIT_VALUE (-1)
#define STARTBIT_MIN 3
#define STARTBIT_MAX 6
#define ZERO_MAX 2
#define ONE_MIN 1
#define ONE_MAX 4


//LCD defines
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

//Player defines
#define PLAYER_WIDTH 8
#define PLAYER_ACTUAL_WIDTH 16
#define PLAYER_HEIGHT 20
#define INITIAL_Y_VEL 10
#define MOVEMENT_SPEED 3

#define GRAVITY 1

//Game loop defines
#define FRAME_TIME 32 // (1000 / 30FPS) - 1 = 32

//Background defines
#define BG_SPRITE_AMOUNT 192
#define BG_SPRITE_WIDTH 10
#define BG_SPRITE_ACTUAL_WIDTH 20
#define BG_SPRITE_HEIGHT 10



/*START OF INTERACTABLE DEFINES*/
//Button defines
#define BUTTON_WIDTH 3
#define BUTTON_HEIGHT 4

//Liquid defines
#define LIQUID_WIDTH 38
#define LIQUID_HEIGHT 5


//Door defines
#define DOOR_WIDTH 20
#define DOOR_HEIGHT 30


//Sign defines
#define SIGN_WIDTH 2
#define SIGN_HEIGHT 13

//Platform defines
#define PLATFORM_WIDTH 30
#define PLATFORM_HEIGHT 8

#define DIA_WIDTH 3
#define DIA_HEIGHT 9
#define LEVER_BASE_WIDTH 4
#define LEVER_BASE_HEIGHT 5
#define LEVER_TOP_WIDTH 4
#define LEVER_TOP_HEIGHT 7

/*END OF INTERACTABLE DEFINES*/

/** STMPE610 Address **/
#define STMPE_ADDR 0x41

/** Reset Control **/
#define STMPE_SYS_CTRL1 0x03
#define STMPE_SYS_CTRL1_RESET 0x02

/** Clock Control **/
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

// TOUCH
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
// END OF TOUCH

// Colors-------------------------------------------------
#define BLACK 0x0000 ///<   0,   0,   0

#define PLAYER_DARK_BLUE 0x007f  ///<   0,   3,  31
#define PLAYER_BLUE 0x033f       ///<   0,  25,  31
#define PLAYER_LIGHT_BLUE 0x05BF ///<   0,  45,  31

#define PLAYER_RED 0xF803    ///< 255,   0,   0
#define PLAYER_YELLOW 0xFE03 ///<  31,  48,   3
#define PLAYER_ORANGE 0xFAC3 ///<  31,  22,   3

#define INTER_BROWN 0x49A2  ///<   9,  13,   2
#define INTER_GOLD 0xFCE5   ///<  31,  39,   5
#define INTER_PURPLE 0xD25F ///<  26,  18,  31
#define INTER_YELLOW 0xC600 ///<  24,  48    0

#define SWAMP_GREEN 0x23C0 ///<   4,  30,   0

#define BACKGROUND_DARK 0x1900  ///<   3,   8,   0
#define BACKGROUND_LIGHT 0x2961 ///<   5,  11,   1

#define FOREGROUND_LIGHT 0x7346 ///<  14,  26,   6
#define FOREGROUND_DARK 0x5A85  ///<  11,  20,   5
//--------------------------------------------------------

// Defines
#define WIDTH 320
#define HEIGHT 240
#define SOFTWARE_RESET 0x01
#define COLUMN_ADDRESS_SET 0x2A
#define PAGE_ADDRESS_SET 0x2B
#define MEMORY_WRITE 0x2C

// Colors-------------------------------------------------
#define BLACK 0x0000 ///<   0,   0,   0
#define WHITE 0xFFFF ///< 255, 255, 255
#define ALPHA 255

#define PLAYER_DARK_BLUE 0x007f  ///<   0,   3,  31
#define PLAYER_BLUE 0x033f       ///<   0,  25,  31
#define PLAYER_LIGHT_BLUE 0x05BF ///<   0,  45,  31

#define PLAYER_RED 0xF803    ///< 255,   0,   0
#define PLAYER_YELLOW 0xFE03 ///<  31,  48,   3
#define PLAYER_ORANGE 0xFAC3 ///<  31,  22,   3

#define INTER_BROWN 0x49A2  ///<   9,  13,   2
#define INTER_GOLD 0xFCE5   ///<  31,  39,   5
#define INTER_PURPLE 0xD25F ///<  26,  18,  31
#define INTER_YELLOW 0xC600 ///<  24,  48    0

#define SWAMP_GREEN 0x23C0 ///<   4,  30,   0

#define BACKGROUND_DARK 0x1900  ///<   3,   8,   0
#define BACKGROUND_LIGHT 0x2961 ///<   5,  11,   1

#define FOREGROUND_LIGHT 0x7346 ///<  14,  26,   6
#define FOREGROUND_DARK 0x5A85  ///<  11,  20,   5
#define PLATFORM_MIDDLE_COLOR 0b00001010
#define LIQUID_TOP_COLOR 0b00000110
#define LIQUID_MIDDLE_COLOR 0b00000101
#define LIQUID_BOTTOM_COLOR 0b00000100

// nunchuk memory addresses
#define NCSTATE 0x00 // address of state (6 bytes)
#define NCCAL 0x20   // address of callibration data (16 bytes)
#define NCID 0xFA    // address of id (4 bytes)

#define CHUNKLEN 32
#define STATELEN 6
#define CALLEN 16
#define IDLEN 4 // bytes

#define WAITFORREAD 1 // ms

// EEPROM adresses defines
#define LIVES_ADDR 0x00
// Highscore uses adresses from 0x01 to 0x04, since MAX_LEVEL is 3.
#define HIGHSCORE_START_LEVEL_ADDR 0x01

#define LEVEL_START_SCORE_ADDR 0x05

// Lives defines
#define MAX_LIVES 5

// Score defines
#define START_SCORE 200
#define HIGHSCORE true
#define CURRENT_SCORE false

// Level defines
#define MAX_LEVEL 3

// Nunchuk defines
#define IO_ADDR (0x39) // The port expander I2C address.
#define NUNCHUK_ADDRESS 0x52

#define IR_38KHZ 52
#define IR_56KHZ 35

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Player defines
#define PLAYER_WIDTH 8
#define PLAYER_ACTUAL_WIDTH 16
#define PLAYER_HEIGHT 20
#define PLAYER_ACCEL 1
#define PLAYER_MAX_ACCEL 6
#define GRAVITY 1

#define SENDINGDATA_LEN 16 // bits - 1
#define SENDINGBIT_START_VALUE (-2)

#define STARTBIT_VALUE (-1)
#define STARTBIT_MIN 3
#define STARTBIT_MAX 6
#define ZERO_MAX 2
#define ONE_MIN 1
#define ONE_MAX 4

// Highscore defines
#define SCORE_POS 142

#define INITIAL_Y_VEL 10
#define FRAME_TIME 32 //(1000/30FPS)-1 = 32

#define BG_SPRITE_AMOUNT 192
#define BG_SPRITE_WIDTH 10
#define BG_SPRITE_ACTUAL_WIDTH 20
#define BG_SPRITE_HEIGHT 10

#define BUTTON_WIDTH 3
#define BUTTON_HEIGHT 4
#define WATER_WIDTH 10
#define WATER_HEIGHT 5
#define DOOR_WIDTH 5
#define DOOR_HEIGHT 10
#define SIGN_WIDTH 2
#define SIGN_HEIGHT 13
#define PLATFORM_WIDTH 5
#define PLATFORM_HEIGHT 8
#define DIA_WIDTH 3
#define DIA_HEIGHT 9
#define LEVER_BASE_WIDTH 4
#define LEVER_BASE_HEIGHT 6
#define LEVER_TOP_WIDTH 4
#define LEVER_TOP_HEIGHT 7