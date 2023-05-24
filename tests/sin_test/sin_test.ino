#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/cos2048_int8.h> // cosine table for vibrato
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <tables/saw2048_int8.h> // sine table for oscillator
#include <tables/square_no_alias_2048_int8.h>
#include <Keypad.h>
#include <LowPassFilter.h>

// TODO: FIgure out way to implement screen through
// IC2 pins with the mozzi library

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin1(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin2(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin3(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin4(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin[] = {aSin1, aSin2, aSin3, aSin4};

Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> aSaw1(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> aSaw2(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> aSaw3(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> aSaw4(SAW2048_DATA);
Oscil<SAW2048_NUM_CELLS, AUDIO_RATE> aSaw[] = {aSaw1, aSaw2, aSaw3, aSaw4};

Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> aSqu1(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> aSqu2(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> aSqu3(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> aSqu4(SQUARE_NO_ALIAS_2048_DATA);
Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> aSqu[] = {aSqu1, aSqu2, aSqu3, aSqu4};

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCos1(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCos2(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCos3(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCos4(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCos[] = {aCos1, aCos2, aCos3, aCos4};

Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aVibrato(COS2048_DATA);

LowPassFilter lpf;

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64 // Hz, powers of 2 are most reliable

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

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
int frequencies[ROWS][COLS] = {
  {262, 294, 330, 349},
  {392, 440, 494, 523},
  {587, 659, 698, 784},
  {880, 988, 1047, 1175}
};
byte colPins[COLS] = {11, 10, 8, 7};
byte rowPins[ROWS] = {6, 5, 4, 3 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int keyCount = 0;
bool keyFound = false;
int freq = 262;
int waveform = 0;
int intensity = 255;
int filtCutoff = 0;
int resonance = 0;

void setup(){
  Serial.begin(115200);
  startMozzi(CONTROL_RATE); // :)
  // aSin.setFreq(freq); // set the frequency
  // aSaw.setFreq(freq);
  // aSqu.setFreq(freq);
  // aCos.setFreq(freq);
  aVibrato.setFreq(15.f);
  // lpf.setResonance(10);
}

void updateControl(){
  keypad.getKeys();
  // char key = keypad.getKey();
  keyCount = 0;
  bool curFound = false;
  for (int i = 0; i < LIST_MAX; i++) {
    if (keypad.key[i].kstate == PRESSED) {
      for (byte j = 0; j < ROWS; j++) {
        for (byte k = 0; k < COLS; k++) {
          if (keys[j][k] == keypad.key[i].kchar) {
            freq = frequencies[j][k];
            curFound = true;
            keyFound = true;

            aSin[keyCount].setFreq(freq);
            aSaw[keyCount].setFreq(freq);
            aSqu[keyCount].setFreq(freq);

            Serial.print(i);
            Serial.print(" ");
            Serial.println(keypad.key[i].kchar);

            keyCount++;
          }
        }
      }
    }
  }

  if (!curFound) {
    keyFound = false;
  }

  if (keyFound) {
    // aSin.setFreq(freq);
    // aSaw.setFreq(freq);
    // aCos.setFreq(freq);
    // aSqu.setFreq(freq);

    int knob1 = mozziAnalogRead(KNOB_PIN_1);
    int knob2 = mozziAnalogRead(KNOB_PIN_2);
    int knob3 = mozziAnalogRead(KNOB_PIN_3);
    int knob4 = mozziAnalogRead(KNOB_PIN_4);
    
    resonance = map(knob1, 0, 1023, 0, 255);
    filtCutoff = map(knob2, 0, 1023, 10, 255);
    intensity = map(knob3, 0, 1023, 0, 255);
    waveform = map(knob4, 0, 1023, 0, 3);

    lpf.setCutoffFreqAndResonance(filtCutoff, resonance);

    Serial.print("Freq: ");
    Serial.print(freq);
    Serial.print(" - NA (1): ");
    Serial.print(knob1);
    Serial.print("=>");
    Serial.print("NA");
    Serial.print(", HPF (2): ");
    Serial.print(knob2);
    Serial.print("=>");
    Serial.print(filtCutoff);
    Serial.print(", Vibrato (3): ");
    Serial.print(knob3);
    Serial.print("=>");
    Serial.print(intensity);
    Serial.print(", Wave (4): ");
    Serial.print(knob4);
    Serial.print("=>");
    Serial.println(waveform);
  }
}


int updateAudio(){
  if (keyFound) {
    Q15n16 vibrato = (Q15n16) intensity * aVibrato.next();
    int asig = 0;
    if (waveform == 0) {
      for (int i = 0; i < keyCount; i++) {
        asig += aSin[i].phMod(vibrato);
      }
      // int lowPassSig = LPF.next(asig >> 3);
      int lpsig = lpf.next(asig >> 3);
      return (asig >> 3) - lpsig;
      // return aSin.phMod(vibrato); // return an int signal centred around 0
    } else if (waveform == 1) {
      for (int i = 0; i < keyCount; i++) {
        asig += aSaw[i].phMod(vibrato);
      }
      return asig >> 3; // return an int signal centred around 0
    } else if (waveform == 2) {
      for (int i = 0; i < keyCount; i++) {
        asig += aSqu[i].phMod(vibrato);
      }
      return asig >> 3;
    } else {
      for (int i = 0; i < keyCount; i++) {
        asig += aCos[i].phMod(vibrato);
      }
      return asig >> 3; // return an int signal centred around 0
    }
  } else {
    return 0;
  }
}


void loop(){
  audioHook(); // required here
}