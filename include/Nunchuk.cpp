#include <util/delay.h>
#include <Wire.h>
#include <Defines.c>

// nibble to hex ascii
char btoa[] = {'0', '1', '2', '3', '4', '5', '6', '7',
               '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

struct ncState
{
    uint8_t joy_x_axis;
    uint8_t joy_y_axis;
    //    uint16_t accel_x_axis;
    //    uint16_t accel_y_axis;
    // uint16_t accel_z_axis;
    uint8_t z_button;
    uint8_t z_button_old;
    uint8_t c_button;
    uint8_t c_button_old;
} state;

bool startNunchuk(uint8_t);

bool begin(uint8_t address);

bool getState(uint8_t address);

char id[2 * IDLEN + 3]; // '0xAABBCCDD\0'
uint8_t read(uint8_t address, uint8_t offset, uint8_t len);

static uint8_t nunchuck_buffer[CHUNKLEN];

bool _getId(uint8_t address);

uint8_t _read(uint8_t address, uint8_t offset, uint8_t len);

bool startNunchuk(uint8_t address)
{
    {
        Wire.beginTransmission(address);
        Wire.write(0xF0);
        Wire.write(0x55);
        Wire.endTransmission(true);
        Wire.beginTransmission(address);
        Wire.write(0xFB);
        Wire.write(0x00);
        Wire.endTransmission(true);
    }

    // get the id
    return (_getId(address));
}

bool getState(uint8_t address)
{
    // read state from memory address
    if (_read(address, NCSTATE, STATELEN) != STATELEN)
        return (false);

    // set parameters
    state.joy_x_axis = nunchuck_buffer[0];
    state.joy_y_axis = nunchuck_buffer[1];
    //    state.accel_x_axis = (nunchuck_buffer[2] << 2) | ((nunchuck_buffer[5] & 0x0C) >> 2);
    //    state.accel_y_axis = (nunchuck_buffer[3] << 2) | ((nunchuck_buffer[5] & 0x30) >> 4);
    // state.accel_z_axis = (nunchuck_buffer[4] << 2) | ((nunchuck_buffer[5] & 0xC0) >> 6);
    /* 0 = pressed */
    state.z_button_old = state.z_button;
    state.c_button_old = state.c_button;
    state.z_button = !(nunchuck_buffer[5] & 0x01);
    state.c_button = !((nunchuck_buffer[5] & 0x02) >> 1);

    return (true);
}

uint8_t read(uint8_t address, uint8_t offset, uint8_t len)
{
    return (_read(address, offset, len));
}

bool _getId(uint8_t address)
{
    // read data from address
    if (_read(address, NCID, IDLEN) != IDLEN)
        return false;

    // copy nunchuck_buffer to id string
    id[0] = '0';
    id[1] = 'x';
    for (uint8_t i = 0; i < IDLEN; i++)
    {
        id[2 + 2 * i] = btoa[(nunchuck_buffer[i] >> 4)];
        id[2 + 2 * i + 1] = btoa[(nunchuck_buffer[i] & 0x0F)];
    }
    id[2 * IDLEN + 2] = '\0';

    return true;
}

uint8_t _read(uint8_t address, uint8_t offset, uint8_t len)
{
    uint8_t n = 0;

    // send offset
    Wire.beginTransmission(address);
    Wire.write(offset);
    Wire.endTransmission(true);

    // wait untill arrived
    _delay_ms(WAITFORREAD);

    // request len bytes
    Wire.requestFrom(address, len);

    // read bytes
    while (Wire.available() && n <= len)
    {
        nunchuck_buffer[n++] = Wire.read();
    }

    /* return nr bytes */
    return (n);
}