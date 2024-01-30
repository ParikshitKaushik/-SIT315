int PIRSensorPin = 3;
int ledPin = 13; 

void setup() {
  pinMode(PIRSensorPin, INPUT); 
  pinMode(ledPin, OUTPUT); 
  Serial.begin(9600); 
}

void loop() {
  int PIRState = digitalRead(pirSensorPin);
  if (PIRState == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Motion detected!");
    delay(1000); 
  } else {
    digitalWrite(ledPin, LOW);
  }
}
