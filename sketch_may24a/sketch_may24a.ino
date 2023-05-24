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

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 10;  // the number of the pushbutton pin
const int ledPin = 13;    // the number of the LED pin

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status
int i = 0;

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // Begin I2C communication and init DAC
  Wire.begin();
  dac.begin(0x62);  // The I2C Address: Run the I2C Scanner if you're not sure
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    // Generate a sine wave
    dac.setVoltage((uint16_t)(2048 + 2047 * sin(i * (PI / 180.0))), false);
    i++;
    if (i > 360) i = 0;  // Reset the phase of the sine wave after a full cycle
  } else {
    // turn LED off and mute the DAC:
    digitalWrite(ledPin, LOW);
    dac.setVoltage(0, false);
  }
  // Short delay so the wave isn't too high frequency
  delay(2);
}
