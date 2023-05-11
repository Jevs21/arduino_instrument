#include <Arduino.h>
// #include <SPI.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <MozziGuts.h>
#include <Oscil.h> // oscillator class
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>

// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define DEBUG 1

#define KEY_PIN_1 2
#define KEY_PIN_2 0
#define KEY_PIN_3 4
#define KEY_PIN_4 16
#define KEY_PIN_5 17
#define KEY_PIN_6 5
#define KEY_PIN_7 18
#define KEY_PIN_8 19

#define KNOB_PIN_1 A0
#define KNOB_PIN_2 A1
#define KNOB_PIN_3 A2
#define KNOB_PIN_4 A3

#define SPEAKER_PIN 9

#define CONTROL_RATE 64

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> oscilSin(SIN2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> oscilSaw(SAW2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> oscilSquare(SQUARE_NO_ALIAS_2048_DATA);

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> oscilMod(SIN2048_DATA);
Oscil<2048, CONTROL_RATE> oscilVib(SIN2048_DATA);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[COLS] = {11, 10, 8, 7};
byte rowPins[ROWS] = {6, 5, 4, 3 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// int frequencies[ROWS][COLS] = {
//   {262, 294, 330, 349},
//   {392, 440, 494, 523},
//   {587, 659, 698, 784},
//   {880, 988, 1047, 1175}
// };
int frequencies[ROWS * COLS] = {
  262, 294, 330, 349,
  392, 440, 494, 523,
  587, 659, 698, 784,
  880, 988, 1047, 1175
};

float freq, amp, modFreq, vibDepth;
int waveform;
bool keyIsPressed = false;

void setup(){
  Serial.begin(115200);
  oscilVib.setFreq(6.5f);
  startMozzi(CONTROL_RATE); // start Mozzi
}

void updateControl(){
  keypad.getKeys();
  keyIsPressed = false;
  for (int i = 0; i < LIST_MAX; i++) {
    if (keypad.key[i].kstate == PRESSED) {
      Serial.println(keypad.key[i].kchar);
      // freq = frequencies[i];
      keyIsPressed = true;
    }
  }

  // if (!keyIsPressed) {
  //   Serial.println("No keys");
  // }

  int knob1Val = mozziAnalogRead(KNOB_PIN_1);
  freq = map(knob1Val, 0, 1023, 262, 1175);

  int knob3Val = mozziAnalogRead(KNOB_PIN_3);
  vibDepth = map(knob3Val, 0, 1023, 0, 100) / 100.0f;

  int knob4Val = mozziAnalogRead(KNOB_PIN_4);
  waveform = map(knob4Val, 0, 1023, 0, 3);

  Serial.print("Frequency: ");
  Serial.print(freq);
  Serial.print(" Amplitude: ");
  Serial.print(amp);
  Serial.print(" Vibrato (3): ");
  Serial.print(vibDepth);
  Serial.print(" Waveform (4): ");
  Serial.println(waveform);
}

int updateAudio() {
  if (keyIsPressed) {
    float vibrato = oscilVib.next() * vibDepth;
    if (waveform == 0) {
      oscilSin.setFreq(freq);
      // oscilSin.setFreq(freq + vibrato);
      // oscilSin.setFreq(freq + oscilMod.next());
      return oscilSin.next();
    } else if (waveform == 1) {
      oscilSaw.setFreq(freq + vibrato);
      // oscilSaw.setFreq(freq + oscilMod.next());
      return oscilSaw.next();
    } else {
      oscilSquare.setFreq(freq + vibrato);
      // oscilSquare.setFreq(freq + oscilMod.next());
      return oscilSquare.next();
    }
  }
  // oscilSin.setFreq(freq);
  //     // oscilSin.setFreq(freq + vibrato);
  //     // oscilSin.setFreq(freq + oscilMod.next());
  //     return oscilSin.next();
  return 0;  // return 0 when no key is pressed
}

void loop() {
  updateControl();
  audioHook();
  // delay(100);
}


// int[] getKnobValues() {
//   int knob1Val = mozziAnalogRead(KNOB_PIN_1);
//   int knob2Val = mozziAnalogRead(KNOB_PIN_2);
//   int knob3Val = mozziAnalogRead(KNOB_PIN_3);
//   int knob4Val = mozziAnalogRead(KNOB_PIN_4);
//   return {knob1Val, knob2Val, knob3Val, knob4Val};
// }
