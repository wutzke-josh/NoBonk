//
// Connect the Grove module to Serial 1
// Grove Pin   Arduino Pin
//   RX           TX1 18
//   TX           RX1 19

void setup()
{
  // Main Arduino Serial IF
  Serial.begin(9600);
  // Grove Bluetooth Serial IF
  Serial1.begin(921600);

  Serial.println("starting");
    
  setupBlueToothConnection();
  //wait 1s and flush the serial buffer
    delay(1000);
    Serial.flush();
}

char input;

void loop()
{

  Serial1.print("hey josh");
  
  if(Serial1.available()){
    Serial.print(".");
        input = Serial1.read();
        Serial.print(input);
    }
    
    delay(100);
}


void setupBlueToothConnection()
{
	Serial1.print("AT");
	delay(400); 
  while(Serial1.available()) {
    input = Serial1.read();
    Serial.print(input);
  }
  Serial.println("");
	
	Serial1.print("AT+DEFAULT");             // Restore all setup value to factory setup
	delay(2000); 
  while(Serial1.available()) {
    input = Serial1.read();
    Serial.print(input);
  }
  Serial.println("");
	
  Serial1.print("AT+NAME?"); 
  delay(400);
  while(Serial1.available()) {
    input = Serial1.read();
    Serial.print(input);
  }
  Serial.println("");
  
  Serial1.print("AT+NAMECactusCont"); // Name must be <= 12 chars
  delay(400);
  while(Serial1.available()) {
    input = Serial1.read();
    Serial.print(input);
  }
  Serial.println("");

	Serial1.print("AT+ROLES");             // set the bluetooth work in slave mode
	delay(400); 
  while(Serial1.available()) {
    input = Serial1.read();
    Serial.print(input);
  }
  Serial.println("");

  Serial1.print("AT+BAUDB");
  delay(400);

  Serial1.print("AT+DISC?");
  delay(400); 
  while(Serial1.available()) {
    input = Serial1.read();
    Serial.print(input);
  }
  Serial.println("");

  Serial1.print("AT+PIN?");
  delay(400); 
  while(Serial1.available()) {
    input = Serial1.read();
    Serial.print(input);
  }
  Serial.println("");
Serial.println("end check");
	
//	Serial1.print("AT+AUTH1");            
//  delay(400);    
	
	Serial1.print("AT+CLEAR");             // Clear connected device mac address
  delay(400);   
  while(Serial1.available()) {
    input = Serial1.read();
    Serial.print(input);
  }
  Serial.println("");
	
  Serial1.flush();
}
