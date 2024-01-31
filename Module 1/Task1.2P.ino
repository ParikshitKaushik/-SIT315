const int PIRSensorPin = 3; 
const int ledPin = 13;
volatile bool motionDetected = false;

void setup() {
  pinMode(PIRSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIRSensorPin), motionDetectedIsTrue, CHANGE);
  Serial.begin(9600);
}

void loop() {
  if (motionDetected) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Motion detected!");
    delay(1000);
    motionDetected = false; 
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void motionDetectedIsTrue() {
  motionDetected = true;
}
