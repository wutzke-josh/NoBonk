#include <BluetoothSerial.h>

const int enable = 13;
const int enableChannel = 0;
const int enableFrequency = 1000;

const int a1 = 12;
const int a2 = 14;

const int echo = 27;
bool echoHold = false;
unsigned long echoStart;

const int trigger = 26;

const int horn = 25;

const int servo = 33;
const int servoChannel = 1;
const int servoFrequency = 50;

float distances[2];
float distanceTimes[2];
float velocity;
float ttc; //time to collsion
bool ttcStop = false;

const unsigned long measurePeriod = 30;

const unsigned long collisionCutoff = 500;

BluetoothSerial Serialbt;

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
	ledcWrite(servoChannel, 19);

	Serialbt.begin("NoBonk ESP");
}


void loop() {
	bool echoState = digitalRead(echo);

	// echoTime & find new distance
	if(echoState && !echoHold) {
		echoHold = true;
		echoStart = micros();
	}

	if(!echoState && echoHold) {
		echoHold = false;
		float measure = (float)(micros() - echoStart) / 58.0;
		if(measure < 400.0) {
			distances[1] = distances[0];
			distances[0] = measure;
			distanceTimes[1] = distanceTimes[0];
			distanceTimes[0] = millis();
		}	
	}

	// new trigger
	if(millis() - distanceTimes[0] >= measurePeriod) {
		digitalWrite(trigger, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigger, LOW);
	}

	// velocity in m/s
	velocity = (distances[1] - distances[0]) / (distanceTimes[0] - distanceTimes[1]) / 10;

	// time to colision in ms
	ttc = distances[0] / velocity / 10;

	// time to colision power setting
	if(((unsigned long)ttc) <= collisionCutoff) {
		ttcStop = true;
		digitalWrite(a1, HIGH);
		digitalWrite(a2, HIGH);
		ledcWrite(enableChannel, 255);
	} else {
		ttcStop = false;
	}

	// bluetooth in

	// set motors

	// set servo

	// horn
}
