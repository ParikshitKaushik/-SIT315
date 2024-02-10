const int PIRSensorPin = 3;
const int tmp36Pin = A0;
const int tiltSensorPin = 2;  
const int ledPin = PB5;
volatile bool motionDetected = false;
volatile bool tiltDetected = false;

void attachPCINT(int pin, void (*function)()) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH); 
  attachInterrupt(digitalPinToInterrupt(pin), function, CHANGE);
}

void setup() {
  pinMode(PIRSensorPin, INPUT);
  pinMode(tiltSensorPin, INPUT);
  DDRB != (1<<ledPin);
  attachPCINT(PIRSensorPin, motionDetectedIsTrue);
  attachPCINT(tiltSensorPin, tiltDetectedIsTrue);
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
  } else if (tiltDetected) {
    Serial.println("Tilt detected!");
    digitalWrite(ledPin, HIGH);
    delay(1000);
    tiltDetected = false;
  } else {
    digitalWrite(ledPin, LOW);
  }
  PORTB ^= (1 << ledPin);
  delay(500);
}

void motionDetectedIsTrue() {
  motionDetected = true;
}
float getTemperature() {
  int sensorValue = analogRead(tmp36Pin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100.0;

  return temperature;
}
void tiltDetectedIsTrue() {
  tiltDetected = true;
}