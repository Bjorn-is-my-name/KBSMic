/*
 * read data from nunchuk and write to Serial
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include <Nunchuk.h>

#define NUNCHUK_ADDRESS 0x52
#define WAIT		10
#define BAUDRATE	9600

// what to show
#define STATE

bool show_state(void);

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
	Serial.flush();

	// handshake with nunchuk
	Serial.print("-------- Connecting to nunchuk at address 0x");
	Serial.println(NUNCHUK_ADDRESS, HEX);
	if (!Nunchuk.begin(NUNCHUK_ADDRESS))
	{
		Serial.println("******** No nunchuk found");
		Serial.flush();
		return(1);
	}

	/*
	 * get identificaton (nunchuk should be 0xA4200000)
	 */
	Serial.print("-------- Nunchuk with Id: ");
	Serial.println(Nunchuk.id);

	// endless loop
	while(1) {
#ifdef STATE
		if (!show_state()) {
			Serial.println("******** No nunchuk found");
			Serial.flush();
			return(1);
		}
#endif
		// wait a while
		_delay_ms(WAIT);
	}

	return(0);
}

bool show_state(void)
{
	if (!Nunchuk.getState(NUNCHUK_ADDRESS))
		return (false);
	// Serial.println("------State data--------------------------");
	Serial.print("Joy X: ");
	Serial.print(Nunchuk.state.joy_x_axis, HEX);
	// Serial.print("\t\tAccel X: ");
	// Serial.print(Nunchuk.state.accel_x_axis, HEX);
	// Serial.print("\t\tButton C: ");
	// Serial.println(Nunchuk.state.c_button, HEX);

	// Serial.print("Joy Y: ");
	// Serial.print(Nunchuk.state.joy_y_axis, HEX);
	// Serial.print("\t\tAccel Y: ");
	// Serial.print(Nunchuk.state.accel_y_axis, HEX);
	// Serial.print("\t\tButton Z: ");
	// Serial.println(Nunchuk.state.z_button, HEX);

	Serial.print("\t\t\tAccel Z: ");
	Serial.println(Nunchuk.state.accel_z_axis, HEX);

	return(true);
}