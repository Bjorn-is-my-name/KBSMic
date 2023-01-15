// Gameloop defines
#define FRAME_TIME 32 //(1000/30FPS)-1 = 32

// Lives defines
#define MAX_LIVES 5
#define LIVES_ADDR 0x00

// Score defines
#define START_SCORE 200
#define DIAMOND_POINTS 30
// Highscore defines
#define HIGHSCORE_START_LEVEL_ADDR 0x01
#define SCORE_POS 142

// Level defines
#define MAX_LEVEL 3
// level one
#define PLAYER_ONE_X_LVL_ONE 13
#define PLAYER_ONE_Y_LVL_ONE 170

#define PLAYER_TWO_X_LVL_ONE 13
#define PLAYER_TWO_Y_LVL_ONE 210

// level two
#define PLAYER_ONE_X_LVL_TWO 207
#define PLAYER_ONE_Y_LVL_TWO 215

#define PLAYER_TWO_X_LVL_TWO 187
#define PLAYER_TWO_Y_LVL_TWO 215

//level three
#define PLAYER_ONE_X_LVL_THREE 274
#define PLAYER_ONE_Y_LVL_THREE 170

#define PLAYER_TWO_X_LVL_THREE 274
#define PLAYER_TWO_Y_LVL_THREE 210

// Nunchuk defines
#define IO_ADDR (0x39) // The port expander I2C address.
#define NUNCHUK_ADDRESS 0x52
// nunchuk memory addresses
#define NCSTATE 0x00 // address of state (6 bytes)
#define NCID 0xFA    // address of id (4 bytes)
#define CHUNKLEN 32
#define STATELEN 6
#define IDLEN 4       // bytes
#define WAITFORREAD 1 // ms

// IR defines
#define IR_38KHZ 52
#define IR_56KHZ 35
#define STARTBIT_VALUE (-1)
#define STARTBIT_MIN 3
#define STARTBIT_MAX 6
#define ZERO_MAX 2
#define SENDINGBIT_START_VALUE (-2)

#define PACKAGE1_HEADER 0b001
#define PACKAGE2_HEADER 0b010
#define PACKAGE3_HEADER 0b011
#define PACKAGE4_HEADER 0b100
#define PACKAGE5_HEADER 0b101
#define PACKAGE_HEADER_LENGTH 3
#define EMPTY_PACKAGE_LENGTH 3
#define PACKAGE1_LENGTH 4
#define PACKAGE2_LENGTH 6
#define PACKAGE3_LENGTH 5
#define PACKAGE4_LENGTH 12
#define PACKAGE5_LENGTH 11

// Sceen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Player defines
#define PLAYER_WIDTH 8
#define PLAYER_ACTUAL_WIDTH 16
#define PLAYER_HEIGHT 20
#define PLAYER_ACCEL 1
#define PLAYER_MAX_ACCEL 6
#define GRAVITY 1
#define INITIAL_Y_VEL 10

// Background defines
#define BG_SPRITE_WIDTH 10
#define BG_SPRITE_ACTUAL_WIDTH 20
#define BG_SPRITE_HEIGHT 10

// Interactibles defines
#define BUTTON_WIDTH 3
#define BUTTON_HEIGHT 4
#define LIQUID_WIDTH 38
#define LIQUID_HEIGHT 5
#define DOOR_WIDTH 20
#define DOOR_HEIGHT 30
#define SIGN_WIDTH 2
#define SIGN_HEIGHT 13
#define PLATFORM_WIDTH 30
#define PLATFORM_HEIGHT 8
#define DIA_WIDTH 3
#define DIA_HEIGHT 9
#define LEVER_BASE_WIDTH 16
#define LEVER_BASE_HEIGHT 5
#define LEVER_TOP_WIDTH 4
#define LEVER_TOP_HEIGHT 7

// Addresses
#define SOFTWARE_RESET 0x01
#define COLUMN_ADDRESS_SET 0x2A
#define PAGE_ADDRESS_SET 0x2B
#define MEMORY_WRITE 0x2C
#define EEPROM_SIZE 21

// Colours-------------------------------------------------
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

// Getcolor values
#define PLATFORM_MIDDLE_COLOUR 0b00001010
#define LIQUID_TOP_COLOUR 0b00000110
#define LIQUID_MIDDLE_COLOUR 0b00000101
#define LIQUID_BOTTOM_COLOUR 0b00000100

// Font defines
#define FONT_SPACE 6
#define FONT_X 8
