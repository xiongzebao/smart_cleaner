/*
 Name:		ai_car_vs.ino
 Created:	2019/6/4 14:41:08
 Author:	xiongbin
*/
#include "IRremoteCharge.h"
#include "MyUtils.h"
#include "Velometer.h"
#include "MyMotor.h"
#include "MotorController.h"
#include "SerialController.h"
#include "port.h"
#include "Controller.h"
#include "FlexiTimer2.h"
#include "IRremoteLocation.h"
#include "SoftwareSerial.h"


//SoftwareSerial mySerial(10, 11); // RX, TX


/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
void setPwmFrequency(int pin, int divisor) {
	byte mode;
	if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
		switch (divisor) {
		case 1: mode = 0x01; break;
		case 8: mode = 0x02; break;
		case 64: mode = 0x03; break;
		case 256: mode = 0x04; break;
		case 1024: mode = 0x05; break;
		default: return;
		}
		if (pin == 5 || pin == 6) {
			TCCR0B = TCCR0B & 0b11111000 | mode;
		}
		else {
			TCCR1B = TCCR1B & 0b11111000 | mode;
		}
	}
	else if (pin == 3 || pin == 11) {
		switch (divisor) {
		case 1: mode = 0x01; break;
		case 8: mode = 0x02; break;
		case 32: mode = 0x03; break;
		case 64: mode = 0x04; break;
		case 128: mode = 0x05; break;
		case 256: mode = 0x06; break;
		case 1024: mode = 0x07; break;
		default: return;
		}
		TCCR2B = TCCR2B & 0b11111000 | mode;
	}
}

MyMotorClass leftMotor = MyMotorClass(motorA1, motorA2, pwdA);
MyMotorClass rightMotor = MyMotorClass(motorB2, motorB1, pwdB);
MotorControllerClass mc = MotorControllerClass(&rightMotor, &leftMotor);
Controller* ctrl = Controller::newInstance(&mc);
IRremoteLocation ir;

// Set pin 9's PWM frequency to 3906 Hz (31250/8 = 3906)
// Note that the base frequency for pins 3, 9, 10, and 11 is 31250 Hz
//setPwmFrequency(9, 8);

// Set pin 6's PWM frequency to 62500 Hz (62500/1 = 62500)
// Note that the base frequency for pins 5 and 6 is 62500 Hz
//setPwmFrequency(6, 1);

// Set pin 10's PWM frequency to 31 Hz (31250/1024 = 31)
//setPwmFrequency(10, 1024);


void setPwmFrequency2560(int pin, int divisor) {
	byte mode;
	if ((pin >= 2 && pin <= 13) || (pin >= 44 && pin <= 46))
	{
		switch (divisor) {
		case 1: mode = 0x01; break;
		case 8: mode = 0x02; break;
		case 64: mode = 0x03; break;
		case 256: mode = 0x04; break;
		case 1024: mode = 0x05; break;
		default: return;
		}
		if (pin == 4 || pin == 13)//Timer0
		{
			TCCR0B = TCCR0B & 0b11111000 | mode;
		}
		else if (pin == 11 || pin == 12 || pin == 13)//Timer1
		{
			TCCR1B = TCCR1B & 0b11111000 | mode;
		}
		else if (pin == 8 || pin == 9)//Timer2
		{
			TCCR2B = TCCR2B & 0b11111000 | mode;
		}
		else if (pin == 5 || pin == 2 || pin == 3)//Timer3
		{
			TCCR3B = TCCR3B & 0b11111000 | mode;
		}
		else if (pin == 6 || pin == 7 || pin == 8)//Timer4
		{
			TCCR4B = TCCR4B & 0b11111000 | mode;
		}
		else if (pin == 46 || pin == 45 || pin == 44)//Timer5
		{
			TCCR5B = TCCR5B & 0b11111000 | mode;
		}

	}

}

unsigned long charge_time = 0;

void onTimer() {
 
	//charge_time++;
	//if (charge_time == 60) {
	//	Serial.println("charge...");
	//	
	//	ir.request_open_irlaser();
	//	
	//	charge_time = 0;
	//}

}

void setup()
{
 
	SerialController.init(&mc);
	ir.init_irremote_location(&mc);
	setPwmFrequency2560(6, 1024);
	//setPwmFrequency2560(6, 256);
	MyUtils.println("welcome to clean robot");
	FlexiTimer2::set(Constant::velometerPeriod * 1000, onTimer); // 500ms period
	FlexiTimer2::start();
	mc.setSpeed(200);
	randomSeed(analogRead(0));
	//ir.request_open_irlaser();
	//ctrl->Lock();
	mc.forward();
	 

}





void loop()
{
	ctrl->loop();
	//ir.findIR();
 
}

void serialEvent() {
	SerialController.onSerialEvent();

}