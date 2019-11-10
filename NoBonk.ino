#include <BluetoothSerial.h>

const int enable = 13;
const int enableChannel = 0;

const int echo = 27;
bool echoHold = false;
unsigned long echoStart;

const int trigger = 26;

const int horn = 25;

const int servo = 33;
const int servoChannel = 1;
const int servoFrequency = 50;

float distances[2] = {100.0, 100.0};
float distanceTimes[2];
float velocity;
float ttc; //time to collsion
bool ttcStop = false;

const unsigned long measurePeriod = 30;

const unsigned long collisionCutoff = 500;

BluetoothSerial Serialbt;

void setup() {
	ledcSetup(enableChannel, servoFrequency, 8);
	ledcAttachPin(enable, enableChannel);
	ledcWrite(enableChannel, 0);

	pinMode(echo, INPUT);
	pinMode(trigger, OUTPUT);
	digitalWrite(trigger, LOW);

	pinMode(horn, OUTPUT);
	digitalWrite(horn, LOW);

	ledcSetup(servoChannel, servoFrequency, 8);
	ledcAttachPin(servo, servoChannel);
	ledcWrite(servoChannel, 19);

  Serial.begin(9600);
  Serial.println("Hello!");

	Serialbt.begin("NoBonk ESP");
//	bool linked = Serialbt.connect("OnePlus 6T");
//  while(!linked) {
//     linked = Serialbt.connected(10000);
//     Serial.println("trying again...");
//  }
//  Serial.println("Connected!");
//  digitalWrite(horn, HIGH);
//  delay(200);
//  digitalWrite(horn, LOW);
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

  Serial.println(distances[0]);

	// bluetooth in
	bool lastDir;
	int speed;
	int steer;
	bool honk;
	while(Serialbt.available()) {
		int msg = Serialbt.read();
		if((msg == 'w') && (distances[0] >= 20.0)) {
			speed = 26;
			lastDir = true;
		} else if(msg == 's') {
			speed = 11;
			lastDir = false;
		} else if(msg == 'd') {
			steer = map(20, 0, 180, 10, 26);
			if(lastDir) {
				speed = 26;
			} else {
				speed = 13;
			}
		} else if(msg == 'a') {
			steer = map(135, 0, 180, 10, 26);
			if(lastDir) {
				speed = 26;
			} else {
				speed = 13;
			}
		} else if(msg == 'e') {
			honk = true;
		} else if(msg == '0') {
			speed = map(90, 0, 180, 13, 26);
			steer = map(75, 0, 180, 10, 26);
		} else if(msg == 'o') {
			honk = false;
		}
	}


	// set motors
	if(!ttcStop) {
		ledcWrite(enableChannel, speed);
	} else {
		honk = true;
	}
	ledcWrite(servoChannel, steer);

	if(honk) {
		digitalWrite(horn, HIGH);
	} else {
		digitalWrite(horn, LOW);
	}
}
