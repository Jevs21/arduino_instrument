#include <Keypad.h>

#define KEY_PIN_1 2
#define KEY_PIN_2 0
#define KEY_PIN_3 4
#define KEY_PIN_4 16
#define KEY_PIN_5 17
#define KEY_PIN_6 5
#define KEY_PIN_7 18
#define KEY_PIN_8 19

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte colPins[COLS] = {11, 10, 8, 7};
byte rowPins[ROWS] = {6, 5, 4, 3 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
}


void loop() {
  keypad.getKeys();

  bool keyFound = false;
  for (int i = 0; i < LIST_MAX; i++) {
    if (keypad.key[i].kstate == PRESSED) {
      Serial.println(keypad.key[i].kchar);
      keyFound = true;
    }
  }

  if (!keyFound) {
    Serial.println("No keys");
  }

  delay(100); // Wait for 100 millisecond(s)
}
