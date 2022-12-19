#define NUNCHUK_ADDRESS 0x52

#define IR_38KHZ 52
#define IR_56KHZ 35

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define PLAYER_WIDTH 8
#define PLAYER_ACTUAL_WIDTH 16
#define PLAYER_HEIGHT 20

#define SENDINGDATA_LEN 16 // bits - 1
#define SENDINGBIT_START_VALUE (-2)

#define STARTBIT_VALUE (-1)
#define STARTBIT_MIN 3
#define STARTBIT_MAX 6
#define ZERO_MAX 2
#define ONE_MIN 1
#define ONE_MAX 4

#define INITIAL_Y_VEL 10
#define FRAME_TIME 32 // (1000 / 30FPS) - 1 = 32
#define MOVEMENT_SPEED 3

#define BG_SPRITE_AMOUNT 192
#define BG_SPRITE_WIDTH 10
#define BG_SPRITE_ACTUAL_WIDTH 20
#define BG_SPRITE_HEIGHT 10

#define GRAVITY 1

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