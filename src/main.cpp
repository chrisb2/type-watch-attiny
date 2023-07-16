#include <Arduino.h>
#include <Bounce2.h>
#include <ezLED.h>

const int piezoPin1 = A2;
const int piezoPin2 = A3;
const int buttonPin = 2;

const int numReadings = 5;
const int flashDelay = 200;

// Threshold control values in millivolts
int threshold = 400;
const int minThreshold = 400;
const int maxThreshold = 800;
const int stepSize = 50;

Bounce button = Bounce();
ezLED led(DISPLAY_LED);

void printMillivolts(int v1, int v2);
void printThreshold(int threshold);
int readPiezo(int pin);

void setup() {
  button.attach(buttonPin, INPUT_PULLUP);
  button.interval(5);
#ifdef UNO
  Serial.begin(9600);
  Serial.println("Begin");
#endif
}

void loop() {
  led.loop();

  button.update();
  if (button.fell()) {
    threshold += stepSize;
    if (threshold > maxThreshold) {
      threshold = minThreshold;
      led.blinkNumberOfTimes(50, 50, 4);
    } else {
      led.blinkNumberOfTimes(100, 100, 1);
    }
    printThreshold(threshold);
  }

  int piezo1 = readPiezo(piezoPin1);
  int piezo2 = readPiezo(piezoPin2);

  if (piezo1 > threshold || piezo2 > threshold) {
    led.blinkNumberOfTimes(flashDelay, flashDelay, 1);
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