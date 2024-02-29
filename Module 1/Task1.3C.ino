const int PIRSensorPin = 3; 
const int ArduinoPin= 2;
const int tmp36Pin = A0; 
const int ledPin = 13;
const int ArduinoLED = 10;
volatile bool motionDetected = false;

void setup() {
  pinMode(PIRSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIRSensorPin), motionDetectedIsTrue, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ArduinoPin), Arduino, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if (motionDetected) {
    float temp = getTemperature();
    Serial.println("Motion detected!");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
    digitalWrite(ledPin, HIGH);
    delay(1000);
    motionDetected = false; 
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void motionDetectedIsTrue() {
  motionDetected = true;
}
void Arduino(){
	digitalWrite(ArduinoLED,HIGH);
  	delay(1000);
  	digitalWrite(ArduinoLED,LOW);
  
}
float getTemperature() {
  int sensorValue = analogRead(tmp36Pin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100.0;

  return temperature;
}
