#include <Arduino.h>
#include <Bounce2.h>
#include <ezLED.h>
#include <ResponsiveAnalogRead.h>

// Threshold control values in millivolts
const int minThreshold = 200;
const int maxThreshold = 2100;
const int stepSize = 200;
int threshold = 200;

// Pins
const int piezoPin1 = A2;
const int piezoPin2 = A3;
const int buttonPin = 2;

// On period for LED flash
const int flashDelay = 200;
// Convert adc value to millivolts
const float mvFactor = 4.88; 

// Smoothing factor for ResponsiveAnalogRead
const float snapMultiplier = 1.0f;

Bounce button = Bounce();
ResponsiveAnalogRead piezoSensor1(piezoPin1, false, snapMultiplier);
ResponsiveAnalogRead piezoSensor2(piezoPin2, false, snapMultiplier);
ezLED led(DISPLAY_LED);

void printMillivolts(int v1, int v2);
void printThreshold(int threshold);

void setup() {
  button.attach(buttonPin, INPUT_PULLUP);
  button.interval(5);
  led.blinkNumberOfTimes(400, 400, 2);
#ifdef UNO
  Serial.begin(9600);
  Serial.println("Begin");
#endif
}

void loop() {
  piezoSensor1.update();
  piezoSensor2.update();

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

  int piezo1 = (int) piezoSensor1.getValue() * mvFactor;
  int piezo2 = (int) piezoSensor2.getValue() * mvFactor;

  if (piezo1 > threshold || piezo2 > threshold) {
    led.blinkNumberOfTimes(flashDelay, flashDelay, 1);
    printMillivolts(piezo1, piezo2);
  }
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