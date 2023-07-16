#include <Arduino.h>
#include <Bounce2.h>

const int piezoPin1 = A2;
const int piezoPin2 = A3;
const int ledPin = DISPLAY_LED;
const int buttonPin = 2;

const int numReadings = 5;
const int flashDelay = 200;

int threshold = 300;
const int minThreshold = 300;
const int maxThreshold = 500;
const int stepSize = 50;

Bounce button = Bounce();

void flashLed();
void printMillivolts(int v1, int v2);
void printThreshold(int threshold);
int readPiezo(int pin);

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  button.attach(buttonPin);
  button.interval(5);

#ifdef UNO
  Serial.begin(9600);
  Serial.println("Begin");
#endif
}

void loop() {
  button.update();
  if (button.fell()) {
    threshold += stepSize;
    if (threshold > maxThreshold) {
      threshold = minThreshold;
    }
    flashLed();
    printThreshold(threshold);
  }

  int piezo1 = readPiezo(piezoPin1);
  int piezo2 = readPiezo(piezoPin2);

  if (piezo1 > threshold || piezo2 > threshold) {
    flashLed();
    printMillivolts(piezo1, piezo2);
  }
}

int readPiezo(int pin) {
  int sum = 0;
  for(int i = 0; i < numReadings; i++) {
    sum += analogRead(pin);
  }
  int average = sum / numReadings;
  
  // convert the average analog reading to millivolts
  return (int)(average * 4.8828125); 
}

void flashLed() {
  digitalWrite(ledPin, HIGH); // On
  delay(flashDelay);
  digitalWrite(ledPin, LOW); // Off
}

void printMillivolts(int v1, int v2) {
#ifdef UNO
  Serial.print(v1);
  Serial.print(", ");
  Serial.println(v2);
#endif
}

void printThreshold(int threshold) {
#ifdef UNO
    Serial.println(threshold);
#endif
}