// #TODO MOETEN NOG GESORTEERD WORDEN
#define FONT_SPACE 6
#define FONT_X 8
#define FONT_Y 8
#define FRAME_TIME 32 //(1000/30FPS)-1 = 32
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// Lives defines
#define MAX_LIVES 5
#define LIVES_ADDR 0x00

// Score defines
#define LEVEL_START_SCORE_ADDR 0x05
#define START_SCORE 200
#define CURRENT_SCORE false
#define DIAMOND_POINTS 30
// Highscore defines
#define HIGHSCORE_START_LEVEL_ADDR 0x01
#define HIGHSCORE true  // niet gebruikt? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define SCORE_POS 142   // niet gebruikt? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Level defines
#define MAX_LEVEL 3 // niet gebruikt? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//level one
#define PLAYER_ONE_X_LVL_ONE 13
#define PLAYER_ONE_Y_LVL_ONE 170

#define PLAYER_TWO_X_LVL_ONE 13
#define PLAYER_TWO_Y_LVL_ONE 210

//level two
#define PLAYER_ONE_X_LVL_TWO 207
#define PLAYER_ONE_Y_LVL_TWO 215

#define PLAYER_TWO_X_LVL_TWO 187
#define PLAYER_TWO_Y_LVL_TWO 215

// Nunchuk defines
#define IO_ADDR (0x39) // The port expander I2C address.
#define NUNCHUK_ADDRESS 0x52
// nunchuk memory addresses
#define NCSTATE 0x00 // address of state (6 bytes)
#define NCCAL 0x20   // address of callibration data (16 bytes)  niet gebruikt? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define NCID 0xFA    // address of id (4 bytes)
#define CHUNKLEN 32
#define STATELEN 6
#define CALLEN 16 // niet gebruikt? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define IDLEN 4 // bytes
#define WAITFORREAD 1 // ms

// IR defines
#define IR_38KHZ 52
#define IR_56KHZ 35
#define STARTBIT_VALUE (-1)
#define STARTBIT_MIN 3
#define STARTBIT_MAX 6
#define ZERO_MAX 2
#define ONE_MIN 1
#define ONE_MAX 4
#define SENDINGDATA_LEN 16 // bits - 1
#define SENDINGBIT_START_VALUE (-2)

// sceen dimensions
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
#define MOVEMENT_SPEED 3
#define CURRENT_PLAYER_ADDRESS 20


// Background defines
#define BG_SPRITE_WIDTH 10
#define BG_SPRITE_ACTUAL_WIDTH 20
#define BG_SPRITE_HEIGHT 10

// interactibles defines
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

// colours-------------------------------------------------
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


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978