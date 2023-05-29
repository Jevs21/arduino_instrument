/*
  Button

  Plays a sin wave on the MCP4725 DAC when a pushbutton attached to pin 10 is pressed.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 10 from +5V
  - 10K resistor attached to pin 10 from ground
  - MCP4725 attached to A4 (SDA), A5 (SCL)

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe
  Modified May 2023
  by Your Name

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/

#include <Wire.h>
#include <Adafruit_MCP4725.h>

// MCP4725 DAC
Adafruit_MCP4725 dac;

// Define the range of input pins
const int START_PIN = 6;
const int END_PIN = 13;

void setup() {
  // Begin serial communication at 9600 baud
  Serial.begin(9600);
  
  // Set each pin from 6 to 13 as an INPUT
  for(int i = START_PIN; i <= END_PIN; i++) {
    pinMode(i, INPUT);
  }
}

void loop() {
  // For each pin from 6 to 13
  for(int i = START_PIN; i <= END_PIN; i++) {
    // If the button on that pin is pressed (LOW because the button is connected to the ground through a resistor when pressed)
    if(digitalRead(i) == LOW) {
      // Print the pin number to the serial monitor
      Serial.println(i);
      delay(100);  // add a delay to debounce the button press
    }
  }
}
