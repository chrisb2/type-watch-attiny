#include <Arduino.h>
#include <Bounce2.h>
#include <ezLED.h>
#include <EEPROM.h>

// Threshold control values in millivolts
const int minThreshold = 1200;
const int maxThreshold = 2200;
const int stepSize = 100;
int threshold = 0;

// Pins
const int piezoPin1 = A2;
const int piezoPin2 = A3;
const int buttonPin = 2;

const int eepromAddr = 0;

// On period for LED flash
const int flashDelay = 200;
// Convert adc value to millivolts
const float mvFactor = 4.88f;
// Number of ADC readings to average
const int numReadings = 3; 


Bounce button = Bounce();
ezLED led(DISPLAY_LED);

int readPiezo(int pin);
void printMillivolts(int v1, int v2);
void printThreshold(int threshold);
int readThreshold();
void writeThreshold(int currThreshold);

void setup() {
  button.attach(buttonPin, INPUT_PULLUP);
  button.interval(5);
  led.blinkInPeriod(100, 100, 1000);
  threshold = readThreshold();
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
    writeThreshold(threshold);
#ifdef UNO
    printThreshold(threshold);
#endif
  }

  int piezo1 = readPiezo(piezoPin1);
  int piezo2 = readPiezo(piezoPin2);
  if (piezo1 > threshold || piezo2 > threshold) {
    led.blinkNumberOfTimes(flashDelay, flashDelay, 1);
#ifdef UNO
    printMillivolts(piezo1, piezo2);
#endif
  }
}

int readPiezo(int pin) {
  int sum = 0;
  for(int i = 0; i < numReadings; i++) {
    sum += analogRead(pin);
  }
  int average = sum / numReadings;
  
  // convert the average analog reading to millivolts
  return (int)(average * mvFactor); 
}

int readThreshold() {
  int val = EEPROM.read(eepromAddr);
  return (val * stepSize) + minThreshold;
}

void writeThreshold(int currThreshold) {
  int val = (currThreshold - minThreshold) / stepSize;
  EEPROM.write(eepromAddr, val);
}

#ifdef UNO
void printMillivolts(int v1, int v2) {
  Serial.print(v1);
  Serial.print(", ");
  Serial.println(v2);
}

void printThreshold(int threshold) {
    Serial.println(threshold);
}
#endif
