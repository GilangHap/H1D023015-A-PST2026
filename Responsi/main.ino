#include <Servo.h>

Servo myservo;

const int servoPin = 5;
const int ledPin   = 6;
const int btnUp    = 2;

void setup() {
  myservo.attach(servoPin);          
  pinMode(ledPin, OUTPUT);           
  pinMode(btnUp,  INPUT_PULLUP);     
  Serial.begin(9600);
}

void loop() {
  int btnState = digitalRead(btnUp);

  if (btnState == LOW) {
    myservo.write(90);
    digitalWrite(ledPin, HIGH);
    Serial.println("DRS: ACTIVE");
  } else {                           
    myservo.write(0);
    digitalWrite(ledPin, LOW);
    Serial.println("DRS: CLOSED");
  }
}