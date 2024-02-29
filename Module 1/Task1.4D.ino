#include <avr/io.h>
#include <avr/interrupt.h>

const int pirPin = 4;
const int buttonPin = 5;
int led1 = 13;
int led2 = 11;
int led3 = 10;
volatile uint8_t pirState, buttonState;
volatile uint8_t prepir, prebutton = 0;
const uint16_t timer = 0;
const uint16_t compare = 31250;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT20) | (1 << PCINT21); 

  TCCR1A = 0;
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  TCNT1 = timer;
  OCR1A = compare;
  TIMSK1 = (1 << OCIE1A);
  sei();
}

  void loop() {
}

ISR(PCINT2_vect) {
  if (prepir != digitalRead(pirPin)) {
    digitalWrite(led3, digitalRead(pirPin));
    Serial.println("PIR sensor detected motion");
    prepir = digitalRead(pirPin);
  }
  if (prebutton != digitalRead(buttonPin)) {
    if (digitalRead(led2) == LOW) {
      digitalWrite(led2, HIGH);
    } 
    else {
      digitalWrite(led2, LOW);
    }
    Serial.println("Button pressed");
    prebutton = digitalRead(buttonPin);
  }
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = timer;
  digitalWrite(led1, digitalRead(led1) ^ 1);
}
