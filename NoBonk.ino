#include <BluetoothSerial.h>

const int enable = 13;
const int enableChannel = 0;
const int enableFrequency = 1000;

const int a1 = 12;
const int a2 = 14;

const int echo = 27;
const int trigger = 26;

const int horn = 25;

const int servo = 33;
const int servoChannel = 1;
const int servoFrequency = 50;


void setup() {
	ledcSetup(enableChannel, enableFrequency, 8);
	ledcAttachPin(enable, enableChannel);
	ledcWrite(enableChannel, 0);

	pinMode(a1, OUTPUT);
	digitalWrite(a1, LOW);
	pinMode(a2, OUTPUT);
	digitalWrite(a2, LOW);

	pinMode(echo, INPUT);
	pinMode(trigger, OUTPUT);
	digitalWrite(trigger, LOW);

	pinMode(horn, OUTPUT);
	digitalWrite(horn, LOW);

	ledcSetup(servoChannel, servoFrequency, QW);
	ledcAttachPin(servo, servoChannel);
	ledcWrite(servoChannel, 19)
}


void main() {
	
}